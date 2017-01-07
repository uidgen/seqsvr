/*
 *  Copyright (c) 2016, https://github.com/nebula-im
 *  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "allocsvr/allocsvr_manager.h"

#include <boost/filesystem.hpp>
#include <folly/FileUtil.h>
#include <folly/Range.h>

#include "seqsvr/message_handler_util.h"
#include "storesvr/storesvr_manager.h"
/*
 1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
 2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：
    如果cur_seq > max_seq，将分配上限提升一个步长max_seq += step，并持久化max_seq
 3. 重启时，读出持久化的max_seq，赋值给cur_seq
 */
// folly::Singleton<SequenceManager> ;
static folly::Singleton<AllocSvrManager> g_allocsvr_manager;

std::shared_ptr<AllocSvrManager> AllocSvrManager::GetInstance() {
  return g_allocsvr_manager.try_get();
}

void AllocSvrManager::Initialize(uint32_t set_id, uint32_t alloc_id) {
  // 1. 初始化set_id_和alloc_id_
  set_id_ = set_id;
  alloc_id_ = alloc_id;
  
  Load(set_id, alloc_id);
}

uint64_t AllocSvrManager::GetCurrentSequence(uint32_t id) {
#ifdef DEBUG_TEST
  DCHECK(id<kMaxIDSize);
#endif
  std::lock_guard<std::mutex> g(mutex_);
  return cur_seqs_[id];
}

uint64_t AllocSvrManager::FetchNextSequence(uint32_t id) {
#ifdef DEBUG_TEST
  DCHECK(id<kMaxIDSize);
#endif
  
  auto idx = id/kSectionSize;
  std::lock_guard<std::mutex> g(mutex_);
  auto seq = ++cur_seqs_[id];
  if (seq > section_max_seqs_[idx]) {
    ++section_max_seqs_[idx];
    Save(set_id_, alloc_id_, idx, section_max_seqs_[idx]);
  }
  return seq;
}

// bytes
void AllocSvrManager::Load(uint32_t set_id, uint32_t alloc_id) {
  // 2. 去storesvr加载max_seqs
  // 先使用StoreSvrManager加载，跑通流程
  state_ = kAllocWaitLoad;
  
  auto store = StoreSvrManager::GetInstance();
  store->Initialize(set_id, "/tmp/seq.dat");
  std::string max_seqs_data = store->GetSectionsData(set_id, alloc_id);
  OnLoad(max_seqs_data);
}

void AllocSvrManager::Save(uint32_t set_id, uint32_t alloc_id, uint32_t section_id, uint64_t section_max_seq) {
  auto store = StoreSvrManager::GetInstance();
  bool rv = store->SetSectionsData(set_id, alloc_id, section_id, section_max_seq);
  OnSave(rv);
}

void AllocSvrManager::OnLoad(const std::string& data) {
  if (!data.empty()) {
    // TODO(@benqi): 检查数据是否合法
    // 复制数据
    memcpy(section_max_seqs_.data(), data.c_str(), data.length());
    state_ = kAllocLoaded;
  } else {
    state_ = kAllocError;
  }
}

void AllocSvrManager::OnSave(bool result) {
  
}


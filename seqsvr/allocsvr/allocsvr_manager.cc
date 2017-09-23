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
#include <folly/io/async/EventBaseManager.h>

// #include "base/message_handler_util.h"
// #include "storesvr/storesvr_manager.h"

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

void AllocSvrManager::Initialize(nebula::TimerManager* timer_manager,
         const IpAddrInfo& alloc_addr,
         const IpAddrInfoList& store_addr_list) {
  
  alloc_addr_ = alloc_addr;
  store_addr_list_ = store_addr_list;
  
  // 初始化store_client
  client_ = std::make_unique<ClientManager>(
        folly::EventBaseManager::get()->getEventBase(),
        store_addr_list);
  
  LOG(INFO) << "client...";
  // 首先加载路由表
  // 加载成功后再加载max_seq

  lease_ = std::make_unique<LeaseClerk>(timer_manager, client_.get(), this);

  auto evb = folly::EventBaseManager::get()->getEventBase();
  evb->runImmediatelyOrRunInEventBaseThreadAndWait([&]() {
    LOG(INFO) << "evb...";
    lease_->Start();
    // LoadMaxSeq();
  });

/*
  // 1. 初始化set_id_和alloc_id_
  set_id_ = set_id;
  alloc_id_ = alloc_id;
  
  Load(set_id, alloc_id);
 */
}

void AllocSvrManager::Destroy() {
  if (lease_) {
    lease_->Stop();
  }
}

bool AllocSvrManager::GetCurrentSequence(uint32_t id, uint32_t client_version, seqsvr::Sequence& o) {
  if (!cache_my_node_) return false;
  if (state_ != kAllocInited) return false;
  
  for (auto & v : cache_my_node_->section_ranges ) {
    auto idx = CalcSectionID(v.id_begin, v.size, id);
    if (idx.first) {
      o.sequence = cur_seqs_[id-v.id_begin];
      if (client_version < table_.version) {
        o.router = table_;
      }
      return true;
    }
  }

  LOG(INFO) << "GetCurrentSequence - id: " << id
      << ", client_version: " << client_version
      << ", sequence: " << o.sequence;

  return false;
}

// id计算公式
//idx =  (id-node.id_begin)
bool AllocSvrManager::FetchNextSequence(uint32_t id, uint32_t client_version, seqsvr::Sequence& o) {
  if (!cache_my_node_) return false;
  if (state_ != kAllocInited) return false;
  
  for (auto & v : cache_my_node_->section_ranges ) {
    auto idx = CalcSectionID(v.id_begin, v.size, id);
    if (idx.first) {
      o.sequence = ++cur_seqs_[id-v.id_begin];
      if (o.sequence > section_max_seqs_[idx.second]) {
        SaveMaxSeq(id, o.sequence);
        // TODO(@benqi): 使用返回值填充
        section_max_seqs_[idx.second] += kSeqStep;
      }
      if (client_version < table_.version) {
        o.router = table_;
      }
      return true;
    }
  }
  
  LOG(INFO) << "FetchNextSequence - id: " << id
    << ", client_version: " << client_version
    << ", sequence: " << o.sequence;

  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 租约生效
void AllocSvrManager::OnLeaseValid(seqsvr::Router& router) {
  table_ = router;

  seqsvr::RouterNode* p = nullptr;
  for (size_t i=0; i< table_.node_list.size(); ++i) {
    auto& v = table_.node_list[i].node_addr;
    if (v.ip == alloc_addr_.addr && v.port == alloc_addr_.port) {
      p = &(table_.node_list[i]);
      LOG(INFO) << "Lookup alloc node: " << "";

      // TODO(@benqi): wait_load_seq...
      state_ = kAllocWaitLoadSeq;
    }
  }

  if (!p) {
    LOG(ERROR) << "Not found!";
  } else {
    cache_my_node_ = p;
  }
}

// 路由表更新
void AllocSvrManager::OnLeaseUpdated(seqsvr::Router& router) {
//  table_.Swap(table);
}

// 租约失效
void AllocSvrManager::OnLeaseInvalid() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
// bytes
void AllocSvrManager::LoadMaxSeq() {
  // TODO(@beqni): NWR读
  // 2. 去storesvr加载max_seqs
  state_ = kAllocWaitRouteTable;
  
  seqsvr::MaxSeqsData max_seqs_data;
  client_->LoadMaxSeqsData(max_seqs_data);
  OnMaxSeqLoaded(max_seqs_data);

}

void AllocSvrManager::SaveMaxSeq(uint32_t id, uint64_t section_max_seq) {
  // TODO(@beqni): NWR写
  
  client_->SaveMaxSeq(id, section_max_seq);
}

void AllocSvrManager::OnMaxSeqLoaded(seqsvr::MaxSeqsData& data) {
  // TODO(@benqi): 检查数据是否合法
  // 复制数据
  // LOG(INFO) << "OnMaxSeqLoaded - data len = " << data.length();
  section_max_seqs_.swap(data.max_seqs);
  // resize(data.length()/8);
  // LOG(INFO) << "OnMaxSeqLoaded - section_max_seqs size = " << section_max_seqs_.size();
  // memcpy(section_max_seqs_.data(), data.c_str(), data.length());
  
  // 将cur_seq设置为max_seq
  cur_seqs_.resize(section_max_seqs_.size()*kPerSectionIdSize);
  LOG(INFO) << "OnMaxSeqLoaded - set cur_seqs len = " << cur_seqs_.size();
  for (size_t i=0; i<section_max_seqs_.size(); ++i) {
    std::fill(cur_seqs_.begin()+i*kPerSectionIdSize,
              cur_seqs_.begin()+(i+1)*kPerSectionIdSize,
              section_max_seqs_[i]);
  }
  
  LOG(INFO) << "OnMaxSeqLoaded - set cur_seqs end";
  // << section_max_seqs_.size()*kPerSectionIdSize;
  
  state_ = kAllocInited;
}

void AllocSvrManager::OnMaxSeqSaved(bool result) {
}



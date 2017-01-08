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

#include "seqsvr/sequence_manager.h"

#include <boost/filesystem.hpp>
#include <folly/FileUtil.h>
#include <folly/Range.h>

/*
 1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
 2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：
    如果cur_seq > max_seq，将分配上限提升一个步长max_seq += step，并持久化max_seq
 3. 重启时，读出持久化的max_seq，赋值给cur_seq
 */
// folly::Singleton<SequenceManager> ;
static folly::Singleton<SequenceManager> g_sequence_manager;

std::shared_ptr<SequenceManager> SequenceManager::GetInstance() {
  return g_sequence_manager.try_get();
}

void SequenceManager::Initialize(const std::string& filepath) {
  // 是不是第一次
  bool is_first = false;
  
  // 1. 文件不存在，则创建文件
  boost::filesystem::path f(filepath);
  if (!boost::filesystem::exists(f)) {
    section_fd_ = folly::openNoInt(filepath.c_str(), O_CREAT | O_RDWR);
    PCHECK(section_fd_!=-1) << "";
    PCHECK(folly::ftruncateNoInt(section_fd_, SECTION_SLOT_MEM_SIZE) == 0) << SECTION_SLOT_MEM_SIZE;

    // section_fd_ = folly::truncateNoInt(filepath.c_str(), SECTION_SLOT_MEM_SIZE);
    is_first = true;
  } else {

    auto sz = boost::filesystem::file_size(f);
    if (sz == SECTION_SLOT_MEM_SIZE) {
      section_fd_ = folly::openNoInt(filepath.c_str(), O_RDWR);
    } else {
      // 直接退出
      LOG(FATAL) << "section' file size invalid, sz = " << sz;
    }
  }

  if (section_fd_ == -1) {
    // throw std::runtime_error("open section file error!");
    CHECK(section_fd_==-1) << "open section file error!";
  }
  
  // 2. 复制副本给section_max_seqs_
  section_max_seqs_ = new uint64_t[SECTION_SLOT_SIZE];
  CHECK(section_max_seqs_) << "Alloc section_max_seqs_ error!";

  if (!is_first) {
    // 基本上能成功，不检查
    folly::readFull(section_fd_, section_max_seqs_, SECTION_SLOT_MEM_SIZE);
    lseek(section_fd_, 0, SEEK_SET);
  } else {
    // 不设置最大值为步长
    memset(section_max_seqs_, 0, SECTION_SLOT_MEM_SIZE);
  }

  // 3. 生成seqs_
  cur_seqs_ = new uint64_t[MAX_UID_SIZE];
  CHECK(section_max_seqs_) << "Alloc cur_seqs_ error!";
  // 由section_max_seqs生成seqs值
  if (is_first) {
    // 初始化为0
    memset(cur_seqs_, 0, MAX_UID_MEM_SIZE);
  } else {
    // 将cur_seq设置为max_seq
    for (int i=0; i<SECTION_SLOT_SIZE; ++i) {
      folly::Range<uint64_t*> r(cur_seqs_+i*SECTION_SLOT_SIZE, SECTION_SIZE);
      std::fill(r.begin(), r.end(), section_max_seqs_[i]);
    }
  }

  // 回写文件
  folly::writeFull(section_fd_, section_max_seqs_, SECTION_SLOT_MEM_SIZE);
  folly::fdatasyncNoInt(section_fd_);
  lseek(section_fd_, 0, SEEK_SET);
  
  // 绑定文件映射
  section_max_seqs_mapping_ = new folly::MemoryMapping(section_fd_, 0, SECTION_SLOT_MEM_SIZE, folly::MemoryMapping::writable());
  CHECK(section_max_seqs_mapping_);
  mapping_mem_ = section_max_seqs_mapping_->asWritableRange<uint64_t>();
}

SequenceManager::~SequenceManager() {
  if (section_max_seqs_) {
    delete [] section_max_seqs_;
  }
  
  if (cur_seqs_) {
    delete [] cur_seqs_;
  }
  
  if (section_max_seqs_mapping_) {
    delete section_max_seqs_mapping_;
  }
  
  folly::closeNoInt(section_fd_);
}

uint64_t SequenceManager::GetCurrentSequence(uint32_t uid) {
#ifdef DEBUG_TEST
  DCHECK(uid<MAX_UID_SIZE);
#endif
  
  std::lock_guard<std::mutex> g(mutex_);
  return cur_seqs_[uid];
}

uint64_t SequenceManager::FetchNextSequence(uint32_t uid) {
#ifdef DEBUG_TEST
  DCHECK(uid<MAX_UID_SIZE);
#endif
  
  auto idx = uid >> SECTION_BITS_SIZE;
  
  std::lock_guard<std::mutex> g(mutex_);
  auto seq = ++cur_seqs_[uid];
  if (seq > section_max_seqs_[idx]) {
    section_max_seqs_[idx] = (seq/MAX_SEQ_STEP+1)*MAX_SEQ_STEP;
    mapping_mem_[idx] = section_max_seqs_[idx];
  }
  return seq;
}

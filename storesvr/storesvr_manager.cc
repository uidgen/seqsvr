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

#include "storesvr/storesvr_manager.h"

#include <boost/filesystem.hpp>
#include <folly/FileUtil.h>
#include <folly/Range.h>

/*
 1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
 2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：
 如果cur_seq > max_seq，将分配上限提升一个步长max_seq += step，并持久化max_seq
 3. 重启时，读出持久化的max_seq，赋值给cur_seq
 */
// folly::Singleton<StoreSvrManager> ;
static folly::Singleton<StoreSvrManager> g_storesvr_manager;

std::shared_ptr<StoreSvrManager> StoreSvrManager::GetInstance() {
  return g_storesvr_manager.try_get();
}

bool StoreSvrManager::Initialize(uint32_t set_id, const std::string& filepath) {
  set_id_ = set_id;
  
  // 是不是第一次
  bool is_first = false;
  
  // 1. 文件不存在，则创建文件
  boost::filesystem::path f(filepath);
  if (!boost::filesystem::exists(f)) {
    section_fd_ = folly::openNoInt(filepath.c_str(), O_CREAT | O_RDWR);
    PCHECK(section_fd_!=-1) << "";
    PCHECK(folly::ftruncateNoInt(section_fd_, kSectionSlotMemSize) == 0) << kSectionSlotMemSize;
    
    // section_fd_ = folly::truncateNoInt(filepath.c_str(), SECTION_SLOT_MEM_SIZE);
    is_first = true;
  } else {
    
    auto sz = boost::filesystem::file_size(f);
    if (sz == kSectionSlotMemSize) {
      section_fd_ = folly::openNoInt(filepath.c_str(), O_RDWR);
    } else {
      // 直接退出
      LOG(FATAL) << "section' file size invalid, sz = " << sz;
      folly::closeNoInt(section_fd_);

      return false;
    }
  }
  
  if (section_fd_ == -1) {
    // throw std::runtime_error("open section file error!");
    CHECK(section_fd_==-1) << "open section file error!";
    return false;
  }
  
  // 绑定文件映射
  section_max_seqs_mapping_ = new folly::MemoryMapping(section_fd_, 0, kSectionSlotMemSize, folly::MemoryMapping::writable());
  CHECK(section_max_seqs_mapping_);
  mapping_mem_ = section_max_seqs_mapping_->asWritableRange<uint64_t>();
  
  if (is_first) {
    memset(mapping_mem_.data(), 0, mapping_mem_.size());
  }
  
  inited_ = true;
  
  return true;
}

StoreSvrManager::~StoreSvrManager() {
  if (section_max_seqs_mapping_) {
    delete section_max_seqs_mapping_;
  }
  
  folly::closeNoInt(section_fd_);
}

std::string StoreSvrManager::GetSectionsData(uint32_t set_id, uint32_t alloc_id) {
  // TODO(@benqi): 映射到set_id/alloc_id
  std::string section_max_seqs((char*)mapping_mem_.data(), mapping_mem_.size()*sizeof(uint64_t));
  return section_max_seqs;
}

bool StoreSvrManager::SetSectionsData(uint32_t set_id, uint32_t alloc_id, uint32_t section_id, uint64_t max_seq) {
  // TODO(@benqi): 映射到set_id/alloc_id
  mapping_mem_[section_id] = max_seq;
  return true;
}

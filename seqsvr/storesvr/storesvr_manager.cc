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

#include <thrift/lib/cpp2/protocol/Serializer.h>

// 1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
// 2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：
//    如果cur_seq > max_seq，将分配上限提升一个步长max_seq += step，并持久化max_seq
// 3. 重启时，读出持久化的max_seq，赋值给cur_seq
//
// folly::Singleton<StoreSvrManager> ;
static folly::Singleton<StoreSvrManager> g_storesvr_manager;

std::shared_ptr<StoreSvrManager> StoreSvrManager::GetInstance() {
  return g_storesvr_manager.try_get();
}

bool StoreSvrManager::Initialize(SetID set_id, const std::string& filepath) {
  CHECK(!inited_);
  
  boost::filesystem::path f2(filepath);
  if (!boost::filesystem::exists(f2)) {
    LOG(ERROR) << "Initialize - store db's filepath not exist!! " << filepath;
    return false;
  }
  
  // set_id_ = set_id;
  auto seq_file_path = folly::sformat("{}/set_{}_{}.db",
      filepath,
      set_id.id_begin,
      set_id.size);
  
  auto section_size = CalcSetSectionSize(set_id);
  auto section_mem_size = section_size << 3;
  
  route_table_file_path_ = folly::sformat("{}/router_{}_{}.dat",
      filepath,
      set_id.id_begin,
      set_id.size);
  // filepath + "/router.dat";

  // 是不是第一次
  bool is_first = false;
  
  // 1. 文件不存在，则创建文件
  boost::filesystem::path f(seq_file_path);
  if (!boost::filesystem::exists(f)) {
    section_fd_ = folly::openNoInt(seq_file_path.c_str(), O_CREAT | O_RDWR);
    PCHECK(section_fd_!=-1) << "";
    PCHECK(folly::ftruncateNoInt(section_fd_, section_mem_size) == 0)
      << section_mem_size;
    
    // section_fd_ = folly::truncateNoInt(filepath.c_str(), SECTION_SLOT_MEM_SIZE);
    is_first = true;
  } else {
    
    auto sz = boost::filesystem::file_size(f);
    if (sz == section_mem_size) {
      section_fd_ = folly::openNoInt(seq_file_path.c_str(), O_RDWR);
    } else {
      // 直接退出
      LOG(FATAL) << "section's file size invalid, sz = " << sz;
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
  section_max_seqs_mapping_ = new folly::MemoryMapping(section_fd_, 0, section_mem_size, folly::MemoryMapping::writable());
  CHECK(section_max_seqs_mapping_);
  auto mapping_mem = section_max_seqs_mapping_->asWritableRange<uint64_t>();
  
  if (is_first) {
    memset(mapping_mem.data(), 0, mapping_mem.size()*8);
  }

  // 至此，初始化set成功
  set_ = std::make_unique<Set>(set_id, mapping_mem);

  f = boost::filesystem::path(route_table_file_path_);
  if (boost::filesystem::exists(f)) {
    std::string data;
    if (folly::readFile(route_table_file_path_.c_str(), data)) {
      auto rv = apache::thrift::SimpleJSONSerializer::deserialize(data, cache_router_);
      if (rv != data.length()) {
        LOG(ERROR) << "Initialize - router data invalid!!!!!";
        // cache_router_.Clear();
      }
    }
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

bool StoreSvrManager::GetMaxSeqsData(seqsvr::MaxSeqsData& max_seqs) {
  if (!inited_ || !set_) {
    return false;
  }
  
  // return set_->GetSectionSetData(max_seqs);
  max_seqs.set_id.id_begin = set_->GetID().id_begin;
  max_seqs.set_id.size = set_->GetID().size;
  max_seqs.max_seqs.assign(
        set_->GetMaxSeqsData().begin(),
        set_->GetMaxSeqsData().end());
  
  return true;
}

uint64_t StoreSvrManager::SetSectionMaxSeq(uint32_t id, uint64_t max_seq) {
  CHECK(inited_);
  CHECK(set_);
  
  return set_->SetMaxSeq(id, max_seq);
}

bool StoreSvrManager::SaveCacheRouter(const seqsvr::Router& router) {
  std::string data;
  apache::thrift::SimpleJSONSerializer::serialize(router, &data);
  
  if (data.empty()) {
    LOG(ERROR) << "SaveCacheRouter - cache_router_ serialize error!!!!";
    return false;
  }
  
  // data.push_back('\n');
  if (!folly::writeFile(data, route_table_file_path_.c_str())) {
    LOG(WARNING) << "SaveCacheRouter - write route_table error!!!";
    return false;
  }
  
  cache_router_ = router;
  return true;
}

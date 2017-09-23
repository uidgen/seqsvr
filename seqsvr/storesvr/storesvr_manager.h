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

#ifndef STORESVR_STORESVR_MANAGER_H_
#define STORESVR_STORESVR_MANAGER_H_

#include <folly/MemoryMapping.h>
#include <folly/Singleton.h>

#include "proto/gen-cpp2/seqsvr_types.h"
#include "base/set.h"


class StoreSvrManager {
public:
  ~StoreSvrManager();
  
  static std::shared_ptr<StoreSvrManager> GetInstance();
  
  // 通过配置文件加载
  // 参数说明
  // set_size: 整个系统里分配了多少个set
  // set_idx:属于第几个set
  // filepath: 存储路径
  bool Initialize(SetID set_id, const std::string& filepath);
  
  bool GetMaxSeqsData(seqsvr::MaxSeqsData& max_seqs);
  uint64_t SetSectionMaxSeq(uint32_t id, uint64_t max_seq);
  
  void GetCacheRouter(seqsvr::Router& router) {
    router = cache_router_;
  }
  bool SaveCacheRouter(const seqsvr::Router& router);
  
private:
  StoreSvrManager() = default;
  friend class folly::Singleton<StoreSvrManager>;
  
  bool inited_ = false;

  // std::string seq_file_path_;
  // set
  std::unique_ptr<Set> set_;

  // 使用内存映射文件文件操作section文件
  int section_fd_ {-1};
  folly::MemoryMapping* section_max_seqs_mapping_ {nullptr};
  // folly::Range<uint64_t*> mapping_mem_;
  
  // 路由表存储文件
  std::string route_table_file_path_;
  seqsvr::Router cache_router_;
};

#endif

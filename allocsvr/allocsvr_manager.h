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

#ifndef ALLOCSVR_ALLOCSVR_MANAGER_H_
#define ALLOCSVR_ALLOCSVR_MANAGER_H_

#include <mutex>

#include <folly/MemoryMapping.h>
#include <folly/Singleton.h>

#include "seqsvr/set.h"

enum AllocSvrState {
  kAllocNone = 0,
  kAllocWaitLoad,
  kAllocLoading,
  kAllocLoaded,
  kAllocError,
};

// TODO(@benqi): 单机模拟set的allocsvr和storesvr
class AllocSvrManager {
public:
  ~AllocSvrManager() = default;
  
  static std::shared_ptr<AllocSvrManager> GetInstance();
  
  void Initialize(uint32_t set_id, uint32_t alloc_id);
  
  uint64_t GetCurrentSequence(uint32_t id);
  uint64_t FetchNextSequence(uint32_t id);
  
private:
  AllocSvrManager()
    : section_max_seqs_(kSectionSlotSize),
      cur_seqs_(kMaxIDSize),
      state_(kAllocNone) {}
  
  friend class folly::Singleton<AllocSvrManager>;
  
  // bytes
  void Load(uint32_t set_id, uint32_t alloc_id);
  void Save(uint32_t set_id, uint32_t alloc_id, uint32_t section_id, uint64_t section_max_seq);

  void OnLoad(const std::string& data);
  void OnSave(bool result);

  uint32_t set_id_{1};
  uint32_t alloc_id_{1};
  
  std::vector<uint64_t> section_max_seqs_;
  std::vector<uint64_t> cur_seqs_;
  // 状态
  int state_{kAllocNone};
  
  std::mutex mutex_;
};

#endif

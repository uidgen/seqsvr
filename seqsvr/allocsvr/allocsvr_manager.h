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

#include "base/set.h"
#include "allocsvr/lease_clerk.h"
#include "proto/cc/seqsvr.pb.h"

enum AllocSvrState {
  kAllocNone = 0,
  kAllocWaitRouteTable,
  // kAllocWaitLoad,
  kAllocWaitLoadSeq,
  kAllocInited,
  kAllocError,
};

//struct IDRangeSections {
//  IDRange range;
//  std::vector<uint64_t> section_max_seqs_;
//  std::vector<uint64_t> cur_seqs_;
//};

class RouteTable;
struct SequenceWithRouterTable {
  ~SequenceWithRouterTable() {
    if (router) delete router;
    router = nullptr;
  }
  
  zproto::Router* Release() {
    zproto::Router* r = router;
    router = nullptr;
    return r;
  }
  
  uint64_t seq{0};
  zproto::Router* router{nullptr};
};

// TODO(@benqi): 单机模拟set的allocsvr和storesvr
// 未加载成功重试加载
class AllocSvrManager : public LeaseClerk::LeaseCallback {
public:
  ~AllocSvrManager() = default;
  
  static std::shared_ptr<AllocSvrManager> GetInstance();
  
  //////////////////////////////////////////////////////////////////////////////////////////////
  void Initialize(nebula::TimerManager* timer_manager, const std::string& set_name, const std::string& alloc_name);
  void Destroy();
  
  bool GetCurrentSequence(uint32_t id, uint32_t client_version, SequenceWithRouterTable& o);
  bool FetchNextSequence(uint32_t id, uint32_t client_version, SequenceWithRouterTable& o);
  
  //////////////////////////////////////////////////////////////////////////////////////////////
  // 租约生效
  virtual void OnLeaseValid(RouteTable& table);
  // 路由表更新
  virtual void OnLeaseUpdated(RouteTable& table);
  // 租约失效
  virtual void OnLeaseInvalid();

private:
  AllocSvrManager()
    : section_max_seqs_(kSectionSlotSize),
      cur_seqs_(kMaxIDSize),
      state_(kAllocNone) {}
  
  friend class folly::Singleton<AllocSvrManager>;
  
  // bytes
  void LoadMaxSeq();
  void SaveMaxSeq(uint32_t section_id, uint64_t section_max_seq);

  void OnMaxSeqLoaded(const std::string& data);
  void OnMaxSeqSaved(bool result);

  //////////////////////////////////////////////////////////////////////////////////////////////
  std::string set_name_;
  std::string alloc_name_;
  
  std::vector<uint64_t> section_max_seqs_;
  std::vector<uint64_t> cur_seqs_;

  // 状态
  int state_{kAllocNone};
    
  std::mutex mutex_;
  std::unique_ptr<LeaseClerk> lease_;
  
  AllocEntry* cache_alloc_entry_ {nullptr};
  RouteTable table_;
  // RouteSearchTable route_search_table_;
};

#endif

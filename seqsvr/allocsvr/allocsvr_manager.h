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

// #include <folly/MemoryMapping.h>
#include <folly/Singleton.h>

#include "base/set.h"
#include "allocsvr/client_manager.h"
#include "allocsvr/lease_clerk.h"

enum AllocSvrState {
  kAllocNone = 0,
  kAllocWaitRouteTable,
  // kAllocWaitLoad,
  kAllocWaitLoadSeq,
  kAllocInited,
  kAllocError,
};

class RouteTable;

// allocsvr流程：
// 1. 从存储系统里取出路由表
// 2. 通过本机Addr信息到路由表里获取当前服务号段列表
// 3. 启动租约服务
// 4. 从存储系统里取出max_seqs集合
// 5. 开始对客户端提供服务
//
// TODO(@benqi): 单机模拟set的allocsvr和storesvr
// 未加载成功重试加载
class AllocSvrManager : public LeaseClerk::LeaseCallback {
public:
  ~AllocSvrManager() = default;
  
  static std::shared_ptr<AllocSvrManager> GetInstance();
  
  ////////////////////////////////////////////////////////////////////////////
  // alloc_addr: allocsvr的本机地址信息
  // store_addr_list: storesvr地址列表
  void Initialize(nebula::TimerManager* timer_manager,
        const IpAddrInfo& alloc_addr,
        const IpAddrInfoList& store_addr_list);
  
  void Destroy();
  
  // 客户端接口
  bool GetCurrentSequence(uint32_t id, uint32_t client_version, seqsvr::Sequence& o);
  bool FetchNextSequence(uint32_t id, uint32_t client_version, seqsvr::Sequence& o);
  
  ////////////////////////////////////////////////////////////////////////////
  // 租约回调接口实现
  // 租约生效
  void OnLeaseValid(seqsvr::Router& router) override;
  // 路由表更新
  void OnLeaseUpdated(seqsvr::Router& router) override;
  // 租约失效
  void OnLeaseInvalid() override;

private:
  AllocSvrManager()
    : state_(kAllocNone) {}
  
  friend class folly::Singleton<AllocSvrManager>;
  
  // bytes
  void LoadMaxSeq();
  void SaveMaxSeq(uint32_t id, uint64_t section_max_seq);

  void OnMaxSeqLoaded(seqsvr::MaxSeqsData& data);
  void OnMaxSeqSaved(bool result);

  ////////////////////////////////////////////////////////////////////////////
  // alloc_addr: allocsvr的本机地址信息
  // store_addr_list: storesvr地址列表
  IpAddrInfo alloc_addr_;
  IpAddrInfoList store_addr_list_;
  
  // store客户端管理器，存储storesvr
  std::unique_ptr<ClientManager> client_;
  
  // 状态
  int state_{kAllocNone};
  
  // 路由表
  seqsvr::Router table_;
  // 保护Router
  // TODO(@benqi):
  // 1. 使用读写锁，
  // 2. 对router的操作封装成wrapper，预防忘记线程保护
  std::mutex table_lock_;
  
  // 本机路由节点
  seqsvr::RouterNode* cache_my_node_ {nullptr};

  // 路由表租约
  std::unique_ptr<LeaseClerk> lease_;

  // 同个号段内的用户共享一个max_seq
  // 号段对应max_seq
  std::vector<int64_t> section_max_seqs_;
  // 用户当前cur_seqs_
  std::vector<int64_t> cur_seqs_;
};

#endif

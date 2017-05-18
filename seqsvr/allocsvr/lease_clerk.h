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

#ifndef	ALLOCSVR_LEASE_CLERK_H_
#define	ALLOCSVR_LEASE_CLERK_H_

#include <mutex>
#include <folly/io/async/EventBase.h>

#include "nebula/base/timer_manager.h"

#include "proto/cc/seqsvr.pb.h"
#include "base/router_table.h"

#define LEASE_TIMEOUT 5000  // 租约时间5s
#define SYNC_LEASE_TIMEOUT 4000  // 定时同步路由4s
#define CHECK_LEASE_TIMEOUT 100  // 租约时间5s

// 为了避免失联AllocSvr提供错误的服务，返回脏数据，AllocSvr需要跟StoreSvr保持租约。这个租约机制由以下两个条件组成：
// 租约失效：AllocSvr N秒内无法从StoreSvr读取加载配置时，AllocSvr停止服务
// 租约生效：AllocSvr读取到新的加载配置后，立即卸载需要卸载的号段，需要加载的新号段等待N秒后提供服务
class LeaseClerk {
public:
  enum State {
    STATE_NONE = 0,
    STATE_VALID,
    STATE_INVALID,
    STATE_ERROR,
  };
  
  class LeaseCallback {
  public:
    virtual ~LeaseCallback() = default;
    
    // 租约生效
    virtual void OnLeaseValid(RouteTable& table) = 0;
    // 路由表更新
    virtual void OnLeaseUpdated(RouteTable& table) = 0;
    // 租约失效
    virtual void OnLeaseInvalid() = 0;
  };
  
  LeaseClerk(nebula::TimerManager* timer_manager, LeaseCallback* cb)
    : timer_manager_(timer_manager),
      cb_(cb) {}
  
  ~LeaseClerk() = default;
  
  // 开始租约服务
  void Start();
  // 停止租约服务
  void Stop();
  
private:
  // 开始租约服务
  void RequestLease();
  // 定时检查租约是否失效
  void CheckLeaseValid();

  // 111
  void OnHandleLease(const zproto::Router& router);
  
  std::mutex lock_;
  bool is_valid_;
  // state_{STATE_NONE};  // 生效
  
  // 路由表
  // RouteTable table_;
  
  uint32_t version_{0};
  int64_t lease_invalid_time_{0};   // 租约失效时间
  
  nebula::TimerManager* timer_manager_{nullptr};
  LeaseCallback* cb_{nullptr};
};

#endif // ALLOCSVR_ALLOC_SERVER_H_

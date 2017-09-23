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

// #include "proto/cc/seqsvr.pb.h"
#include "proto/gen-cpp2/seqsvr_types.h"
#include "base/router_table.h"

/*
   所谓租约(leases)，其实就是一个合同，即服务端给予客户端在一定期限内可以控制修改操作的权力。
   如果服务端要修改数据，首先要征求拥有这块数据的租约的客户端的同意，之后才可以修改。
   客户端从服务端读取数据时往往就同时获取租约，在租约期限内，如果没有收到服务端的修改请求，
   就可以保证当前缓存中的内容就是最新的。如果在租约期限内收到了修改数据的请求并且同意了，
   就需要清空缓存。在租约过期以后，客户端如果还要从缓存读取数据，就必须重新获取租约，
   我们称这个操作为“续约”{!引用自<<租约机制简介>>}。
 */

// ## 租约
// 为了避免失联AllocSvr提供错误的服务，返回脏数据，AllocSvr需要跟StoreSvr保持租约。
// 这个租约机制由以下两个条件组成：
// > - 租约失效：AllocSvr N秒内无法从StoreSvr读取加载配置时，AllocSvr停止服务
//   - 租约生效：AllocSvr读取到新的加载配置后，立即卸载需要卸载的号段，
//     需要加载的新号段等待N秒后提供服务
//
// 这两个条件保证了切换时，新AllocSvr肯定在旧AllocSvr下线后才开始提供服务。
// 但这种租约机制也会造成切换的号段存在小段时间的不可服务，
// 不过由于微信后台逻辑层存在重试机制及异步重试队列，
// 小段时间的不可服务是用户无感知的，而且出现租约失效、切换是小概率事件，整体上是可以接受的。
//

#define LEASE_TIMEOUT 5000        // 租约时间5s
#define SYNC_LEASE_TIMEOUT 4000   // 定时同步路由4s
#define CHECK_LEASE_TIMEOUT 1000  //

class ClientManager;

// 租约管理，只维护租约状态，并在租约失效和生效时生成回调
class LeaseClerk {
public:
  enum State {
    STATE_NONE = 0,
    STATE_VALID,
    STATE_INVALID,
    STATE_ERROR,
  };
  
  // 租约事件触发
  class LeaseCallback {
  public:
    virtual ~LeaseCallback() = default;
    
    // 租约生效
    virtual void OnLeaseValid(seqsvr::Router& router) = 0;
    // 路由表更新
    virtual void OnLeaseUpdated(seqsvr::Router& router) = 0;
    // 租约失效
    virtual void OnLeaseInvalid() = 0;
  };
  
  LeaseClerk(nebula::TimerManager* timer_manager,
             ClientManager* client,
             LeaseCallback* cb);
  ~LeaseClerk() = default;
  
  // 开始租约服务
  void Start();
  // 停止租约服务
  void Stop();
  
private:
  // 开始租约服务
  int RequestLease();
  // 定时检查租约是否失效
  int CheckLeaseValid();

  // 111
  int OnHandleLease(seqsvr::Router& router);
  
  std::mutex lock_;
  // bool is_valid_{false};
  int state_{STATE_NONE};  // 生效
  
  // 路由表
  // RouteTable table_;
  uint32_t version_{0};
  int64_t lease_invalid_time_{0};   // 租约失效时间
  
  nebula::TimerManager* timer_manager_{nullptr};
  ClientManager* client_{nullptr};
  LeaseCallback* cb_{nullptr};
};

#endif // ALLOCSVR_ALLOC_SERVER_H_

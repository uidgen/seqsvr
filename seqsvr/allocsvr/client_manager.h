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

#ifndef	ALLOCSVR_CLIENT_MANAGER_H_
#define	ALLOCSVR_CLIENT_MANAGER_H_

#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>

#include "nebula/net/base/ip_addr_info.h"

#include "proto/gen-cpp2/seqsvr_types.h"
#include "proto/gen-cpp2/StoreService.h"


/*
 ## NWR 模型
 > NWR是一种在分布式存储系统中用于控制一致性级别的策略，应用于 Amazon Dynamo。
   NWR 模型将 CAP 的选择权交给了用户，由用户自己选择 CAP 中的哪两个。
   其中，N 代表 N 个备份，W 代表至少写 W 份才认为成功，R 代表至少要读 R 份才认为成功。
 
 - 如果 W+R>N ，是可以保证强一致性的。
   因为 W+R > N， 所以 R > N-W，什么意思呢？
   就是读取的份数必须要大于未成功写的份数，这样至少能读到一份最新值。
 - 如果 W+R<=N，则能够保证最终一致性。
 - 如果我们要高可写的环境，我们可以配置 W=1 R=N。
   这个时候只要写任何节点成功就认为成功，但是读的时候必须从所有的节点都读出数据。
 - 如果我们要求读的高效率，我们可以配置 W=N R=1。
   这个时候任何一个节点读成功就认为成功，但是写的时候必须写所有三个节点成功才认为成功。
 */

// 分布式Quorum机制,NWR策略读写模型
// https://my.oschina.net/manmao/blog/618344

// TODO(@benqi): wrapper客户端，使用客户端线程池
// 可能需要把nebula/net集成进来
class ClientUtil {
public:
  ClientUtil(folly::EventBase* evb, const IpAddrInfo& addr)
    : evb_(evb),
      addr_(addr) {}
  
  void LoadMaxSeqsData(seqsvr::MaxSeqsData& _return) {
    CheckOrMakeClient();
    if (client_) {
      client_->sync_LoadMaxSeqsData(_return);
    }
  }
  
  int64_t SaveMaxSeq(int32_t id, int64_t max_seq);
  
  bool SaveRouteTable(const ::seqsvr::Router& router) {
    CheckOrMakeClient();
    
    if (client_) {
      return client_->sync_SaveRouteTable(router);
    }
    return false;
  }
  
  int LoadRouteTable( ::seqsvr::Router& _return) {
    CheckOrMakeClient();
    
    if (client_) {
      try {
        client_->sync_LoadRouteTable(_return);
        return 0;
      } catch(...) {
        client_.reset();
      }
    }
    
    return -1;
  }

private:
  void CheckOrMakeClient() {
    if (!client_) {
      // 创建客户端
      client_ = std::make_unique<seqsvr::StoreServiceAsyncClient>(
         apache::thrift::HeaderClientChannel::newChannel(
             apache::thrift::async::TAsyncSocket::newSocket(
               evb_, {addr_.addr, addr_.port})));
    }
  }
  
  folly::EventBase* evb_;
  IpAddrInfo addr_;
  std::unique_ptr<seqsvr::StoreServiceAsyncClient> client_;
  int state_; // invalid, ready, error...
};

// TODO(@benqi): NWR操作
class ClientManager {
public:
  ClientManager(folly::EventBase* evb, const IpAddrInfoList& addrs) {
    for (const auto& addr : addrs) {
      client_addr_list_.emplace_back(evb, addr);
    }
  }

  void LoadMaxSeqsData(seqsvr::MaxSeqsData& _return) {
    client_addr_list_[0].LoadMaxSeqsData(_return);
  }
  
  int64_t SaveMaxSeq(int32_t id, int64_t max_seq) {
    return client_addr_list_[0].SaveMaxSeq(id, max_seq);
  }
  
  bool SaveRouteTable(const  ::seqsvr::Router& router) {
    return client_addr_list_[0].SaveRouteTable(router);
  }

  // R=N
  int LoadRouteTable( ::seqsvr::Router& _return) {
    return client_addr_list_[0].LoadRouteTable(_return);
  }

private:
  std::vector<ClientUtil> client_addr_list_;
};

#endif

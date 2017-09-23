/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdio.h>

#include <folly/init/Init.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>

#include "proto/gen-cpp2/seqsvr_types.h"
#include "proto/gen-cpp2/StoreService.h"

#include <thrift/lib/cpp2/protocol/Serializer.h>

using namespace std;
using namespace folly;
using namespace apache::thrift;
using namespace seqsvr;
/*
stdcxx::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
CalculatorClient client(protocol);

try {
  transport->open();
*/
  
int main(int argc, char** argv) {
  folly::init(&argc, &argv, true);
  
  EventBase eb;
  auto client = std::make_unique<seqsvr::StoreServiceAsyncClient>(
        HeaderClientChannel::newChannel(
        async::TAsyncSocket::newSocket(
        &eb, {"127.0.0.1", 9090})));
  
  try {
    /*
     struct NodeAddrInfo {
     1: string ip;
     2: i32 port;
     }
     
     // 大部分情况，一个AllocSvr里的号段大部分是连续的
     // 为了减少网络传输量，将连续的号段使用SectionRange进行压缩
     // 例如，1～10个号段，
     // id_begin: 1, size: 10
     struct SectionRange {
     1: i32 id_begin;  // 号段起始地址
     2: i32 size;      // 有多少个号段
     }
     
     // 路由Alloc节点
     struct RouterNode {
     1: NodeAddrInfo node_addr;              // 节点地址
     2: list<SectionRange> section_ranges;   // 本节点管理的所有号段
     }
     
     //////////////////////////////////////////////////////////////////////////////
     // 客户端眼里的路由表：
     struct Router {
     1: i32 version;
     2: list<RouterNode> node_list; // 整个集群所有allocsvr节点
     }

     */
    
    RouterNode node;
    node.node_addr.ip = "127.0.0.1";
    node.node_addr.port = 10000;

    SectionRange range;

    range.id_begin = 0;
    range.size = 1<<19;
    node.section_ranges.push_back(range);

    range.id_begin = 1<<19;
    range.size = 1<<19;
    node.section_ranges.push_back(range);

    Router table;
    table.version = 1;
    table.node_list.push_back(node);
    
    // auto s = makeTestStruct();
    std::string data;
    apache::thrift::SimpleJSONSerializer::serialize(table, &data);
    // auto data = q.move();

    // apache::thrift::JSONSerializer::serialize(table, &data);
    printf("update route table: %s\n", data.c_str());

    
    auto rv = client->sync_SaveRouteTable(table);
//    Request req;
//    req.str = "hello";
//    Response rsp;
//    client->sync_helloWorld( rsp, req);
    printf("update route table: %d\n", rv);
  } catch (TException &tx) {
    printf("ERROR: %s\n", tx.what());
  }
}

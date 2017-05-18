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

#include "mediatesvr/mediate_handler.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"
#include "base/message_handler_util.h"
#include "base/router_table.h"

REGISTER_HTTP_HANDLER(UpdateRouteTable, "/mediate/router/update", UpdateRouteTable);

// 初始化路由表
void UpdateRouteTable(const proxygen::HTTPMessage& headers, folly::IOBufQueue* body, proxygen::ResponseBuilder* r) {
  zproto::UpdateRouteTableReq update_route_table_req;
  
  RouteTable table;
  RouteTable::MakeTestRouteTable(table);
  table.SerializeToRouter(update_route_table_req.mutable_router());
  
  ZRpcClientCall<zproto::UpdateRouteTableRsp>("store_client",
                                             MakeRpcRequest(update_route_table_req),
                                             [&] (std::shared_ptr<ApiRpcOk<zproto::UpdateRouteTableRsp>> rpc_ok,
                                                  ProtoRpcResponsePtr rpc_error) -> int {
                                               if (rpc_error) {
                                                 LOG(ERROR) << "LoadMaxSeqsDataReq - rpc_error: " << rpc_error->ToString();
                                                 // OnLoad("");
                                               } else {
                                                 LOG(INFO) << "LoadMaxSeqsDataReq - load_max_seqs_data_rsp: " << rpc_ok->ToString();
                                                 // OnLoad((*load_max_seqs_data_rsp)->max_seqs());
                                               }
                                               return 0;
                                             });

  std::string json("{\"status\": \"OK\"}\n");
  std::unique_ptr<folly::IOBuf> json_string = folly::IOBuf::copyBuffer(json.c_str(), json.length());
  r->header("Content-Type", "application/json;charset=utf-8").body(std::move(json_string));
}

// AllocSvr加入和移除集群
void AddAllocSvr(const proxygen::HTTPMessage& headers, folly::IOBufQueue*, proxygen::ResponseBuilder* r) {
  
}

void RemoveAllocSvr(const proxygen::HTTPMessage& headers, folly::IOBufQueue*, proxygen::ResponseBuilder* r) {
  
}

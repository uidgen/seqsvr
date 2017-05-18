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

// TODO(@benqi): 使用zrpc-code-gen代码生成工具自动生成

#include "storesvr/zrpc_store_dispatcher.h"

#include "storesvr/store_service_impl.h"
#include "nebula/net/rpc/zrpc_service_util.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"


// #include "nebula/net/zproto/api_message_box.h"

static ZRpcStoreDispatcher g_rpc_store_dispatcher;

ZRpcStoreDispatcher::ZRpcStoreDispatcher() {
  ZRpcUtil::Register("zproto.LoadMaxSeqsDataReq", ZRpcStoreDispatcher::LoadMaxSeqsData);
  ZRpcUtil::Register("zproto.SaveMaxSeqReq", ZRpcStoreDispatcher::SaveMaxSeq);

  ZRpcUtil::Register("zproto.UpdateRouteTableReq", ZRpcStoreDispatcher::UpdateRouteTable);
  ZRpcUtil::Register("zproto.GetRouteTableReq", ZRpcStoreDispatcher::GetRouteTable);
}

ProtoRpcResponsePtr ZRpcStoreDispatcher::LoadMaxSeqsData(RpcRequestPtr request) {
  CAST_RPC_REQUEST(LoadMaxSeqsDataReq, load_max_seqs_data_req);
  LOG(INFO) << "LoadMaxSeqsData - [" << request->ToString()
            << "], " << load_max_seqs_data_req.Utf8DebugString();
  
  zproto::LoadMaxSeqsDataRsp load_max_seqs_data_rsp;
  
  StoreServiceImpl service_impl;
  service_impl.LoadMaxSeqsData(load_max_seqs_data_req, &load_max_seqs_data_rsp);
  
  return MakeRpcOK(load_max_seqs_data_rsp);
}

ProtoRpcResponsePtr ZRpcStoreDispatcher::SaveMaxSeq(RpcRequestPtr request) {
  CAST_RPC_REQUEST(SaveMaxSeqReq, save_max_seq_req);
  LOG(INFO) << "LoadMaxSeqsData - [" << request->ToString()
            << "], " << save_max_seq_req.Utf8DebugString();
  
  zproto::SaveMaxSeqRsp save_max_seq_rsp;
  
  StoreServiceImpl service_impl;
  service_impl.SaveMaxSeq(save_max_seq_req, &save_max_seq_rsp);
  
  return MakeRpcOK(save_max_seq_rsp);
}

ProtoRpcResponsePtr ZRpcStoreDispatcher::UpdateRouteTable(RpcRequestPtr request) {
  CAST_RPC_REQUEST(UpdateRouteTableReq, update_route_table_req);
  LOG(INFO) << "UpdateRouteTable - [" << request->ToString()
            << "], " << update_route_table_req.Utf8DebugString();
  
  zproto::UpdateRouteTableRsp update_route_table_rsp;
  
  StoreServiceImpl service_impl;
  service_impl.UpdateRouteTable(update_route_table_req, &update_route_table_rsp);
  
  return MakeRpcOK(update_route_table_rsp);
}

ProtoRpcResponsePtr ZRpcStoreDispatcher::GetRouteTable(RpcRequestPtr request) {
  CAST_RPC_REQUEST(GetRouteTableReq, get_route_table_req);
  LOG(INFO) << "GetRouteTable - [" << request->ToString()
            << "], " << get_route_table_req.Utf8DebugString();
  
  zproto::GetRouteTableRsp get_route_table_rsp;
  
  StoreServiceImpl service_impl;
  service_impl.GetRouteTable(get_route_table_req, &get_route_table_rsp);
  
  return MakeRpcOK(get_route_table_rsp);
}

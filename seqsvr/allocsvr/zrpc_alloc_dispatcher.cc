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

#include "allocsvr/zrpc_alloc_dispatcher.h"

#include "allocsvr/alloc_service_impl.h"
#include "nebula/net/rpc/zrpc_service_util.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"


// #include "nebula/net/zproto/api_message_box.h"

static ZRpcAllocDispatcher g_rpc_alloc_dispatcher;

ZRpcAllocDispatcher::ZRpcAllocDispatcher() {
  ZRpcUtil::Register("zproto.FetchNextSequenceReq", ZRpcAllocDispatcher::FetchNextSequence);
  ZRpcUtil::Register("zproto.GetCurrentSequenceReq", ZRpcAllocDispatcher::GetCurrentSequence);
  ZRpcUtil::Register("zproto.FetchNextSequenceListReq", ZRpcAllocDispatcher::FetchNextSequenceList);
  ZRpcUtil::Register("zproto.GetCurrentSequenceListReq", ZRpcAllocDispatcher::GetCurrentSequenceList);
}

ProtoRpcResponsePtr ZRpcAllocDispatcher::FetchNextSequence(RpcRequestPtr request) {
  CAST_RPC_REQUEST(FetchNextSequenceReq, fetch_next_sequece_req);
  LOG(INFO) << "DoFetchNextSequenceReq - [" << request->ToString()
              << "], " << fetch_next_sequece_req.Utf8DebugString();
  
  zproto::SequenceRsp sequence_rsp;
  
  AllocServiceImpl service_impl;
  service_impl.FetchNextSequence(fetch_next_sequece_req, &sequence_rsp);
  
  return MakeRpcOK(sequence_rsp);
}

ProtoRpcResponsePtr ZRpcAllocDispatcher::GetCurrentSequence(RpcRequestPtr request) {
  CAST_RPC_REQUEST(GetCurrentSequenceReq, get_current_sequece_req);
  LOG(INFO) << "DoGetCurrentSequenceReq - [" << request->ToString() << "], "
              << get_current_sequece_req.Utf8DebugString();
  
  zproto::SequenceRsp sequence_rsp;
  
  AllocServiceImpl service_impl;
  service_impl.GetCurrentSequence(get_current_sequece_req, &sequence_rsp);
  
  return MakeRpcOK(sequence_rsp);
}

ProtoRpcResponsePtr ZRpcAllocDispatcher::FetchNextSequenceList(RpcRequestPtr request) {
  CAST_RPC_REQUEST(FetchNextSequenceListReq, fetch_next_sequece_list_req);
  LOG(INFO) << "FetchNextSequenceList - [" << request->ToString() << "], "
            << fetch_next_sequece_list_req.Utf8DebugString();
  
  zproto::SequenceListRsp sequence_list_rsp;
  
  AllocServiceImpl service_impl;
  service_impl.FetchNextSequenceList(fetch_next_sequece_list_req, &sequence_list_rsp);
  
  return MakeRpcOK(sequence_list_rsp);
}

ProtoRpcResponsePtr ZRpcAllocDispatcher::GetCurrentSequenceList(RpcRequestPtr request) {
  CAST_RPC_REQUEST(GetCurrentSequenceListReq, get_current_sequece_list_req);
  LOG(INFO) << "GetCurrentSequenceList - [" << request->ToString() << "], "
            << get_current_sequece_list_req.Utf8DebugString();
  
  zproto::SequenceListRsp sequence_list_rsp;
  
  AllocServiceImpl service_impl;
  service_impl.GetCurrentSequenceList(get_current_sequece_list_req, &sequence_list_rsp);
  
  return MakeRpcOK(sequence_list_rsp);
}

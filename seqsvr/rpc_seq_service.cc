/*
 *  Copyright (c) 2016, https://github.com/zhatalk
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

#include "seqsvr/rpc_seq_service.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"

static std::map<uint32_t, uint64_t> g_sequece_map;

ProtoRpcResponsePtr DoFetchNextSequenceReq(RpcRequestPtr request) {
  CAST_RPC_REQUEST(FetchNextSequenceReq, fetch_next_sequece_req);
  
  LOG(INFO) << "DoFetchNextSequenceReq - [" << request->ToString() << "], " << fetch_next_sequece_req.Utf8DebugString();

  zproto::SequenceRsp sequence_rsp;
  auto it = g_sequece_map.find(fetch_next_sequece_req.user_id());
  if (it!=g_sequece_map.end()) {
    sequence_rsp.set_sequence(it->second + 1);
    g_sequece_map[fetch_next_sequece_req.user_id()] = it->second + 1;
  } else {
    sequence_rsp.set_sequence(1);
    g_sequece_map[fetch_next_sequece_req.user_id()] = 1;
  }
  
  return MakeRpcOK(sequence_rsp);
}

ProtoRpcResponsePtr DoGetCurrentSequenceReq(RpcRequestPtr request) {
  CAST_RPC_REQUEST(GetCurrentSequenceReq, get_current_sequece_req);
  
  LOG(INFO) << "DoGetCurrentSequenceReq - [" << request->ToString() << "], " << get_current_sequece_req.Utf8DebugString();

  zproto::SequenceRsp sequence_rsp;
  auto it = g_sequece_map.find(get_current_sequece_req.user_id());
  if (it!=g_sequece_map.end()) {
    sequence_rsp.set_sequence(it->second);
    // g_sequece_map[fetch_next_sequece_req.user_id()] = it->second + 1;
  } else {
    sequence_rsp.set_sequence(0);
  }
  
  return MakeRpcOK(sequence_rsp);
}

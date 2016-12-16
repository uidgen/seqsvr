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

// TODO(@benqi): 使用zrpc-code-gen代码生成工具自动生成服务框架代码

#include "seqsvr/seq_service_impl.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"

static std::map<uint32_t, uint64_t> g_sequece_map;

int SeqServiceImpl::FetchNextSequence(const zproto::FetchNextSequenceReq& request, zproto::SequenceRsp* response) {
  auto it = g_sequece_map.find(request.user_id());
  if (it!=g_sequece_map.end()) {
    response->set_sequence(it->second + 1);
    g_sequece_map[request.user_id()] = it->second + 1;
  } else {
    response->set_sequence(1);
    g_sequece_map[request.user_id()] = 1;
  }
  
  return 0;
}

int SeqServiceImpl::GetCurrentSequence(const zproto::GetCurrentSequenceReq& request, zproto::SequenceRsp* response) {
  auto it = g_sequece_map.find(request.user_id());
  if (it!=g_sequece_map.end()) {
    response->set_sequence(it->second);
    // g_sequece_map[fetch_next_sequece_req.user_id()] = it->second + 1;
  } else {
    response->set_sequence(0);
  }
  
  return 0;
}


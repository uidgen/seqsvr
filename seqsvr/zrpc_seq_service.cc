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

#include "seqsvr/zrpc_seq_service.h"

#include "nebula/base/logger/glog_util.h"


int ZRpcSeqService::FetchNextSequence(const zproto::FetchNextSequenceReq& request, zproto::SequenceRsp* response) {
  LOG(ERROR) << "FetchNextSequence - FetchNextSequence unimplemented";
  return -1;
}

int ZRpcSeqService::GetCurrentSequence(const zproto::GetCurrentSequenceReq& request, zproto::SequenceRsp* response) {
  LOG(ERROR) << "GetCurrentSequence - GetCurrentSequence unimplemented";
  return -1;
}

int ZRpcSeqService::FetchNextSequenceList(const zproto::FetchNextSequenceListReq& request, zproto::SequenceListRsp* response) {
  LOG(ERROR) << "FetchNextSequenceList - FetchNextSequenceList unimplemented";
  return -1;
}

int ZRpcSeqService::GetCurrentSequenceList(const zproto::GetCurrentSequenceListReq& request, zproto::SequenceListRsp* response) {
  LOG(ERROR) << "GetCurrentSequenceList - GetCurrentSequenceList unimplemented";
  return -1;
}

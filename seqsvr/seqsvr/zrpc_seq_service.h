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

#ifndef	SEQSVR_ZRPC_SEQ_SERVICE_H_
#define	SEQSVR_ZRPC_SEQ_SERVICE_H_

#include "proto/cc/seqsvr.pb.h"

class ZRpcSeqService {
public:
  virtual ~ZRpcSeqService() = default;
  
  virtual int FetchNextSequence(const zproto::FetchNextSequenceReq& request, zproto::SequenceRsp* response);
  virtual int GetCurrentSequence(const zproto::GetCurrentSequenceReq& request, zproto::SequenceRsp* response);
  virtual int FetchNextSequenceList(const zproto::FetchNextSequenceListReq& request, zproto::SequenceListRsp* response);
  virtual int GetCurrentSequenceList(const zproto::GetCurrentSequenceListReq& request, zproto::SequenceListRsp* response);
};

#endif


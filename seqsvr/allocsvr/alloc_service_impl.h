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

#ifndef	ALLOCSVR_ALLOC_SERVICE_IMPL_H_
#define	ALLOCSVR_ALLOC_SERVICE_IMPL_H_

#include "allocsvr/zrpc_alloc_service.h"

class AllocServiceImpl : public ZRpcAllocService {
public:
  virtual ~AllocServiceImpl() = default;
  
  int FetchNextSequence(const zproto::FetchNextSequenceReq& request, zproto::SequenceRsp* response) override;
  int GetCurrentSequence(const zproto::GetCurrentSequenceReq& request, zproto::SequenceRsp* response) override;
  int FetchNextSequenceList(const zproto::FetchNextSequenceListReq& request, zproto::SequenceListRsp* response) override;
  int GetCurrentSequenceList(const zproto::GetCurrentSequenceListReq& request, zproto::SequenceListRsp* response) override;
};

#endif


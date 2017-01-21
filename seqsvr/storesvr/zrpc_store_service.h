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

#ifndef	STORESVR_ZRPC_STORE_SERVICE_H_
#define	STORESVR_ZRPC_STORE_SERVICE_H_

#include "proto/cc/seqsvr.pb.h"

class ZRpcStoreService {
public:
  virtual ~ZRpcStoreService() = default;
  
  virtual int LoadMaxSeqsData(const zproto::LoadMaxSeqsDataReq& request, zproto::LoadMaxSeqsDataRsp* response);
  virtual int SaveMaxSeq(const zproto::SaveMaxSeqReq& request, zproto::SaveMaxSeqRsp* response);
  
  virtual int UpdateRouteTable(const zproto::UpdateRouteTableReq& request, zproto::UpdateRouteTableRsp* response);
  virtual int GetRouteTable(const zproto::GetRouteTableReq& request, zproto::GetRouteTableRsp* response);
};

#endif


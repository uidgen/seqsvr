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

#include "storesvr/zrpc_store_service.h"

#include "nebula/base/logger/glog_util.h"

int ZRpcStoreService::LoadMaxSeqsData(const zproto::LoadMaxSeqsDataReq& request, zproto::LoadMaxSeqsDataRsp* response) {
  LOG(ERROR) << "LoadMaxSeqsData - LoadMaxSeqsData unimplemented";
  return -1;
}

int ZRpcStoreService::SaveMaxSeq(const zproto::SaveMaxSeqReq& request, zproto::SaveMaxSeqRsp* response) {
  LOG(ERROR) << "SaveMaxSeq - SaveMaxSeq unimplemented";
  return -1;
}

int ZRpcStoreService::UpdateRouteTable(const zproto::UpdateRouteTableReq& request, zproto::UpdateRouteTableRsp* response) {
  LOG(ERROR) << "UpdateRouteTable - SaveMaxSeq unimplemented";
  return -1;
}

int ZRpcStoreService::GetRouteTable(const zproto::GetRouteTableReq& request, zproto::GetRouteTableRsp* response) {
  LOG(ERROR) << "GetRouteTable - SaveMaxSeq unimplemented";
  return -1;
}

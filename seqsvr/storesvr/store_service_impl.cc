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

// TODO(@benqi): 使用zrpc-code-gen代码生成工具自动生成服务框架代码

#include "storesvr/store_service_impl.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"

#include "storesvr/storesvr_manager.h"

int StoreServiceImpl::LoadMaxSeqsData(const zproto::LoadMaxSeqsDataReq& request, zproto::LoadMaxSeqsDataRsp* response) {
  auto max_seqs = response->mutable_max_seqs();
  StoreSvrManager::GetInstance()->GetSectionsData(max_seqs);
  return 0;
}

int StoreServiceImpl::SaveMaxSeq(const zproto::SaveMaxSeqReq& request, zproto::SaveMaxSeqRsp* response) {
  uint64_t last_max_seq = StoreSvrManager::GetInstance()->SetSectionsData(request.section_id(),
                                                                          request.max_seq());
  response->set_last_max_seq(last_max_seq);
  return 0;
}

int StoreServiceImpl::UpdateRouteTable(const zproto::UpdateRouteTableReq& request, zproto::UpdateRouteTableRsp* response) {
  StoreSvrManager::GetInstance()->SaveCacheRouter(request.router());
  return 0;
}

int StoreServiceImpl::GetRouteTable(const zproto::GetRouteTableReq& request, zproto::GetRouteTableRsp* response) {
  response->mutable_router()->CopyFrom(StoreSvrManager::GetInstance()->GetCacheRouter());
  return 0;
}


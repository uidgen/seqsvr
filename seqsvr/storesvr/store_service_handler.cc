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

#include "storesvr/store_service_handler.h"

#include "base/thrift_util.h"
#include "storesvr/storesvr_manager.h"

void StoreServiceHandler::LoadMaxSeqsData(seqsvr::MaxSeqsData& _return) {
  auto instance = StoreSvrManager::GetInstance();
  instance->GetMaxSeqsData(_return);
  
  LOG(INFO) << "LoadMaxSeqsData - _return: " << convertToString(_return);
}

int64_t StoreServiceHandler::SaveMaxSeq(int32_t id, int64_t max_seq) {
  LOG(INFO) << "SaveMaxSeq - id: " << id << ", max_seq: " << max_seq;
  auto instance = StoreSvrManager::GetInstance();
  return instance->SetSectionMaxSeq(id, max_seq);
}

void StoreServiceHandler::LoadRouteTable(seqsvr::Router& _return) {
  auto instance = StoreSvrManager::GetInstance();
  instance->GetCacheRouter(_return);
  
  LOG(INFO) << "LoadRouteTable - _return: " << convertToString(_return);
}

bool StoreServiceHandler::SaveRouteTable(std::unique_ptr<seqsvr::Router> router) {
  auto instance = StoreSvrManager::GetInstance();
  return instance->SaveCacheRouter(*router);
  // response->mutable_router()->CopyFrom(StoreSvrManager::GetInstance()->GetCacheRouter());
  
  LOG(INFO) << "SaveRouteTable - router: " << convertToString(*router);
  return true;
}


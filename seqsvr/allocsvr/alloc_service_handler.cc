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

#include "allocsvr/alloc_service_handler.h"

#include "nebula/base/logger/glog_util.h"
#include "allocsvr/allocsvr_manager.h"

folly::Future<std::unique_ptr< ::seqsvr::Sequence>> AllocServiceHandler::future_FetchNextSequence(int32_t id, int32_t version) {
  auto instance = AllocSvrManager::GetInstance();
  auto _return = std::make_unique<::seqsvr::Sequence>();
  
  // IO线程
  auto evb = getEventBase();
  evb->runInEventBaseThread([]() mutable {
    LOG(INFO) << "evb's thread_id";
  });
  
  // TODO(@benqi): 不同的线程里，FetchNextSequence必须线程安全
  instance->FetchNextSequence(id, version, *_return);
  
  LOG(INFO) << "future_FetchNextSequence - id: " << id
    << ", version: " << version
    << ", _return: " << _return->sequence;

  return folly::makeFuture<std::unique_ptr<seqsvr::Sequence>>(std::move(_return));
}

folly::Future<std::unique_ptr< ::seqsvr::Sequence>> AllocServiceHandler::future_GetCurrentSequence(int32_t id, int32_t version) {
  auto instance = AllocSvrManager::GetInstance();
  auto _return = std::make_unique<::seqsvr::Sequence>();
  
  // TODO(@benqi): 不同的线程里，FetchNextSequence必须线程安全
  instance->GetCurrentSequence(id, version, *_return);
  
  // IO线程
  auto evb = getEventBase();
  evb->runInEventBaseThread([]() mutable {
    LOG(INFO) << "evb's thread_id";
  });

  LOG(INFO) << "future_GetCurrentSequence - id: " << id
    << ", version: " << version
    << ", _return: " << _return->sequence;

  return folly::makeFuture<std::unique_ptr<seqsvr::Sequence>>(std::move(_return));
}

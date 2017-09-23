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

#ifndef ALLOCSVR_ALLOC_SERVICE_HANDLER_H_
#define ALLOCSVR_ALLOC_SERVICE_HANDLER_H_

#include "proto/gen-cpp2/AllocService.h"

class AllocServiceHandler : public seqsvr::AllocServiceSvIf {
public:
  AllocServiceHandler() = default;
  ~AllocServiceHandler() override = default;
  
  folly::Future<std::unique_ptr< ::seqsvr::Sequence>> future_FetchNextSequence(int32_t id, int32_t version) override;
  folly::Future<std::unique_ptr< ::seqsvr::Sequence>> future_GetCurrentSequence(int32_t id, int32_t version) override;
};

#endif

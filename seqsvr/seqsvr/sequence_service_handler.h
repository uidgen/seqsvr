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

#ifndef SEQSVR_SEQUENCE_SERVICE_HANDLER_H_
#define SEQSVR_SEQUENCE_SERVICE_HANDLER_H_

#include "proto/gen-cpp2/SequenceService.h"

class SequenceServiceHandler : public seqsvr::SequenceServiceSvIf {
public:
  SequenceServiceHandler();
  ~SequenceServiceHandler() override;

  void FetchNextSequence( ::seqsvr::SequenceData& result, int32_t id, int32_t version) override;
  void GetCurrentSequence( ::seqsvr::SequenceData& result, int32_t id, int32_t version) override;
  void FetchNextSequenceList(std::vector< ::seqsvr::IdSeq>& result, std::unique_ptr<std::vector<int32_t>> id_list) override;
  void GetCurrentSequenceList(std::vector< ::seqsvr::IdSeq>& result, std::unique_ptr<std::vector<int32_t>> id_list) override;
};

#endif

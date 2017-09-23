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

#include "seqsvr/sequence_service_handler.h"

#include "nebula/base/logger/glog_util.h"
#include "seqsvr/sequence_manager.h"

SequenceServiceHandler::SequenceServiceHandler() {
  LOG(INFO) << "New SequenceServiceHandler!";
}

SequenceServiceHandler::~SequenceServiceHandler() {
  LOG(INFO) << "Delete SequenceServiceHandler";
}

void SequenceServiceHandler::FetchNextSequence( ::seqsvr::SequenceData& result, int32_t id, int32_t version) {
  auto seq = SequenceManager::GetInstance()->FetchNextSequence(id);
  result.set_sequence(seq);
}

void SequenceServiceHandler::GetCurrentSequence( ::seqsvr::SequenceData& result, int32_t id, int32_t version) {
  auto seq = SequenceManager::GetInstance()->GetCurrentSequence(id);
  result.set_sequence(seq);
}

void SequenceServiceHandler::FetchNextSequenceList(std::vector< ::seqsvr::IdSeq>& result, std::unique_ptr<std::vector<int32_t>> id_list) {
  auto seq_mgr = SequenceManager::GetInstance();
  for (size_t i=0; i<id_list->size(); ++i) {
    auto seq = seq_mgr->FetchNextSequence((*id_list)[i]);
    seqsvr::IdSeq id_seq;
    id_seq.id = (*id_list)[i];
    id_seq.sequence = seq;
    result.push_back(id_seq);
  }
}

void SequenceServiceHandler::GetCurrentSequenceList(std::vector< ::seqsvr::IdSeq>& result, std::unique_ptr<std::vector<int32_t>> id_list) {
  auto seq_mgr = SequenceManager::GetInstance();
  for (size_t i=0; i<id_list->size(); ++i) {
    auto seq = seq_mgr->GetCurrentSequence((*id_list)[i]);
    seqsvr::IdSeq id_seq;
    id_seq.id = (*id_list)[i];
    id_seq.sequence = seq;
    result.push_back(id_seq);
  }
}

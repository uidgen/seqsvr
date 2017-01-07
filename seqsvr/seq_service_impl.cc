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

#include "seqsvr/seq_service_impl.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"

#include "seqsvr/sequence_manager.h"

int SeqServiceImpl::FetchNextSequence(const zproto::FetchNextSequenceReq& request, zproto::SequenceRsp* response) {
  auto seq = SequenceManager::GetInstance()->FetchNextSequence(request.id());
  response->set_sequence(seq);
  return 0;
}

int SeqServiceImpl::GetCurrentSequence(const zproto::GetCurrentSequenceReq& request, zproto::SequenceRsp* response) {
  auto seq = SequenceManager::GetInstance()->GetCurrentSequence(request.id());
  response->set_sequence(seq);
  return 0;
}

int SeqServiceImpl::FetchNextSequenceList(const zproto::FetchNextSequenceListReq& request, zproto::SequenceListRsp* response) {
  auto seq_mgr = SequenceManager::GetInstance();
  for (int i=0; i<request.id_list_size(); ++i) {
    auto seq = seq_mgr->FetchNextSequence(request.id_list(i));
    auto id_seq = response->add_sequence_list();
    id_seq->set_id(request.id_list(i));
    id_seq->set_sequence(seq);
  }
  return 0;
}

int SeqServiceImpl::GetCurrentSequenceList(const zproto::GetCurrentSequenceListReq& request, zproto::SequenceListRsp* response) {
  auto seq_mgr = SequenceManager::GetInstance();
  for (int i=0; i<request.id_list_size(); ++i) {
    auto seq = seq_mgr->GetCurrentSequence(request.id_list(i));
    auto id_seq = response->add_sequence_list();
    id_seq->set_id(request.id_list(i));
    id_seq->set_sequence(seq);
  }
  return 0;
}

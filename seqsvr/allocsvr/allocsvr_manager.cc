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

#include "allocsvr/allocsvr_manager.h"

#include <boost/filesystem.hpp>
#include <folly/FileUtil.h>
#include <folly/Range.h>

#include "base/message_handler_util.h"
// #include "storesvr/storesvr_manager.h"

/*
 1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
 2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：
    如果cur_seq > max_seq，将分配上限提升一个步长max_seq += step，并持久化max_seq
 3. 重启时，读出持久化的max_seq，赋值给cur_seq
 */
// folly::Singleton<SequenceManager> ;
static folly::Singleton<AllocSvrManager> g_allocsvr_manager;

std::shared_ptr<AllocSvrManager> AllocSvrManager::GetInstance() {
  return g_allocsvr_manager.try_get();
}

void AllocSvrManager::Initialize(nebula::TimerManager* timer_manager, const std::string& set_name, const std::string& alloc_name) {
  // 首先加载路由表
  // 加载成功后再加载max_seq
  set_name_ = set_name;
  alloc_name_ = alloc_name;
  
  lease_ = std::make_unique<LeaseClerk>(timer_manager, this);
  lease_->Start();
  
/*
  // 1. 初始化set_id_和alloc_id_
  set_id_ = set_id;
  alloc_id_ = alloc_id;
  
  Load(set_id, alloc_id);
 */
}

void AllocSvrManager::Destroy() {
  if (lease_) {
    lease_->Stop();
  }
}

//uint64_t AllocSvrManager::GetCurrentSequence(uint32_t id) {
//#ifdef DEBUG_TEST
//  DCHECK(id<kMaxIDSize);
//#endif
//  std::lock_guard<std::mutex> g(mutex_);
//  return cur_seqs_[id];
//}
//
//uint64_t AllocSvrManager::FetchNextSequence(uint32_t id) {
//#ifdef DEBUG_TEST
//  DCHECK(id<kMaxIDSize);
//#endif
//  
//  auto idx = id/kSectionSize;
//  
//  std::lock_guard<std::mutex> g(mutex_);
//  auto seq = ++cur_seqs_[id];
//  if (seq > section_max_seqs_[idx]) {
//    ++section_max_seqs_[idx];
//    SaveMaxSeq(idx, section_max_seqs_[idx]);
//  }
//  return seq;
//}

bool AllocSvrManager::GetCurrentSequence(uint32_t id, uint32_t client_version, SequenceWithRouterTable& o) {
  if (!cache_alloc_entry_) return false;
  if (state_ != kAllocInited) return false;
  
  for (auto & v : cache_alloc_entry_->ranges ) {
    if (id>=v.id && id<v.id+v.size) {
      o.seq = cur_seqs_[id];
      if (client_version < table_.version()) {
        o.router = new zproto::Router;
        table_.SerializeToRouter(o.router);
      }
      return true;
    }
  }
  
  return false;
}

bool AllocSvrManager::FetchNextSequence(uint32_t id, uint32_t client_version, SequenceWithRouterTable& o) {
  if (!cache_alloc_entry_) return false;
  if (state_ != kAllocInited) return false;
  
  for (auto & v : cache_alloc_entry_->ranges ) {
    if (id>=v.id && id<v.id+v.size) {
      auto idx = id/kSectionSize;
      
      o.seq = ++cur_seqs_[id];
      if (o.seq > section_max_seqs_[idx]) {
        ++section_max_seqs_[idx];
        SaveMaxSeq(idx, section_max_seqs_[idx]);
      }

      if (client_version < table_.version()) {
        o.router = new zproto::Router;
        table_.SerializeToRouter(o.router);
      }
      return true;
    }
  }
  
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 租约生效
void AllocSvrManager::OnLeaseValid(RouteTable& table) {
  table_.Swap(table);
  // route_search_table_.Initialize(table_);
  cache_alloc_entry_ = table_.LookupAlloc(set_name_, alloc_name_);
  state_ = kAllocWaitLoadSeq;
  LoadMaxSeq();
}

// 路由表更新
void AllocSvrManager::OnLeaseUpdated(RouteTable& table) {
  table_.Swap(table);
}

// 租约失效
void AllocSvrManager::OnLeaseInvalid() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
// bytes
void AllocSvrManager::LoadMaxSeq() {
  // TODO(@beqni): NWR读
  // 2. 去storesvr加载max_seqs
  state_ = kAllocWaitRouteTable;
  
  zproto::LoadMaxSeqsDataReq load_max_seqs_data_req;
  ZRpcClientCall<zproto::LoadMaxSeqsDataRsp>(
     "store_client",
     MakeRpcRequest(load_max_seqs_data_req),
     [&] (std::shared_ptr<ApiRpcOk<zproto::LoadMaxSeqsDataRsp>> load_max_seqs_data_rsp,
          ProtoRpcResponsePtr rpc_error) -> int {
       if (rpc_error) {
         LOG(ERROR) << "LoadMaxSeqsDataReq - rpc_error: " << rpc_error->ToString();
         OnMaxSeqLoaded("");
       } else {
         LOG(INFO) << "LoadMaxSeqsDataReq - load_max_seqs_data_rsp: "
                    << load_max_seqs_data_rsp->ToString();
         OnMaxSeqLoaded((*load_max_seqs_data_rsp)->max_seqs());
       }
       return 0;
  });

  // 先使用StoreSvrManager加载，跑通流程
  // auto store = StoreSvrManager::GetInstance();
  // store->Initialize(set_id, "/tmp/seq.dat");
  // std::string max_seqs_data = store->GetSectionsData(set_id, alloc_id);
  // OnLoad(max_seqs_data);
}

void AllocSvrManager::SaveMaxSeq(uint32_t section_id, uint64_t section_max_seq) {
  // TODO(@beqni): NWR写
//  auto store = StoreSvrManager::GetInstance();
//  bool rv = store->SetSectionsData(set_id, alloc_id, section_id, section_max_seq);
  
  zproto::SaveMaxSeqReq save_max_seq_req;
  save_max_seq_req.set_section_id(section_id);
  save_max_seq_req.set_max_seq(section_max_seq);
  
  ZRpcClientCall<zproto::SaveMaxSeqRsp>(
    "store_client",
    MakeRpcRequest(save_max_seq_req),
    [section_max_seq, this] (std::shared_ptr<ApiRpcOk<zproto::SaveMaxSeqRsp>> save_max_seq_rsp,
                             ProtoRpcResponsePtr rpc_error) -> int {
      if (rpc_error) {
        LOG(ERROR) << "SaveMaxSeqReq - rpc_error: " << rpc_error->ToString();
        this->OnMaxSeqSaved(false);
      } else {
        LOG(INFO) << "SaveMaxSeqReq - load_max_seqs_data_rsp: " << save_max_seq_rsp->ToString();
        this->OnMaxSeqSaved((*save_max_seq_rsp)->last_max_seq() == section_max_seq-1);
      }
      return 0;
  });
}

void AllocSvrManager::OnMaxSeqLoaded(const std::string& data) {
  if (!data.empty()) {
    // TODO(@benqi): 检查数据是否合法
    // 复制数据
    memcpy(section_max_seqs_.data(), data.c_str(), data.length());
    // 将cur_seq设置为max_seq
    for (int i=0; i<kSectionSlotSize-1; ++i) {
      std::fill(cur_seqs_.begin()+i*kSectionSize, cur_seqs_.begin()+(i+1)*kSectionSize, section_max_seqs_[i]);
    }

    std::fill(cur_seqs_.begin()+(kSectionSlotSize-1)*kSectionSize, cur_seqs_.end(), section_max_seqs_[kSectionSlotSize-1]);

    state_ = kAllocInited;
  } else {
    state_ = kAllocError;
  }
}

void AllocSvrManager::OnMaxSeqSaved(bool result) {
}



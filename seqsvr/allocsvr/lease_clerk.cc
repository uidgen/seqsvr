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

#include "allocsvr/lease_clerk.h"

// #include "base/message_handler_util.h"
#include "nebula/base/time_util.h"
#include "allocsvr/client_manager.h"


LeaseClerk::LeaseClerk(nebula::TimerManager* timer_manager,
    ClientManager* client,
    LeaseCallback* cb)
  : timer_manager_(timer_manager),
    client_(client),
    cb_(cb) {
}

// 开始租约服务
void LeaseClerk::Start() {
  // 只能执行一次
  CHECK(state_ == STATE_NONE);
  
  LOG(INFO) << "Start lease...";
  // 启动以后请求租约
  RequestLease();
  
  // timer_manager_->ScheduleRepeatingTimeout(
  //     std::bind(&LeaseClerk::RequestLease, this), SYNC_LEASE_TIMEOUT);

}

// 停止租约服务
void LeaseClerk::Stop() {
  timer_manager_->GetMainEventBase()
      ->runImmediatelyOrRunInEventBaseThreadAndWait([this] {
        // 启动检查租约失效
        timer_manager_->cancelAll();
      });
}

int LeaseClerk::RequestLease() {
  LOG(INFO) << "RequestLease - LoadRouteTable...";
  // TODO(@benqi):
  // 同步失败后需要停止服务
  // 一旦成功收到，则表示续租成功
  seqsvr::Router router;
  auto r = client_->LoadRouteTable(router);
  if (r==0) {
    auto now = NowInMsecTime();
    lease_invalid_time_ = now + LEASE_TIMEOUT;
    LOG(INFO) << "RequestLease - LoadRouteTable ok: now=" << now
            << ", lease_invalid_time = " << lease_invalid_time_;

    return OnHandleLease(router);
  } else {
    LOG(ERROR) << "RequestLease - LoadRouteTable error";
    // timer_manager_->ScheduleOneShotTimeout(
    //    std::bind(&LeaseClerk::RequestLease, this), SYNC_LEASE_TIMEOUT);
  }
  return 0;
}

int LeaseClerk::OnHandleLease(seqsvr::Router& router) {
  // 租约是否生效？
  if (router.version > version_) {
    version_ = router.version;
    if (state_ != STATE_VALID) {
      state_ = STATE_VALID;
  
      LOG(INFO) << "OnHandleLease - is_valid";
      // 租约生效
      cb_->OnLeaseValid(router);
      
      // 数据已达
      // timer_manager_->GetMainEventBase()->runInEventBaseThread([this] {
      // 启动检查租约失效
      timer_manager_->cancelAll();
      LOG(INFO) << "OnHandleLease - start timer...";
      timer_manager_->ScheduleRepeatingTimeout(
          std::bind(&LeaseClerk::CheckLeaseValid, this), CHECK_LEASE_TIMEOUT);
      timer_manager_->ScheduleRepeatingTimeout(
          std::bind(&LeaseClerk::RequestLease, this), SYNC_LEASE_TIMEOUT);
      return 1;
    }
  }
  
  return 0;
}

int LeaseClerk::CheckLeaseValid() {
  auto now = NowInMsecTime();
  
  // 租约检查
  LOG(WARNING) << "CheckLeaseValid - check: now=" << now
               << ", lease_invalid_time = " << lease_invalid_time_;

  if (now > lease_invalid_time_) {
    if (state_ == STATE_VALID) {

      state_ = STATE_INVALID;
      cb_->OnLeaseInvalid();
      version_ = 0;
      LOG(WARNING) << "CheckLeaseValid - cancel, state: " << state_;
      // lease_invalid_time";
      timer_manager_->cancelAll();
      timer_manager_->ScheduleRepeatingTimeout(
          std::bind(&LeaseClerk::RequestLease, this), SYNC_LEASE_TIMEOUT);
      return 1;

    }
  }
  
  return 0;
}

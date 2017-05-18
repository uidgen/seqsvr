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

#include "base/message_handler_util.h"
#include "nebula/base/time_util.h"

// 开始租约服务
void LeaseClerk::Start() {
  // 已经启动不执行
  if (is_valid_)
    return;

  // 启动以后请求租约
  RequestLease();
}

// 停止租约服务
void LeaseClerk::Stop() {
  if (!is_valid_)
    return;

  timer_manager_->GetMainEventBase()
      ->runImmediatelyOrRunInEventBaseThreadAndWait([this] {
        // 启动检查租约失效
        timer_manager_->GetHHWheelTimer()->cancelAll();
      });
}

void LeaseClerk::RequestLease() {
  zproto::GetRouteTableReq get_route_table_req;
  ZRpcClientCall<zproto::GetRouteTableRsp>(
      "store_client", MakeRpcRequest(get_route_table_req),
      [this](std::shared_ptr<ApiRpcOk<zproto::GetRouteTableRsp>> rpc_ok,
             ProtoRpcResponsePtr rpc_error) -> int {
        if (rpc_error) {
          LOG(ERROR) << "RequestRouteTable - rpc_error: "
                     << rpc_error->ToString();
          // this->OnSave(false);
        } else {
          LOG(INFO) << "RequestRouteTable - rpc_ok: " << rpc_ok->ToString();
          OnHandleLease((*rpc_ok)->router());
        }
        return 0;
      });
}

void LeaseClerk::OnHandleLease(const zproto::Router &router) {
  int cb = -1;
  {
    std::lock_guard<std::mutex> g(lock_);
    lease_invalid_time_ = NowInMsecTime() + LEASE_TIMEOUT;
    if (router.version() > version_) {
      version_ = router.version();
      if (!is_valid_) {
        is_valid_ = true;
        cb = 0;
      } else {
        cb = 1;
      }
    } else {
    }
  }

  if (cb_ && cb != -1) {
    RouteTable table;
    table.ParseFromRouter(router);
    if (cb == 0) {
      cb_->OnLeaseValid(table);
    } else {
      cb_->OnLeaseUpdated(table);
    }
  }

  // 数据已达
  timer_manager_->GetMainEventBase()->runInEventBaseThread([cb, this] {
    if (cb == 0) {
      // 启动检查租约失效
      timer_manager_->GetHHWheelTimer()->cancelAll();
      timer_manager_->ScheduleRepeatingTimeout(
          std::bind(&LeaseClerk::CheckLeaseValid, this), CHECK_LEASE_TIMEOUT);
    }

    timer_manager_->ScheduleOneShotTimeout(
        std::bind(&LeaseClerk::RequestLease, this), SYNC_LEASE_TIMEOUT);
  });
}

void LeaseClerk::CheckLeaseValid() {
  auto now = NowInMsecTime();

  LOG(WARNING) << "CheckLeaseValid - check: now=" << now
               << ", lease_invalid_time = " << lease_invalid_time_;

  bool is_valid = true;
  {
    std::lock_guard<std::mutex> g(lock_);
    if (is_valid_) {
      if (now > lease_invalid_time_) {
        is_valid_ = false;
        LOG(WARNING) << "CheckLeaseValid - lease_invalid_time";
      }
    }

    is_valid = is_valid_;
  }

  if (!is_valid) {
    timer_manager_->GetMainEventBase()->runInEventBaseThread(
        [this] {
          timer_manager_->GetHHWheelTimer()->cancelAll();
        });
  } else {
    if (cb_) {
      cb_->OnLeaseInvalid();
    }
  }
}

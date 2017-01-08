/*
 *  Copyright (c) 2016, https://github.com/zhatalk
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

#ifndef	BASE_ZPROTO_HANDLER_UTIL_H_
#define	BASE_ZPROTO_HANDLER_UTIL_H_

#include <nebula/nebula/net/zproto/zproto_package_data.h>
#include "nebula/net/zproto/zproto_package_data.h"
#include "nebula/net/handler/nebula_handler_util.h"
#include "nebula/net/rpc/zrpc_service_util.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"


#define RPC_TIMEOUT 5000  // 5000毫秒

inline folly::Future<folly::Unit> WritePackage(const std::string& service_name, std::shared_ptr<PackageMessage> message_data) {
  std::unique_ptr<folly::IOBuf> data;
  message_data->SerializeToIOBuf(data);
  return WriterUtil::Write(service_name, std::move(data));
}

inline folly::Future<folly::Unit> WritePackage(uint64_t conn_id, std::shared_ptr<PackageMessage> message_data) {
  std::unique_ptr<folly::IOBuf> data;
  message_data->SerializeToIOBuf(data);
  return WriterUtil::Write(conn_id, std::move(data));
}

inline folly::Future<folly::Unit> WritePackage(nebula::ZProtoPipeline* pipeline, std::shared_ptr<PackageMessage> message_data) {
  std::unique_ptr<folly::IOBuf> data;
  message_data->SerializeToIOBuf(data);
  return pipeline->write(std::move(data));
}

inline folly::Future<folly::Unit> WriteToClientPackage(uint64_t conn_id, std::shared_ptr<PackageMessage> message_data) {
  std::unique_ptr<folly::IOBuf> data;
  message_data->clear_has_attach_data();
  message_data->SerializeToIOBuf(data);
  return WriterUtil::Write(conn_id, std::move(data));
}

inline folly::Future<folly::Unit> WriteToClientPackage(nebula::ZProtoPipeline* pipeline, std::shared_ptr<PackageMessage> message_data) {
  std::unique_ptr<folly::IOBuf> data;
  message_data->clear_has_attach_data();
  message_data->SerializeToIOBuf(data);
  return pipeline->write(std::move(data));
}

template <typename MESSAGE>
inline void ZRpcClientCall(const std::string& service_name,
                           RpcRequestPtr request,
                           std::function<int(std::shared_ptr<ApiRpcOk<MESSAGE>>, ProtoRpcResponsePtr)> cb,
                           int timeout = RPC_TIMEOUT) {
  ZRpcUtil::DoClientCall(service_name, request)
    .within(std::chrono::milliseconds(timeout))
      // TODO(@benqi): conn_data可能会失效
    .then([cb](ProtoRpcResponsePtr rsp) {
      static ProtoRpcResponsePtr kEmptyRpcError;
      static std::shared_ptr<ApiRpcOk<MESSAGE>> kEmptyRpcOk;

      CHECK(rsp);
      LOG(INFO) << "ZRpcClientCall rsp: " << rsp->ToString();

      if (rsp->GetPackageType() == Package::RPC_OK) {
        auto rpc_ok = ToApiRpcOk<MESSAGE>(rsp);
        // LOG(INFO) << "auth_client login_rsp: " << (*login_rsp)->Utf8DebugString();
        cb(rpc_ok, kEmptyRpcError);
      } else {
        cb(kEmptyRpcOk, rsp);
      }
    });
}

//// s2s中，我们将conn_id存储在PackageMessage的options里
//inline uint64_t GetConnIDByPackageMessage(const std::shared_ptr<PackageMessage>& message_data) {
//  const auto& options = message_data->options();
//  if (!options.empty() && options[0].type==0) {
//    return options[0].data.n;
//  }
//  return -1;
//}

#endif // BASE_HANDLER_UTIL_H_


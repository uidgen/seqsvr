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

// TODO(@benqi): 使用zrpc-code-gen代码生成工具自动生成

#ifndef	ALLOCSVR_ALLOC_SERVER_H_
#define	ALLOCSVR_ALLOC_SERVER_H_

#include <thrift/lib/cpp2/server/ThriftServer.h>

#include "nebula/base/base_daemon.h"

#include "base/section.h"
#include "base/config.h"

// 缓存中间层AllocSvr。
// AllocSvr则是缓存中间层，部署于多台机器，
// 每台AllocSvr负责若干号段的sequence分配，分摊海量的sequence申请请求。
class AllocServer : public nebula::BaseDaemon, public nebula::Configurable {
public:
  AllocServer();
  ~AllocServer() override = default;
  
  // Override from Configurable
  bool SetConf(const std::string& conf_name, const folly::dynamic& conf) override;

protected:
  bool Initialize() override;
  bool Run() override;
  void Quit() override;
  
private:
  IpAddrInfo addr_;
  SetsConfig sets_;

  std::shared_ptr<apache::thrift::ThriftServer> server_;
};

#endif // ALLOCSVR_ALLOC_SERVER_H_

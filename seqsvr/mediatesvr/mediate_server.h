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

#ifndef	MEDIATESVR_MEDIATE_SERVER_H_
#define	MEDIATESVR_MEDIATE_SERVER_H_

#include <folly/io/async/EventBase.h>
#include <thrift/lib/cpp2/server/ThriftServer.h>

#include "nebula/base/base_daemon.h"

// 仲裁服务的一个主要功能探测AllocSvr:
// 这里需要引入一个仲裁服务，探测AllocSvr的服务状态，决定每个uid段由哪台AllocSvr加载。
// 出于可靠性的考虑，仲裁模块并不直接操作AllocSvr，而是将加载配置写到StoreSvr持久化，
// 然后AllocSvr定期访问StoreSvr读取最新的加载配置，决定自己的加载状态。
//
// TODO(@benq): 引入第三方服务，比如etcd或zookeeper
// 当前暂不实现AllocSvr探测，提供一个api接口更新StoreSvr里的路由表
class MediateServer : public nebula::BaseDaemon {
public:
  MediateServer() = default;
  ~MediateServer() override = default;
  
protected:
  // 不使用自动配置框架
  bool LoadConfig(const std::string& config_file) override {
    return true;
  }
  
  bool Initialize() override;
  bool Run() override;
  void Quit() override;
  
private:
  std::shared_ptr<apache::thrift::ThriftServer> server_;
};

#endif // MEDIATESVR_MEDIATE_SERVER_H_


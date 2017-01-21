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

#include "nebula/net/base_server.h"

// 仲裁服务的一个主要功能探测AllocSvr
// 一般会引入第三方服务，比如etcd或zookeeper
// 当前暂不实现AllocSvr探测，提供一个api接口更新StoreSvr里的路由表
class MediateServer : public nebula::BaseServer {
public:
  MediateServer() = default;
  ~MediateServer() override = default;
  
protected:
  // From BaseServer
  bool Initialize() override;
  bool Run() override;
};

#endif // MEDIATESVR_MEDIATE_SERVER_H_


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

#ifndef	STORESVR_STORE_SERVER_H_
#define	STORESVR_STORE_SERVER_H_

#include <thrift/lib/cpp2/server/ThriftServer.h>

#include "nebula/base/configurable.h"
#include "nebula/base/base_daemon.h"

#include "base/section.h"
#include "base/config.h"

// 把存储层StoreSvr
// StoreSvr为存储层，利用了多机NRW策略来保证数据持久化后不丢失
//
// NWR模型，把CAP的选择权交给了用户，让用户自己选择CAP中的哪两个。
//
// N代表N个副本（replication），
// W代表写入数据时至少要写入W份副本才认为成功，
// R表示读取数据时至少要读取R份副本。
// 对于R和W的选择，要求W+R > N。
class StoreServer : public nebula::BaseDaemon, public nebula::Configurable {
public:
  StoreServer();
  ~StoreServer() override = default;
  
  // Override from Configurable
  bool SetConf(const std::string& conf_name, const folly::dynamic& conf) override;

protected:
  bool Initialize() override;
  bool Run() override;
  void Quit() override;
  
  // StoreConfig store_config_;
  
private:
  std::shared_ptr<apache::thrift::ThriftServer> server_;
  
  SetID set_id_;
  IpAddrInfo addr_;
  SetsConfig sets_;
  std::string db_path_;
  
};

#endif // STORESVR_STORE_SERVER_H_

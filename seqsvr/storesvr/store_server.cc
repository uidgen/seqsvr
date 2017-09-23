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

#include "storesvr/store_server.h"

#include "nebula/base/config_manager.h"

#include "storesvr/storesvr_manager.h"
#include "storesvr/store_service_handler.h"
#include "base/thrift_util.h"

StoreServer::StoreServer() {
  auto config_manager = nebula::ConfigManager::GetInstance();
  config_manager->Register("system", this);
  config_manager->Register("storesvr", this);
  config_manager->Register("sets", this);
}

// Override from Configurable
bool StoreServer::StoreServer::SetConf(const std::string& conf_name, const folly::dynamic& conf) {
  if (conf_name == "storesvr") {
    addr_ = folly::convertTo<IpAddrInfo>(conf);
  } else if (conf_name == "system") {
    db_path_ = folly::convertTo<std::string>(conf["db_path"]);
    set_id_.id_begin = folly::convertTo<uint32_t>(conf["set_id"]);
    set_id_.size = folly::convertTo<uint32_t>(conf["set_size"]);
  } else if (conf_name == "sets") {
    sets_ = folly::convertTo<SetsConfig>(conf);
  }
  
  return true;
}

bool StoreServer::Initialize() {
  auto instance = StoreSvrManager::GetInstance();
  instance->Initialize(set_id_, db_path_);

#ifdef DEBUG_TEST
  auto router = MakeTestRouter(sets_);
  LOG(INFO) << "MakeTestRouter: " << convertToString(router);
  instance->SaveCacheRouter(router);
#endif
  
  // RegisterService("store_server", "rpc_server", "zrpc");
  // BaseServer::Initialize();
  
  return true;
}

bool StoreServer::Run() {
  auto handler = std::make_shared<StoreServiceHandler>();
  server_ = std::make_shared<apache::thrift::ThriftServer>();
  server_->setInterface(handler);
  server_->setPort(addr_.port);
  
  printf("Starting the server...\n");
  server_->serve();
  printf("done.\n");
  return true;
}

void StoreServer::Quit() {
  server_->stop();
  sleep(1);
  BaseDaemon::Quit();
}

int main(int argc, char* argv[]) {
  return nebula::DoMain<StoreServer>(argc, argv);
}

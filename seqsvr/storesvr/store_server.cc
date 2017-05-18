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


StoreServer::StoreServer() {
  auto config_manager = nebula::ConfigManager::GetInstance();
  
  config_manager->Register("store", &store_config_);
}


bool StoreServer::Initialize() {
  auto store_instance = StoreSvrManager::GetInstance();
  store_instance->Initialize(store_config_.set_name, store_config_.store_path);
  
  RegisterService("store_server", "rpc_server", "zrpc");
  BaseServer::Initialize();
  
#if 0
  // one
  timer_manager_->ScheduleOneShotTimeout([]() {
    LOG(INFO) << "ScheduleOneShotTimeout!!!!";
  }, 1000);
  
  // once
  timer_manager_->ScheduleRepeatingTimeout([]() {
    static int i = 0;
    LOG(INFO) << "ScheduleRepeatingTimeout - " << i++;
  }, 1000);
#endif
  
  return true;
}

int main(int argc, char* argv[]) {
  return nebula::DoMain<StoreServer>(argc, argv);
}

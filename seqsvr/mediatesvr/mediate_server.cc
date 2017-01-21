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

#include "mediatesvr/mediate_server.h"
#include "mediatesvr/mediate_handler.h"

bool MediateServer::Initialize() {

  // 客户端连接服务
  RegisterService("mediate_server", "http_server", "http");
  RegisterService("store_client", "rpc_client", "zrpc");
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

bool MediateServer::Run() {
  BaseServer::Run();
  return true;
}

int main(int argc, char* argv[]) {
  return nebula::DoMain<MediateServer>(argc, argv);
}

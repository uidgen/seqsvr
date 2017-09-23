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

#include "client/client.h"

#include <iostream>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>
#include "nebula/base/readline_gets.h"

#include "proto/gen-cpp2/seqsvr_types.h"
#include "proto/gen-cpp2/AllocService.h"

#include "client/client_command_handler.h"

using namespace apache::thrift;
//
//#include "proto/cc/seqsvr.pb.h"
//#include "nebula/net/zproto/api_message_box.h"
//
//#include "nebula/net/rpc/zrpc_service_util.h"
//#include "nebula/net/net_engine_manager.h"


bool Client::Initialize() {
  // 注册服务
  // RegisterService("seq_client", "rpc_client", "zrpc");
  // 注册服务
  // RegisterService("alloc_client", "rpc_client", "zrpc");

  return BaseDaemon::Initialize();
}

bool Client::Run() {
  // auto net_engine_manager = nebula::NetEngineManager::GetInstance();
  // 启动成功
  // try {
  //   nebula::NetEngineManager::GetInstance()->Start();
  // } catch (std::exception& e) {
  //   LOG(ERROR) << "Run - catch exception: " << e.what();
  //   return false;
  // } catch (...) {
  //   LOG(ERROR) << "Run - catch  a invalid exception";
  //   return false;
  // }
  
  // GPerftoolsProfiler profiler;
  // profiler.ProfilerStart();
  
  //BaseDaemon::Run();
  DoCommandLineLoop();
  
  // profiler.ProfilerStop();
  
  // nebula::NetEngineManager::GetInstance()->Stop();

  return true;
  // return BaseServer::Run();
}

void Client::DoCommandLineLoop() {
  
  // EventBase eb;
  auto client = std::make_unique<seqsvr::AllocServiceAsyncClient>(
        HeaderClientChannel::newChannel(
        async::TAsyncSocket::newSocket(
        &main_eb_, {"127.0.0.1", 10000})));

  try {
    while (true) {
      auto line = ReadlineGets("nebula-im> ");
      if (line.empty()) {
        continue;
      }
      
      std::vector<folly::StringPiece> cmds;
      folly::split(" ", line, cmds);
      
      if (-2 == DoClientCommand(client.get(), cmds)) {
        break;
      }
    }
  } catch (const std::exception& e) {
    std::cout << "DoCommandLineLoop - " << folly::exceptionStr(e) << std::endl;
  }
}


int main(int argc, char* argv[]) {
  return nebula::DoMain<Client>(argc, argv);
}


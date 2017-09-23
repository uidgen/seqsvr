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

#include "allocsvr/alloc_server.h"

#include <folly/json.h>
#include "nebula/base/config_manager.h"
#include "allocsvr/allocsvr_manager.h"
#include "allocsvr/alloc_service_handler.h"

AllocServer::AllocServer() {
  auto config_manager = nebula::ConfigManager::GetInstance();
  config_manager->Register("allocsvr", this);
  config_manager->Register("sets", this);
}

// Override from Configurable
bool AllocServer::SetConf(const std::string& conf_name, const folly::dynamic& conf) {
  LOG(INFO) << "conf_name: " << conf_name << folly::toJson(conf);
  
  if (conf_name == "allocsvr") {
    addr_ = folly::convertTo<IpAddrInfo>(conf);
  } else if (conf_name == "sets") {
    sets_ = folly::convertTo<SetsConfig>(conf);
  }
  
  return true;
}

bool AllocServer::Initialize() {
  // RegisterService("alloc_server", "rpc_server", "zrpc");
  // RegisterService("store_client", "rpc_client", "zrpc");
  
  // BaseServer::Initialize();
 
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

bool AllocServer::Run() {
//  auto net_engine_manager = nebula::NetEngineManager::GetInstance();
//  // 启动成功
//  try {
//    net_engine_manager->Start();
//  } catch (std::exception& e) {
//    LOG(ERROR) << "Run - catch exception: " << e.what();
//    return -1;
//  } catch (...) {
//    LOG(ERROR) << "Run - catch  a invalid exception";
//    return -1;
//  }
  
  // auto alloc_name = std::string("alloc_server:10000");
  // static IpAddrInfo addr("127.0.0.1", 10000);
  // static std::list<IpAddrInfo> addrs;
  // addrs.emplace_back("127.0.0.1", 9090);

  // 服务发现
  // 根据本机服务找到set
  // 通过set找到set所有的storesvr
  IpAddrInfoList* addrs = nullptr;
  for (auto& s : sets_) {
    for (auto & a : s.allocsvrs) {
      if (addr_ == a) {
        addrs = &s.storesvrs;
        break;
      }
    }
  }
  
  if (addrs == nullptr) {
    // 没有找到符合条件的set
    LOG(ERROR) << "Not find allocsvr: " << addr_.ToString();
    return false;
  }
  
  auto alloc_manager = AllocSvrManager::GetInstance();
  alloc_manager->Initialize(timer_manager_.get(), addr_, *addrs);

  // 等thread线程创建成功然后返回
  // folly::EventBase* main_eb = nullptr;
  std::thread t([&] () {
    auto handler = std::make_shared<AllocServiceHandler>();
    server_ = std::make_shared<apache::thrift::ThriftServer>();
    server_->setInterface(handler);
    server_->setPort(addr_.port);
    
    LOG(INFO) << "Starting the server...";
    server_->serve();
    LOG(INFO) << "done.";
  });

//  main_eb_.runAfterDelay([&] {
//    auto alloc_service = net_engine_manager->Lookup("alloc_server");
//    auto alloc_name = std::string("alloc_server:10000");
//
//    // GPerftoolsProfiler profiler;
//    // profiler.ProfilerStart();
//    alloc_manager->Initialize(timer_manager_.get(), "set_cluster_1", alloc_name);
//    
//    // auto store_instance = StoreSvrManager::GetInstance();
//    // store_instance->Initialize(1, "/tmp/seq.dat");
//  }, 1000);
//  
  BaseDaemon::Run();
  if (server_) {
    server_->stop();
  }
  t.join();
//  
//  // profiler.ProfilerStop();
//  
//  net_engine_manager->Stop();
  alloc_manager->Destroy();
  
  return true;
}

void AllocServer::Quit() {
  // sleep(1);
  BaseDaemon::Quit();
}

int main(int argc, char* argv[]) {
  return nebula::DoMain<AllocServer>(argc, argv);
}

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
// #include "mediatesvr/mediate_handler.h"

#include "mediatesvr/mediate_service_handler.h"

bool MediateServer::Initialize() {
  return true;
}

bool MediateServer::Run() {
  auto handler = std::make_shared<MediateServiceHandler>();
  server_ = std::make_shared<apache::thrift::ThriftServer>();
  server_->setInterface(handler);
  server_->setPort(9090);
  
  printf("Starting the server...\n");
  server_->serve();
  printf("done.\n");
  return true;
}

void MediateServer::Quit() {
  server_->stop();
}

// #include <folly/json.h>
// #include "base/config.h"

int main(int argc, char* argv[]) {
  // auto sets = MakeTestSetsConfig(2);
  // auto d = folly::toDynamic(sets);
  // folly::PrintTo(d, &std::cout);
  
  return nebula::DoMain<MediateServer>(argc, argv);
}

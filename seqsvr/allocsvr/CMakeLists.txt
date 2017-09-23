#  Copyright (c) 2016, https://github.com/nebula-im/nebula
#  All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include_directories(. .. ../../nebula)

set (SRC_LIST
  lease_clerk.cc
  lease_clerk.h
  client_manager.cc
  client_manager.h
  allocsvr_manager.cc
  allocsvr_manager.h
  alloc_server.cc
  alloc_server.h
  alloc_service_handler.cc
  alloc_service_handler.h
)

add_executable (allocsvr ${SRC_LIST})
#target_link_libraries (allocsvr net base protobuf)
target_link_libraries (allocsvr seqsvr_base base seqsvr_gen-cpp2 boost_system boost_filesystem pthread glog gflags folly thrift thriftcpp2 thriftprotocol zstd ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} dl)


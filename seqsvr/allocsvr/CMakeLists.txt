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
  ../base/section.h
  ../base/set.h
  ../base/router_table.cc
  ../base/router_table.h
  lease_clerk.cc
  lease_clerk.h
  allocsvr_manager.cc
  allocsvr_manager.h
  alloc_server.cc
  alloc_server.h
  zrpc_alloc_dispatcher.cc
  zrpc_alloc_dispatcher.h
  zrpc_alloc_service.cc
  zrpc_alloc_service.h
  alloc_service_impl.cc
  alloc_service_impl.h

  ../proto/cc/seqsvr.pb.cc
  ../proto/cc/seqsvr.pb.h
  ../proto/proto/seqsvr.proto
  ../base/message_handler_util.h
  )

add_executable (allocsvr ${SRC_LIST})
target_link_libraries (allocsvr net base protobuf)


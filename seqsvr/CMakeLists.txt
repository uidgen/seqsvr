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
  sequence_manager.cc
  sequence_manager.h
  seq_server.cc
  seq_server.h
  zrpc_seq_dispatcher.cc
  zrpc_seq_dispatcher.h
  zrpc_seq_service.cc
  zrpc_seq_service.h
  seq_service_impl.cc
  seq_service_impl.h

  ../proto/cc/seqsvr.pb.cc
  ../proto/cc/seqsvr.pb.h
  ../proto/proto/seqsvr.proto
  )

add_executable (seqsvr ${SRC_LIST})
target_link_libraries (seqsvr net base protobuf)


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

  mediate_server.cc
  mediate_server.h
  mediate_handler.cc
  mediate_handler.h

  ../proto/cc/seqsvr.pb.cc
  ../proto/cc/seqsvr.pb.h
  ../proto/proto/seqsvr.proto
  )

add_executable (mediatesvr ${SRC_LIST})
target_link_libraries (mediatesvr net base protobuf)


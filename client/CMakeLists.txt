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

include_directories(. .. ../seqsvr ../../nebula)

set (SRC_LIST
  client.cc
  client.h
  client_command_handler.cc
  client_command_handler.h
  ../../nebula/nebula/base/readline_gets.cc
  ../../nebula/nebula/base/readline_gets.h
  )

add_executable (client ${SRC_LIST})
target_link_libraries (client seqsvr_base base seqsvr_gen-cpp2 boost_system boost_filesystem pthread glog gflags folly thrift thriftcpp2 thriftprotocol zstd ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} readline dl)


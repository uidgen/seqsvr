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
 seqsvr.thrift

 gen-cpp2/AllocService.cpp
 gen-cpp2/AllocService.h
 gen-cpp2/AllocService.tcc
 gen-cpp2/AllocService_client.cpp
 gen-cpp2/AllocService_custom_protocol.h
 gen-cpp2/AllocService_processmap_binary.cpp
 gen-cpp2/AllocService_processmap_compact.cpp
 gen-cpp2/MediateService.cpp
 gen-cpp2/MediateService.h
 gen-cpp2/MediateService.tcc
 gen-cpp2/MediateService_client.cpp
 gen-cpp2/MediateService_custom_protocol.h
 gen-cpp2/MediateService_processmap_binary.cpp
 gen-cpp2/MediateService_processmap_compact.cpp
 gen-cpp2/StoreService.cpp
 gen-cpp2/StoreService.h
 gen-cpp2/StoreService.tcc
 gen-cpp2/StoreService_client.cpp
 gen-cpp2/StoreService_custom_protocol.h
 gen-cpp2/StoreService_processmap_binary.cpp
 gen-cpp2/StoreService_processmap_compact.cpp
 gen-cpp2/seqsvr_constants.cpp
 gen-cpp2/seqsvr_constants.h
 gen-cpp2/seqsvr_data.cpp
 gen-cpp2/seqsvr_data.h
 gen-cpp2/seqsvr_types.cpp
 gen-cpp2/seqsvr_types.h
 gen-cpp2/seqsvr_types.tcc
 gen-cpp2/seqsvr_types_custom_protocol.h

)

add_library(seqsvr_gen-cpp2 STATIC ${SRC_LIST})


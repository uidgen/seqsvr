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

cmake_minimum_required(VERSION 2.8)

project(seqsvr C CXX)

#add_compile_options(-std=c++1y)
add_compile_options(-fPIC)
add_compile_options(-DFOLLY_HAVE_CLOCK_GETTIME=1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++1y")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_library(GLOG_LIBRARY_PATH glog)
find_library(GFLAGS_LIBRARY_PATH gflags)
find_package(Folly REQUIRED)
find_package(Wangle REQUIRED)
find_package(Proxygen REQUIRED COMPONENTS proxygenhttpserver proxygenlib)

include_directories(
${CMAKE_SOURCE_DIR}
${BOOST_INCLUDE_DIR}
${OPENSSL_INCLUDE_DIR}
${FOLLY_INCLUDE_DIR}
${WANGLE_INCLUDE_DIR}
${PROXYGEN_INCLUDE_DIR}
${INCLUDE_DIR}
${PROJECT_SOURCE_DIR}/nebula
)

#link_directories(/usr/local/Cellar/openssl/1.0.2d_1/lib /opt/local/lib /usr/local/lib /usr/local/mysql/lib ${PROJECT_BINARY_DIR}/lib)

message("Folly lib: " ${FOLLY_LIBRARY})
message("Wangle lib: " ${WANGLE_LIBRARY})
message("Proxygen lib: " ${PROXYGEN_LIBRARY})

link_directories(
/usr/local/lib
/usr/local/mysql/lib
#${FOLLY_LIBRARYDIR}
#${WANGLE_LIBRARYDIR}
${PROXYGEN_LIBRARYDIR}
#${OPENSSL_LIBRARYDIR}
#${BOOST_LIBRARYDIR}
#${PROJECT_BINARY_DIR}/lib
)

#set(CMAKE_EXE_LINKER_FLAGS "-lpthread -lrt -ldl")

add_subdirectory(nebula/nebula)
add_subdirectory(client)
add_subdirectory(seqsvr)

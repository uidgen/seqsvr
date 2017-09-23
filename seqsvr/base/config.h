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

#ifndef SEQSVR_BASE_CONFIG_H_
#define SEQSVR_BASE_CONFIG_H_

// #include "nebula/base/json/json.h"

#include <string>
#include <list>
#include <folly/DynamicConverter.h>

#include "nebula/net/base/ip_addr_info.h"
#include "nebula/base/configurable.h"

#include "proto/gen-cpp2/seqsvr_types.h"
// #include "nebula/base/string_builder.h"


/*******************************************************************
// sets.json配置文件格式
[
  {
    "allocsvrs" : [
      {
        "addr" : "127.0.0.1",
        "port" : 10000
      },
      {
        "addr" : "127.0.0.1",
        "port" : 10001
      },
      {
        "addr" : "127.0.0.1",
        "port" : 10002
      }
    ],
    "id" : 0,
    "size" : 1048576,
    "storesvrs" : [
      {
        "addr" : "127.0.0.1",
        "port" : 11000
      },
      {
        "addr" : "127.0.0.1",
        "port" : 11001
      },
      {
        "addr" : "127.0.0.1",
        "port" : 11002
      }
    ]
  },
  {
    "allocsvrs" : [
      {
        "addr" : "127.0.0.1",
        "port" : 10003
      },
      {
        "addr" : "127.0.0.1",
        "port" : 10004
      },
      {
        "addr" : "127.0.0.1",
        "port" : 10005
      }
    ],
    "id" : 1048576,
    "size" : 1048576,
    "storesvrs" : [
      {
        "addr" : "127.0.0.1",
        "port" : 11003
      },
      {
        "addr" : "127.0.0.1",
        "port" : 11004
      },
      {
        "addr" : "127.0.0.1",
        "port" : 11005
      }
    ]
  }
]
*/


// 配置文件
struct SetConfig {
  uint32_t id;
  uint32_t size;
  IpAddrInfoList allocsvrs;
  IpAddrInfoList storesvrs;
};

// TODO(@benqi): 使用反射Marsh/Unmarshal
namespace folly {
  
template <> struct DynamicConverter<SetConfig> {
  static SetConfig convert(const dynamic& d) {
    SetConfig set;
    set.id = convertTo<uint32_t>(d["id"]);
    set.size = convertTo<uint32_t>(d["size"]);
    set.allocsvrs = convertTo<IpAddrInfoList>(d["allocsvrs"]);
    set.storesvrs = convertTo<IpAddrInfoList>(d["storesvrs"]);
    
    return set;
  }
};

template <> struct DynamicConstructor<SetConfig, void> {
  static dynamic construct(const SetConfig& x) {
    dynamic d = dynamic::object;
    d.insert("id", x.id);
    d.insert("size", x.size);
    d.insert("allocsvrs", toDynamic(x.allocsvrs));
    d.insert("storesvrs", toDynamic(x.storesvrs));
    return d;
  }
};
  
}

// struct SetsConfig {
typedef std::list<SetConfig> SetsConfig;


/////////////////////////////////////////////////////////////////////
// DEBUG_TEST
SetsConfig MakeTestSetsConfig(int set_size, int set_range_size=1<<20);
seqsvr::Router MakeTestRouter(const SetsConfig& sets);

// 测试用
//
// seqsvr集群配置信息
struct SeqSvrConfig : public nebula::Configurable {
  SeqSvrConfig() {
    // 默认两个set集
    // name = "cluster_seqsvr_test";
  }
  bool SetConf(const std::string& conf_name, const folly::dynamic& conf) override;
  
  // SetList sets;
  SetsConfig sets;
};

//struct AllocEntry {
//  int set_id;
//  int allocsvr_id;
//  std::string host;
//  uint16_t port;
//  int section_count;  // section数
//};
//
//struct RouterTable {
//  std::vector<AllocEntry> alloc_nodes_;
//};

#endif


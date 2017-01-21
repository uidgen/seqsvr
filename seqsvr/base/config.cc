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

#include "base/config.h"

/*
{
  "name": "cluster_seqsvr_test";
  "seqsvr": {[
    { "range": { "id": 0, "size": 10000000}
      "allocsvrs" : [
        { "addr": { "host" : "127.0.0.1", "port" : 10000 },
          "ranges": [ {"id" : 0, "size" : 5000000 } ]
        },
        { "addr": { "host" : "127.0.0.1", "port" : 10001 },
          "ranges": [ {"id" : 5000000, "size" : 5000000 } ]
        }
      ]
    },
    { "range": { "id": 10000000, "size": 10000000}
      "allocsvrs" : [
        { "addr": { "host" : "127.0.0.1", "port" : 10002 },
          "ranges": [ {"id" : 100000000, "size" : 15000000 } ]
        },
        { "addr": { "host" : "127.0.0.1", "port" : 10003 },
          "ranges": [ {"id" : 150000000, "size" : 20000000 } ]
        }
      ]
    },
  ]}
}
 */

// 测试用seqsvr
// 2个set
// 每个set2个alloc_svr
SetClusterConfig MakeTestClusterConfig() {
  SetClusterConfig config;
  config.name = "cluster_seqsvr_test";
  
  int alloc_id = 1;
  int set_id = 1;
  int port = 10000;
  // int alloc_config_range_id = 0;
  
  for (int i=0; i<2; ++i) {
    SetConfig set_config;
    
    // set1
    set_config.set_id = set_id++;
    set_config.range.id = i*10000000;
    set_config.range.size = 10000000;
    
    for (int j=0; j<2; j++) {
      AllocConfig alloc_config;

      IDRange range;
      // alloc_1
      alloc_config.alloc_id = alloc_id++;
      alloc_config.addr.host = "127.0.0.1";
      alloc_config.addr.port = port++;
      range.id = i*10000000+j*5000000;
      range.size = 5000000;
      alloc_config.ranges.push_back(range);
      set_config.allocs.push_back(alloc_config);

      config.sets_.push_back(set_config);
    }
    config.sets_.push_back(set_config);
  }

  return config
}

bool SeqSvrConfig::SetConf(const std::string& conf_name, const folly::dynamic& conf) {
  return false;
}


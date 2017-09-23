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

#include "base/set.h"
#include "base/thrift_util.h"

SetsConfig MakeTestSetsConfig(int set_size, int set_range_size) {
  SetsConfig sets;

  for (int i=0; i< set_size; ++i) {
    SetConfig set;
    set.id = i*set_range_size;
    set.size = set_range_size;
    for (int j=0; j<3; ++j) {
      set.allocsvrs.emplace_back("127.0.0.1", 10000+i*3+j);
    }
    for (int j=0; j<3; ++j) {
      set.storesvrs.emplace_back("127.0.0.1", 11000+i*3+j);
    }
    
    sets.push_back(set);
  }
  
  return sets;
}

seqsvr::Router MakeTestRouter(const SetsConfig& sets) {
  seqsvr::Router table;
  table.version = 1;

  for (auto& s : sets) {
    // 计算每个alloc管理的号段范围
    auto per_alloc_range_sz = CalcSetSectionSize(s.size, s.allocsvrs.size()*kPerSectionIdSize);

    LOG(INFO) << "allocsvrs_size = " << s.allocsvrs.size()
        << ", s.size = " << s.size
        << ", per_alloc_range_sz = " << per_alloc_range_sz;
    for (size_t i=0; i<s.allocsvrs.size(); ++i) {
      seqsvr::RangeID range;
      range.id_begin = s.id + i*(per_alloc_range_sz*kPerSectionIdSize);
      if (i+1 ==s.allocsvrs.size()){
        // 最后一个值可能不是per_alloc_range_sz*kPerSectionIdSize的倍数
        range.size = s.size % (per_alloc_range_sz*kPerSectionIdSize);
      } else {
        range.size = per_alloc_range_sz*kPerSectionIdSize;
      }

      seqsvr::RouterNode node;
      node.section_ranges.push_back(range);
      node.node_addr.ip = s.allocsvrs[i].addr;
      node.node_addr.port = s.allocsvrs[i].port;

      table.node_list.push_back(node);
    }
  }

  return table;
}

bool SeqSvrConfig::SetConf(const std::string& conf_name, const folly::dynamic& conf) {
  try {
    sets = folly::convertTo<SetsConfig>(conf);
  } catch (std::runtime_error& e) {
    LOG(ERROR) << "convertTo<SetsConfig> error: " << e.what();
    return false;
  }
  
  return true;
}


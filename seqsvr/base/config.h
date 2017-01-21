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

#include <string>
#include <vector>

#include "base/set.h"

#include "nebula/base/configurable.h"
#include "nebula/base/string_builder.h"


// 测试用
//
// seqsvr集群配置信息
struct SeqSvrConfig : public nebula::Configurable {
  SeqSvrConfig() {
    // 默认两个set集
    // name = "cluster_seqsvr_test";
  }
  bool SetConf(const std::string& conf_name, const folly::dynamic& conf) override;
  
  SetList sets;
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

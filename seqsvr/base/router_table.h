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

#ifndef SEQSVR_BASE_ROUTER_TABLE_H_
#define SEQSVR_BASE_ROUTER_TABLE_H_

#include <string>
#include <vector>

#include "base/set.h"

struct AllocEntry {
  int set_id;
  int allocsvr_id;
  std::string host;
  uint16_t port;
  int section_count;  // section数
};

struct RouterTable {
  std::vector<AllocEntry> alloc_nodes_;
};

#endif

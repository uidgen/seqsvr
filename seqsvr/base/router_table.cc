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

#include "base/router_table.h"

#include <folly/Format.h>
#include <folly/Conv.h>

#include "nebula/base/time_util.h"

void RouteTable::ParseFromRouter(const zproto::Router& router) {
  Clear();
  
  version_ = router.version();
  
  for (int i=0; i<router.sets_size(); ++i) {
    const auto& s = router.sets(i);

    Set set;
    
    set.set_id = i;
    set.set_name = s.set_name();
    set.range.id = s.range().id();
    set.range.size = s.range().size();
    
    for (int j=0; j<s.allocs_size(); ++j) {
      const auto& a = s.allocs(j);

      AllocEntry alloc_entry;

      alloc_entry.alloc_name = a.alloc_name();
      alloc_entry.alloc_id = (int64_t)set.set_id >> 32 | j;
      alloc_entry.addr.host = a.addr().host();
      alloc_entry.addr.port = a.addr().port();

      for (int k=0; k<a.ranges_size(); ++k) {
        auto& r =a.ranges(k);
        
        IDRange range;
        range.id = r.id();
        range.size = r.size();
        alloc_entry.ranges.push_back(range);
      }
      set.allocs.push_back(alloc_entry);
    }
    sets_.push_back(set);
  }
}

void RouteTable::SerializeToRouter(zproto::Router* router) const {
  router->set_version(version_);
  
  for (auto& set : sets_) {
    auto s = router->add_sets();
    s->set_set_name(set.set_name);
    auto s_range = s->mutable_range();
    s_range->set_id(set.range.id);
    s_range->set_size(set.range.size);
    for (auto & alloc_node : set.allocs) {
      auto a = s->add_allocs();
      a->set_alloc_name(alloc_node.alloc_name);
      auto a_addr = a->mutable_addr();
      a_addr->set_host(alloc_node.addr.host);
      a_addr->set_port(alloc_node.addr.port);
      for (auto& range : alloc_node.ranges) {
        auto r = a->add_ranges();
        r->set_id(range.id);
        r->set_size(range.size);
      }
    }
  }
}

void RouteTable::MakeTestRouteTable(RouteTable& table) {
  table.set_version(Now());
  int alloc_id = 0;
  int set_id = 0;
  int port = 10000;
  // int alloc_config_range_id = 0;
  
  for (int i=0; i<2; ++i) {
    Set set;
    
    // set1
    set.set_id = set_id++;
    set.range.id = i*10000000;
    set.range.size = 10000000;
    set.set_name = std::string("set_cluster_" + folly::to<std::string>(set_id));

    for (int j=0; j<2; j++) {
      AllocEntry alloc_config;
      
      IDRange range;
      // alloc_1
      alloc_config.alloc_id = (int64_t) set_id>>32 | alloc_id++;
      alloc_config.addr.host = "127.0.0.1";
      alloc_config.addr.port = port++;
      alloc_config.alloc_name = std::string("alloc_server:") + folly::to<std::string>(alloc_config.addr.port);
      range.id = i*10000000+j*5000000;
      range.size = 5000000;
      alloc_config.ranges.push_back(range);
      set.allocs.push_back(alloc_config);
    }
    table.sets_.push_back(set);
  }
}

AllocEntry* RouteTable::LookupAlloc(const std::string& set_name, const std::string& alloc_name) {
  AllocEntry* alloc_entry = nullptr;
  
  for (auto& set : sets_) {
    if (set.set_name == set_name) {
      for (auto & alloc_node : set.allocs) {
        if (alloc_node.alloc_name == alloc_name) {
          alloc_entry = &alloc_node;
          break;
        }
      }
    }
  }
  
  return alloc_entry;
}

////////////////////////////////////////////////////////////////////////////////////
void RouteSearchTable::Initialize(RouteTable& route_table) {
  table_.clear();
  
  for (auto& set : route_table.sets_) {
    for (auto & alloc_node : set.allocs) {
      for (auto& range : alloc_node.ranges) {
        IDRangeEntry node;
        node.range = range;
        node.alloc = &alloc_node;
        table_.push_back(node);
      }
    }
  }
}

bool RouteSearchTable::CheckSectionID(AllocID alloc_id, uint32_t section_id) {
  for(auto& v : table_) {
    if (section_id >= v.range.id && section_id < v.range.id + v.range.size) {
      if (alloc_id == v.alloc->alloc_id) {
        return true;
      }
    }
  }
  
  return false;
}


////////////////////////////////////////////////////////////////////////////////////
void RouteTableManager::Initialize(RouteTable& table) {
  route_table_.Swap(table);
  route_search_table_.Initialize(route_table_);
}


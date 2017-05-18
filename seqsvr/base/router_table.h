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

// 分配set集合

#include "nebula/base/string_builder.h"
#include "base/set.h"
#include "proto/cc/seqsvr.pb.h"

/*
 set: id(1~10240)
 set: 从0～100000
 */

//struct Section {
//  uint32_t begin;     // 节点开始
//};

typedef uint32_t SectionID; // 以第一个id作为SectionID

typedef int64_t AllocID;
typedef int32_t SetID;

struct SvrAddr {
  std::string host;   // host
  uint16_t port;      // port
};

// 号段
struct IDRange {
  uint32_t id;
  size_t   size;
};

// AllocSvr配置信息
struct AllocEntry {
  AllocID alloc_id;
  SvrAddr addr;
  std::string alloc_name;
  std::vector<IDRange> ranges;
};

// set配置信息
struct Set {
  SetID set_id;                      // set_id
  std::string set_name;
  std::vector<AllocEntry> allocs;    // set集合
  IDRange range;                     // 号段范围
};

// class RouteSearchTable;
// seq集群配置信息
class RouteTable {
public:
  RouteTable() = default;
  ~RouteTable() = default;
  
  static void MakeTestRouteTable(RouteTable& table);

  uint32_t version() const {
    return version_;
  }
  
  inline void set_version(uint32_t v) {
    version_ = v;
  }
  
  void Swap(RouteTable& o) {
    std::swap(version_, o.version_);
    sets_.swap(o.sets_);
  }
  
  // 序列化和反序列化
  void ParseFromRouter(const zproto::Router& router);
  void SerializeToRouter(zproto::Router* router) const;

  std::string ToString() const {
    CStringBuilder sb;
    return sb.ToString();
  }
  
  AllocEntry* LookupAlloc(const std::string& set_name, const std::string& alloc_name);
  
private:
  void Clear() {
    version_ = 0;
    sets_.clear();
  }
  
  friend class RouteSearchTable;
  uint32_t version_{0};
  // std::string name_;     // 服务名，可能会有多个seqsvr提供服务，为统一管理，提供一个唯一名字
  std::vector<Set> sets_;   // set集合，1个集群分配的set
};

// > = <
struct IDRangeEntry {
  IDRange range;
  AllocEntry* alloc{nullptr};
};

class RouteSearchTable {
public:
  RouteSearchTable() = default;
  ~RouteSearchTable() = default;
  
  void Initialize(RouteTable& route_table);
  
  bool CheckSectionID(AllocID alloc_id, uint32_t section_id);
  
private:
  std::vector<IDRangeEntry> table_;
};

////////////////////////////////////////////////////////////////////////////////////
class RouteTableManager {
public:
  RouteTableManager() = default;
  ~RouteTableManager() = default;
  
  // 初始化
  void Initialize(RouteTable& table);
  
private:
  RouteTable route_table_;
  RouteSearchTable route_search_table_;
};

#endif

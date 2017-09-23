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
#include "proto/gen-cpp2/seqsvr_types.h"
#include "nebula/net/base/ip_addr_info.h"

// ## 容灾2.0架构：嵌入式路由表容灾
// - 既然Client端与AllocSvr存在路由状态不一致的问题，
//   那么让AllocSvr把当前的路由状态传递给Client端，
//   打破之前只能根据本地Client配置文件做路由决策的限制，从根本上解决这个问题。
//
// - 所以在2.0架构中，我们把AllocSvr的路由状态嵌入到Client请求sequence的响应包中，
//   在不带来额外的资源消耗的情况下，实现了Client端与AllocSvr之间的路由状态一致。
//
// ## 具体实现方案如下：
//
// seqsvr所有模块使用了统一的路由表，描述了uid号段到AllocSvr的全映射。
// 这份路由表由仲裁服务根据AllocSvr的服务状态生成，写到StoreSvr中，
// 由AllocSvr当作租约读出，最后在业务返回包里旁路给Client端。
//
// 路由表优化：
// 1. Client根据本地共享内存缓存的路由表，选择对应的AllocSvr；
//    如果路由表不存在，随机选择一台AllocSvr
//
// 2. 对选中的AllocSvr发起请求，请求带上本地路由表的版本号
//
// 3. AllocSvr收到请求，除了处理sequence逻辑外，
//    判断Client带上版本号是否最新，如果是旧版则在响应包中附上最新的路由表
//
// 4. Client收到响应包，除了处理sequence逻辑外，判断响应包是否带有新路由表。
//    如果有，更新本地路由表，并决策是否返回第1步重试
/*
 set: id(1~10240)
 set: 从0～100000
 */

//struct Section {
//  uint32_t begin;     // 节点开始
//};

// json配置格式的路由表
/*
{
  "version":1,
  "sets": [
    "set_id":1,
    "set_name":"0-100",
    "range": {
      "id":0,
      "size":10
    },
    "allocs": [
      "alloc_id":,
      "addr":,
      "alloc_name":,
      "ranges":[
      ],
    ]
  ]
}
*/

//////////////////////////////////////////////////////////////////////////////
// 路由表关注的是AllocSvr服务节点
// 从客户端视角看：
//   从uid找到号段section，再从section找到AllocSvr
// 从构建路由表视角看：
//   由AllocSvr找到分配的Section，构成一个列表
// 可以设计如下数据结构
//

/*
// 大部分情况，一个AllocSvr里的号段大部分是连续的
// 为了减少网络传输量，将连续的号段使用SectionRange进行压缩
// 例如，1～10个号段，
// id_begin: 1, size: 10
struct SectionRange {
  uint32_t id_begin;  // 号段起始地址
  uint32_t size;      // 有多少个号段
};

// 路由节点
struct RouterNode {
  IpAddrInfo node_addr;               // 节点地址
  std::vector<SectionRange> section_ranges;  // 本节点管理的所有号段
};

// 客户端角度看的路由表：
struct RouterTable {
  uint32_t version;
  // std::list<RouterNode> node_list; // 整个集群所有allocsvr节点
};
 */

/////////////////////////////////////////////////////////////////////////
// 路由搜索表，客户端用
class RouteSearchTable {
public:
  RouteSearchTable() = default;
  ~RouteSearchTable() = default;
  
  void OnRouterTable() {}
};


/////////////////////////////////////////////////////////////////////////
// 从构建路由表视角看，要实现的功能：
// 1. 由配置文件创建路由表
// 2. 节点下线时将号段迁移到其它节点
// 3. 节点上线时，将部分节点迁移到新上线的节点中
// 每个节点的路由表
class RouterTableManager {
public:
  RouterTableManager() = default;
  ~RouterTableManager() = default;
  
  /////////////////////////////////////////////////////////////////////////
  // 构建测试用路由表
  static void MakeTestRouteTable(seqsvr::Router& table);
  
  /////////////////////////////////////////////////////////////////////////
  // 获取版本号
  inline uint32_t GetVersion() const {
    return version_;
  }
  
  // 从配置文件加载所有路由表
  void Initialize(const std::list<seqsvr::SetNodeInfo>& node_list);

  // AllocSvr节点上线和下线
  void OnAllocSvrRegister(const seqsvr::SetNodeInfo& node);
  void OnAllocSvrUnRegister(const seqsvr::SetNodeInfo& node);
  
  // 转换成客户端用路由表
  void ToRouterTable(seqsvr::Router& table) const;
  
private:
  uint32_t version_ {0};
  std::list<seqsvr::PerSetRouterTable> per_set_table_list_; // 整个集群所有allocsvr节点
};

/*
class RouterTable {
public:
  
private:
  uint32_t version;
  std::vector<PerSetRouterTable> table;
  
  friend class RouteSearchTable;
};
*/

/*
// 以如下号段举一个case
// 假设用户号段为1～16，有2个set，每个set里分配了2个section，每个section有4个号段
//
// |0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|
// |sec_0  |sec_1  |sec_2    |sec_3      |
// |    set_0      |    set_1            |
//

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
  void ParseFromRouter(const seqsvr::Router& router);
  void SerializeToRouter(seqsvr::Router* router) const;

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
*/

#endif

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

/**
 * The first thing to know about are types. The available types in Thrift are:
 *
 *  bool        Boolean, one byte
 *  byte        Signed byte
 *  i16         Signed 16-bit integer
 *  i32         Signed 32-bit integer
 *  i64         Signed 64-bit integer
 *  double      64-bit floating point value
 *  string      String
 *  binary      Blob (byte array)
 *  map<t1,t2>  Map from one type to another
 *  list<t1>    Ordered list of one type
 *  set<t1>     Set of unique elements of one type
 *
 * Did you also notice that Thrift supports C style comments?
 */

namespace cpp2 seqsvr

////////////////////////////////////////////////////////////////////////////////////
// 路由表
/*
struct IpPort {
  1: string str;
  2: i32 port;
}

struct Range {
  1: i32 id;
  2: i32 size;
}

struct AllocSvrEntry {
  1: string alloc_name;
  2: IpPort addr;
  3: list<Range> ranges;
}

struct SetEntry {
  1: string set_name;
  2: list<AllocSvrEntry> allocs;    // set集合
  3: Range range;                 // 号段范围
}

struct Router {
  1: i32 version;
  // string name = 2;      // 服务名，可能会有多个seqsvr提供服务，为统一管理，提供一个唯一名字
  2: list<SetEntry> sets;  // set集合，1个集群分配的set
}
*/

//////////////////////////////////////////////////////////////////////////////
// 路由表关注的是AllocSvr服务节点
// 从网络传输视角看：
//   只要传输整个路由列表，设计的数据结构要求紧凑，传输数据量小
//

struct NodeAddrInfo {
  1: string ip;
  2: i32 port;
}

struct RangeID {
  1: i32 id_begin;    // 起始地址
  2: i32 size;        // 大小
}

// 大部分情况，一个AllocSvr里的号段大部分是连续的
// 为了减少网络传输量，将连续的号段使用SectionRange进行压缩
// 例如，1～10个号段，
// id_begin: 1, size: 10
//struct SectionRange {
//  1: i32 id_begin;  // 号段起始地址
//  2: i32 size;      // 有多少个号段
//}

// 路由Alloc节点
struct RouterNode {
  1: NodeAddrInfo node_addr;         // 节点地址
  2: list<RangeID> section_ranges;   // 本节点管理的所有号段
}

//////////////////////////////////////////////////////////////////////////////
// 客户端眼里的路由表：
struct Router {
  1: i32 version;
  2: list<RouterNode> node_list;    // 整个集群所有allocsvr节点
}

//////////////////////////////////////////////////////////////////////////////
// 仲裁服务眼里的路由表
//
// 附带set信息的节点，
// 1. 从配置文件中加载
// 2. Alloc节点上下线
struct SetNodeInfo {
  1: RangeID set_id;
  2: NodeAddrInfo node_addr;
}

struct SetNodeInfoList {
  1: RangeID set_id;
  2: list<NodeAddrInfo> node_addrs; // set里分配了多少AllocSvr
}
// 一个set中的路由表
struct PerSetRouterTable {
  1: RangeID set_id;
  2: list<RouterNode> node_list;
}

//////////////////////////////////////////////////////////////////////////////
// AllocSvr
struct Sequence {
  // 1. 无法提供服务
  // 2. id不存在，仅返回路由表
  // 3. id存在，返回sequence，路由表未更新
  // 4. id存在，返回sequence，路由表需要更新
  // int32  result = 1;

  1: i64 sequence;
  2: optional Router router;
}

// struct IdSeq {
//   1: i32 id;
//   2: i64 sequence;
// }

//////////////////////////////////////////////////////////////////////////////
// StoreSvr
struct MaxSeqsData {
  1: RangeID set_id;       // set_id
  2: list<i64> max_seqs;   // seqs_data
}

service AllocService {
  // 获取下一个Seq
  Sequence FetchNextSequence(1: i32 id, 2: i32 version);
  // 查询当前Seq
  Sequence GetCurrentSequence(1: i32 id, 2: i32 version);

  // FetchNextSequenceListReq -> SequenceListRsp
  // list<IdSeq> FetchNextSequenceList(1: list<i32> id_list);
  // list<IdSeq> GetCurrentSequenceList(1: list<i32> id_list);
}

////////////////////////////////////////////////////////////////////////////////////
// StoreSvr
service StoreService {
  // 加载max_seqs数据
  MaxSeqsData LoadMaxSeqsData();
  // SaveMaxSeq
  i64 SaveMaxSeq(1: i32 id, 2: i64 max_seq);
  // 拉取路由表
  Router LoadRouteTable();

  // 接收仲裁服务生成的路由表
  bool SaveRouteTable(1: Router router);
}

////////////////////////////////////////////////////////////////////////////////////
// 仲裁服务
service MediateService {
  void RegisterAllocSvr(1: NodeAddrInfo node_addr);
  void UnRegisterAllocSvr(1: NodeAddrInfo node_addr);
  
  // 当前暂不实现AllocSvr探测，提供一个api接口手动更新StoreSvr里的路由表
  // void UpdateRouteTable(1: Router router);
}


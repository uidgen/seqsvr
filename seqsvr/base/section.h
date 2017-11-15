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

#ifndef SEQSVR_SECTION_H_
#define SEQSVR_SECTION_H_

// #include <folly/Range.h>

#include <stdint.h>

#include <folly/Range.h>
#include <folly/Format.h>

#define DEBUG_TEST

#ifndef DEBUG_TEST
// 生产环境，按uint32_t整个空间来分
const uint32_t kMaxIDSize = 0xffffffff;              // uint32_t范围的id
#else
// 测试时，为了在开发机上能快速启动、运行和调试
const uint32_t kMaxIDSize = 1<<20;                   // 1MB
#endif

// ## 预分配中间层
// - sequence只要求递增，并没有要求连续，也就是说出现一大段跳跃是允许的
// （例如分配出的sequence序列：1,2,3,10,100,101）。
// 于是我们实现了一个简单优雅的策略：
//  1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
//  2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：如果cur_seq > max_seq，
//     将分配上限提升一个步长max_seq += step，并持久化max_seq
//  3. 重启时，读出持久化的max_seq，赋值给cur_seq
//
// - 实际应用中每次提升的步长为10000
const uint64_t kSeqStep = 10000;

// ## 分号段共享存储
// > 重启时要读取大量的max_seq数据加载到内存中。
// - uid相邻的一段用户属于一个号段，而同个号段内的用户共享一个max_seq，
//   这样大幅减少了max_seq数据的大小，同时也降低了IO次数。
//
// - 目前seqsvr一个Section包含10万个uid，max_seq数据只有300+KB
//   一个Section包含10万个uid，一个uint32_t空间共有42950个Section
const uint32_t kPerSectionIdSize = 100000;

// 整个uid空间内有多少个section
const uint32_t kMaxSectionSize = (kMaxIDSize%kPerSectionIdSize==0) ?
                                 (kMaxIDSize/kPerSectionIdSize) :
                                 (kMaxIDSize/kPerSectionIdSize)+1;

// 整个uid空间总计占用section内存
// 号段为uint64_t
const uint32_t kMaxSectionMemSize = kMaxSectionSize<<3;

// 整个系统又按uid范围进行分Set，每个Set都是一个完整的、独立的StoreSvr+AllocSvr子系统。
// 分Set设计目的是为了做灾难隔离，一个Set出现故障只会影响该Set内的用户，而不会影响到其它用户
//
// @benqi: 实际部署时，按实际服务能力需配置Set数，Set最好由配置文件来定义
//
//
// set标识符
// id_begin为set第一个id
//
// 号段一个简单例子
// 假设用户号段为1～16，有2个set，每个set里分配了2个section，每个section有4个号段
//
// |0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|
// |sec_0  |sec_4  |sec_8    |sec_12     |
// |    set_0      |    set_8            |
//
// 约定，任一服务所有的set_idx, section_idx, id都是基于整个号段空间的
//

// ID范围
struct RangeID {
  uint32_t id_begin;   // 整个空间
  uint32_t size;        // set集合里整个空间大小
};

// section标识符
// id_begin为section里第一个id
typedef RangeID SectionID;

typedef RangeID SetID;      // set标识符

// 计算出set里可以分配多少号段
inline uint32_t CalcSetSectionSize(uint32_t range_size,
    uint32_t per_range_size = kPerSectionIdSize) {
  auto m = range_size % per_range_size;
  auto sz = range_size / per_range_size;
  return m==0 ? sz : sz + 1;
}

// 计算出set里可以分配多少号段
inline uint32_t CalcSetSectionSize(SetID set_id,
    uint32_t per_range_size = kPerSectionIdSize) {
  return CalcSetSectionSize(set_id.size, per_range_size);
}

// 检查id是否在当前set里
inline bool CheckIDByRange(RangeID range, uint32_t id) {
  return id>=range.id_begin && id<range.id_begin+range.size;
}

// 存储里会用到
// 由id计算出属于set里哪个section
inline std::pair<bool, uint32_t> CalcSectionID(RangeID range, uint32_t id) {
  if (!CheckIDByRange(range, id)) {
    return std::make_pair(false, 0);
  }
  
  return std::make_pair(true, (id-range.id_begin) / kPerSectionIdSize);
}

inline std::pair<bool, uint32_t> CalcSectionID(uint32_t begin, uint32_t size, uint32_t id) {
  RangeID range{begin, size};
  
  if (!CheckIDByRange(range, id)) {
    return std::make_pair(false, 0);
  }
  
  return std::make_pair(true, (id-range.id_begin) / kPerSectionIdSize);
}

// Section只关注是第几个section
struct Section {
  Section(uint32_t id_begin, uint64_t* id_max_seq)
    : section_id(id_begin),
      max_seq(id_max_seq) {
    
    // 命名：section_name
    folly::format(section_name, "section_{}_{}", kMaxSectionSize, id_begin);
  }
  
  //////////////////////////////////////////////////////////////////////////
  // section里分配的第一个id做为section_id
  uint32_t section_id;
  // section_name命名规则：section_begin(id)_end(id+slot_size-1)
  std::string section_name;
  // section里的max_seq值
  uint64_t* max_seq {nullptr};
  
  // 一个section分配了多少个ID
  // 不能放到配置文件里，因为是全局共享的，如果配置文件写错了
  // static uint32_t kIDSize;
};

#endif

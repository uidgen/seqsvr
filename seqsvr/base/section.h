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

#define DEBUG_TEST

#ifndef DEBUG_TEST
// 生产环境
const uint32_t kMaxIDSize = 0xffffffff;              // uint32_t范围的id
// const uint32_t kSetSize = 1;                      // 最少3个set
#else
// 为了在开发机上能快速启动、运行和调试
const uint32_t kMaxIDSize = 1<<20;                   // 1MB
// const uint32_t kSetSize = 1;                      // 1个set
#endif

const uint64_t kMaxIDMemSize = kMaxIDSize<<3;                           // uint32_t范围的id
const uint32_t kSectionSize = 100000;                                   // 一个Section包含10万个uid，一个uint32_t空间共有42950个Section
const uint32_t kSectionSlotSize = (kMaxIDSize/kSectionSize)+1;          // 一个Section包含10万个uid，一个uint32_t空间共有42950个Section
const uint32_t kSectionSlotMemSize = kSectionSlotSize<<3;               // 一个Section包含10万个uid，一个uint32_t空间共有42950个Section
const uint64_t kMaxSeqStep = 10000;               // 步长为1万

//// 号段
//struct IDRange {
//  uint32_t id;
//  size_t   size;
//};
//
//struct IDRangeList {
//  
//};

struct Section {
  uint32_t id_bgein;  // 当前section首id
};

struct SectionSet {
  
};

#if 0
// 由id-->set-->cur_seq->max_seq

// max_seq最小单位
class Section {
public:
  Section() = default;
  ~Section() = default;
  
  void Initialize(uint64_t max_seq, uint64_t* cur_seqs);
  
  uint64_t GetCurrentSeq(uint32_t id);
  uint64_t GetNextSeq(uint32_t id);
  
  // 触发max_seq事件
  uint64_t OnMaxSeq();
  
private:
  // uint32_t section_idx_;
  uint64_t max_seq_;
  folly::Range<uint64_t*> cur_seqs_;
};
#endif


#endif

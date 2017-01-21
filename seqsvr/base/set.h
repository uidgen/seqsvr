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

#ifndef SEQSVR_SET_H_
#define SEQSVR_SET_H_

#include "base/section.h"

#ifndef DEBUG_TEST
// 生产环境
const uint32_t kSetSize = 1;                      // 最少3个set
const uint32_t kSetAllocSize = 1;                 // 1个set分配n个allocsvr
#else
// 为了在开发机上能快速启动、运行和调试
const uint32_t kSetSize = 1;                      // 1个set
const uint32_t kSetAllocSize = 1;                 // 1个set分配n个allocsvr
#endif

//struct SvrAddr {
//  std::string host;   // host
//  uint16_t port;      // port
//};

//struct Set {
//  uint32_t set_id;        // set唯一ID，由运维配置
//  uint32_t id_begin;      // set集中第一个id
//  uint32_t size;          // 整个set管理的id值
//};

#if 0
// 初始化，通过配置获取
// void Initialize(uint32_t set_id=1, uint32_t alloc_id=1);
// 当前服务分配给哪个set
// uint32_t GetSetID();
// 当前Alloc模块
// uint32_t GetSetAllocID();

// 单服务Set单位
class SetUnit {
public:
  SetUnit();
  ~SetUnit();
  
  bool Initialize(uint32_t set_id=1, uint32_t alloc_id=1);
  
private:
  // 当前序号
  std::vector<uint64_t> cur_seqs_;
  std::vector<uint64_t> section_max_seqs_;
  std::vector<Section>  sections_;
};
#endif

#endif

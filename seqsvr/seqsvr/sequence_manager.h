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

#ifndef SEQSVR_SEQUENCE_MANAGER_H_
#define SEQSVR_SEQUENCE_MANAGER_H_

#include <mutex>

#include <folly/MemoryMapping.h>
#include <folly/Singleton.h>

#define DEBUG_TEST

#ifdef DEBUG_TEST
// 为了在开发机上能快速启动、运行和调试
#define SECTION_BITS_SIZE     12            //
#define SECTION_SIZE          0xfff         //
#define SECTION_SLOT_SIZE     0xfff         // 2^12
#define SECTION_SLOT_MEM_SIZE 0xfff<<3      // 2^12*8
#define MAX_UID_SIZE          0xffffff      // 2^24
#define MAX_UID_MEM_SIZE      0xffffff<<3   // 2^24
#else
#define SECTION_BITS_SIZE     17            //
#define SECTION_SIZE          0x1ffff       //
#define SECTION_SLOT_SIZE     0x7fff        // 2^15
#define SECTION_SLOT_MEM_SIZE 0x7fff<<3     // 2^15*8
#define MAX_UID_SIZE          0xffffffff    // 2^32
#define MAX_UID_MEM_SIZE      0xffffffff<<3 // 2^32*8

#endif

#define MAX_SEQ_STEP          10000

// TODO(@benqi): 单机模拟set的allocsvr和storesvr
class SequenceManager {
public:
  ~SequenceManager();

  static std::shared_ptr<SequenceManager> GetInstance();
  
  void Initialize(const std::string& filepath);
  
  uint64_t GetCurrentSequence(uint32_t uid);
  uint64_t FetchNextSequence(uint32_t uid);
  
private:
  SequenceManager() = default;
  friend class folly::Singleton<SequenceManager>;
  
  int section_fd_ {-1};
  uint64_t* section_max_seqs_ {nullptr};
  uint64_t* cur_seqs_ {nullptr};
  
  folly::MemoryMapping* section_max_seqs_mapping_ {nullptr};
  folly::Range<uint64_t*> mapping_mem_;
  
  std::mutex mutex_;
};

#endif

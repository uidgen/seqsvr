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

#ifndef BASE_SET_H_
#define BASE_SET_H_

#include "base/section.h"

// SectionSet
class Set {
public:
  Set(SetID _set_id, folly::Range<uint64_t*> data)
  : set_id(_set_id),
  set_max_seqs_data(data) {
    set_max_seqs_data = data;
  }
  
  ~Set() {
    set_max_seqs_data.clear();
  }
  
  //////////////////////////////////////////////////////////////////////////
  std::string ToString() const {
    return folly::sformat("set_{}_{}", set_id.id_begin, set_id.size);
  }
  
  // 对外服务
  // void GetSectionSetData(std::string& data) const {
  //   data.assign((char*)set_max_seqs_data.data(), set_max_seqs_data.size()*sizeof(uint64_t));
  // }
  
  SetID GetID() const {
    return set_id;
  }
  
  folly::Range<uint64_t*> GetMaxSeqsData() const {
    return set_max_seqs_data;
  }
  
  uint64_t SetMaxSeq(uint32_t id, uint64_t max_seq);
  
  // uint64_t SetMaxSeq(uint32_t id, uint64_t max_seq);
  
private:
  //////////////////////////////////////////////////////////////////////////
  SetID set_id;
  folly::Range<uint64_t*> set_max_seqs_data;
};

#endif

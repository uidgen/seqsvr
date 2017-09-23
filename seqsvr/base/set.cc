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

#include "base/set.h"

#include "nebula/base/logger/glog_util.h"

uint64_t Set::SetMaxSeq(uint32_t id, uint64_t max_seq) {
  auto idx = CalcSectionID(set_id, id);
  if (!idx.first) {
    LOG(ERROR) << "SetSectionsData - max_seq invalid: local seq = "
      << id << ", req seq = " << max_seq
      << ", in set: " << ToString();
    
    return 0;
  }
  
  if (max_seq > set_max_seqs_data[idx.second]) {
    max_seq = (max_seq/kSeqStep+1)*kSeqStep;
    set_max_seqs_data[idx.second] = max_seq;
  }
  
  return max_seq;
}

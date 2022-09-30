// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Author: Benqi (wubenqi@gmail.com)
//

package seqsvr

import (
	"encoding/binary"

	"github.com/edsrzf/mmap-go"
)

type SliceInt64 struct {
	mmap.MMap
}

func (s SliceInt64) SetIdx(idx int, v int64) {
	binary.LittleEndian.PutUint64(s.MMap[idx<<3:], uint64(v))
	s.Flush()
}

func (s SliceInt64) GetIdx(idx int) int64 {
	return int64(binary.LittleEndian.Uint64(s.MMap[idx<<3:]))
}

func (s SliceInt64) Len() int {
	return len(s.MMap) / 8
}

type Set struct {
	SetId          SetID
	SetMaxSeqsData SliceInt64
}

func (s *Set) ID() SetID {
	return s.SetId
}

func (s *Set) SetMaxSeq(id int32, maxSeq int64) int64 {
	ok, idx := s.SetId.CalcSectionID(id)
	if !ok {
		//LOG(ERROR) << "SetSectionsData - max_seq invalid: local seq = "
		//<< id << ", req seq = " << max_seq
		//<< ", in set: " << ToString();
		return 0
	}

	v := s.SetMaxSeqsData.GetIdx(int(idx))
	if maxSeq > v {
		maxSeq = (maxSeq/SeqStep + 1) * SeqStep
		s.SetMaxSeqsData.SetIdx(int(idx), maxSeq)
	}

	return maxSeq
}

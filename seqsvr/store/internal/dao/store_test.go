// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Author: Benqi (wubenqi@gmail.com)
//

package dao

import (
	"fmt"
	"testing"

	"github.com/teamgram/seqsvr/proto/seqsvr"

	"github.com/gogo/protobuf/jsonpb"
)

func TestMustNewStoreManager(t *testing.T) {
	setId := seqsvr.SetID{
		IdBegin: 0,
		Size:    1024,
	}

	store := MustNewStoreManager(setId, "./data")
	store.SetSectionMaxSeq(0, 1)
	// store.SetSectionMaxSeq(1, 234)
	// store.SetSectionMaxSeq(2, 222)
	maxSeqs, _ := store.GetMaxSeqsData()
	data, _ := (&jsonpb.Marshaler{}).MarshalToString(maxSeqs)
	fmt.Println(data)
}

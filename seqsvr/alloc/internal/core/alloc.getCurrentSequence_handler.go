// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: teamgramio (teamgram.io@gmail.com)
//

package core

import (
	"github.com/teamgram/seqsvr/proto/seqsvr"
)

// AllocGetCurrentSequence
// alloc.getCurrentSequence id:int version:int = Sequence;
func (c *AllocCore) AllocGetCurrentSequence(in *seqsvr.TLAllocGetCurrentSequence) (*seqsvr.Sequence, error) {
	seq, err := c.svcCtx.Dao.AllocManager.GetCurrentSequence(in.Id, in.Version)
	if err != nil {
		c.Logger.Errorf("alloc.getCurrentSequence - error: %v", err)
		return nil, err
	}

	return seq, nil
}

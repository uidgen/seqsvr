/*
 * WARNING! All changes made in this file will be lost!
 * Created from 'scheme.tl' by 'mtprotoc'
 *
 * Copyright 2022 Teamgram Authors.
 *  All rights reserved.
 *
 * Author: teamgramio (teamgram.io@gmail.com)
 */

package service

import (
	"context"

	"github.com/teamgram/seqsvr/proto/seqsvr"
	"github.com/teamgram/seqsvr/seqsvr/alloc/internal/core"
)

// AllocFetchNextSequence
// alloc.fetchNextSequence id:int version:int = Sequence;
func (s *Service) AllocFetchNextSequence(ctx context.Context, request *seqsvr.TLAllocFetchNextSequence) (*seqsvr.Sequence, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("alloc.fetchNextSequence - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.AllocFetchNextSequence(request)
	if err != nil {
		return nil, err
	}

	c.Infof("alloc.fetchNextSequence - reply: %s", r.DebugString())
	return r, err
}

// AllocGetCurrentSequence
// alloc.getCurrentSequence id:int version:int = Sequence;
func (s *Service) AllocGetCurrentSequence(ctx context.Context, request *seqsvr.TLAllocGetCurrentSequence) (*seqsvr.Sequence, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("alloc.getCurrentSequence - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.AllocGetCurrentSequence(request)
	if err != nil {
		return nil, err
	}

	c.Infof("alloc.getCurrentSequence - reply: %s", r.DebugString())
	return r, err
}

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
	"github.com/teamgram/seqsvr/seqsvr/mediate/internal/core"
)

// MediateRegisterAllocSvr
// mediate.registerAllocSvr node_addr:NodeAddrInfo = Bool2;
func (s *Service) MediateRegisterAllocSvr(ctx context.Context, request *seqsvr.TLMediateRegisterAllocSvr) (*seqsvr.Bool2, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("mediate.registerAllocSvr - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.MediateRegisterAllocSvr(request)
	if err != nil {
		return nil, err
	}

	c.Infof("mediate.registerAllocSvr - reply: %s", r.DebugString())
	return r, err
}

// MediateUnRegisterAllocSvr
// mediate.unRegisterAllocSvr node_addr:NodeAddrInfo = Bool2;
func (s *Service) MediateUnRegisterAllocSvr(ctx context.Context, request *seqsvr.TLMediateUnRegisterAllocSvr) (*seqsvr.Bool2, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("mediate.unRegisterAllocSvr - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.MediateUnRegisterAllocSvr(request)
	if err != nil {
		return nil, err
	}

	c.Infof("mediate.unRegisterAllocSvr - reply: %s", r.DebugString())
	return r, err
}

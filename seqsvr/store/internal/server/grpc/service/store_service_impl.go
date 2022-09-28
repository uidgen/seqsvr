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
	"github.com/teamgram/seqsvr/seqsvr/store/internal/core"
)

// StoreLoadMaxSeqsData
// store.loadMaxSeqsData = MaxSeqsData;
func (s *Service) StoreLoadMaxSeqsData(ctx context.Context, request *seqsvr.TLStoreLoadMaxSeqsData) (*seqsvr.MaxSeqsData, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("store.loadMaxSeqsData - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.StoreLoadMaxSeqsData(request)
	if err != nil {
		return nil, err
	}

	c.Infof("store.loadMaxSeqsData - reply: %s", r.DebugString())
	return r, err
}

// StoreSaveMaxSeq
// store.saveMaxSeq id:int max_seq:long = Int64V;
func (s *Service) StoreSaveMaxSeq(ctx context.Context, request *seqsvr.TLStoreSaveMaxSeq) (*seqsvr.Int64V, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("store.saveMaxSeq - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.StoreSaveMaxSeq(request)
	if err != nil {
		return nil, err
	}

	c.Infof("store.saveMaxSeq - reply: %s", r.DebugString())
	return r, err
}

// StoreLoadRouteTable
// store.loadRouteTable = Router;
func (s *Service) StoreLoadRouteTable(ctx context.Context, request *seqsvr.TLStoreLoadRouteTable) (*seqsvr.Router, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("store.loadRouteTable - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.StoreLoadRouteTable(request)
	if err != nil {
		return nil, err
	}

	c.Infof("store.loadRouteTable - reply: %s", r.DebugString())
	return r, err
}

// StoreSaveRouteTable
// store.saveRouteTable router:Router = Bool2;
func (s *Service) StoreSaveRouteTable(ctx context.Context, request *seqsvr.TLStoreSaveRouteTable) (*seqsvr.Bool2, error) {
	c := core.New(ctx, s.svcCtx)
	c.Infof("store.saveRouteTable - metadata: %s, request: %s", c.MD.DebugString(), request.DebugString())

	r, err := c.StoreSaveRouteTable(request)
	if err != nil {
		return nil, err
	}

	c.Infof("store.saveRouteTable - reply: %s", r.DebugString())
	return r, err
}

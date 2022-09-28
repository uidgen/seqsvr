/*
 * WARNING! All changes made in this file will be lost!
 * Created from 'scheme.tl' by 'mtprotoc'
 *
 * Copyright 2022 Teamgram Authors.
 *  All rights reserved.
 *
 * Author: teamgramio (teamgram.io@gmail.com)
 */

package store_client

import (
	"context"

	"github.com/teamgram/seqsvr/proto/seqsvr"

	"github.com/zeromicro/go-zero/zrpc"
)

type StoreClient interface {
	StoreLoadMaxSeqsData(ctx context.Context, in *seqsvr.TLStoreLoadMaxSeqsData) (*seqsvr.MaxSeqsData, error)
	StoreSaveMaxSeq(ctx context.Context, in *seqsvr.TLStoreSaveMaxSeq) (*seqsvr.Int64V, error)
	StoreLoadRouteTable(ctx context.Context, in *seqsvr.TLStoreLoadRouteTable) (*seqsvr.Router, error)
	StoreSaveRouteTable(ctx context.Context, in *seqsvr.TLStoreSaveRouteTable) (*seqsvr.Bool2, error)
}

type defaultStoreClient struct {
	cli zrpc.Client
}

func NewStoreClient(cli zrpc.Client) StoreClient {
	return &defaultStoreClient{
		cli: cli,
	}
}

// StoreLoadMaxSeqsData
// store.loadMaxSeqsData = MaxSeqsData;
func (m *defaultStoreClient) StoreLoadMaxSeqsData(ctx context.Context, in *seqsvr.TLStoreLoadMaxSeqsData) (*seqsvr.MaxSeqsData, error) {
	client := seqsvr.NewRPCStoreClient(m.cli.Conn())
	return client.StoreLoadMaxSeqsData(ctx, in)
}

// StoreSaveMaxSeq
// store.saveMaxSeq id:int max_seq:long = Int64V;
func (m *defaultStoreClient) StoreSaveMaxSeq(ctx context.Context, in *seqsvr.TLStoreSaveMaxSeq) (*seqsvr.Int64V, error) {
	client := seqsvr.NewRPCStoreClient(m.cli.Conn())
	return client.StoreSaveMaxSeq(ctx, in)
}

// StoreLoadRouteTable
// store.loadRouteTable = Router;
func (m *defaultStoreClient) StoreLoadRouteTable(ctx context.Context, in *seqsvr.TLStoreLoadRouteTable) (*seqsvr.Router, error) {
	client := seqsvr.NewRPCStoreClient(m.cli.Conn())
	return client.StoreLoadRouteTable(ctx, in)
}

// StoreSaveRouteTable
// store.saveRouteTable router:Router = Bool2;
func (m *defaultStoreClient) StoreSaveRouteTable(ctx context.Context, in *seqsvr.TLStoreSaveRouteTable) (*seqsvr.Bool2, error) {
	client := seqsvr.NewRPCStoreClient(m.cli.Conn())
	return client.StoreSaveRouteTable(ctx, in)
}

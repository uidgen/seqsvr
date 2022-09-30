// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Author: Benqi (wubenqi@gmail.com)
//

package dao

import (
	"context"

	"github.com/teamgram/marmota/pkg/net/rpcx"
	"github.com/teamgram/seqsvr/proto/seqsvr"
	"github.com/teamgram/seqsvr/seqsvr/alloc/internal/config"
	store_client "github.com/teamgram/seqsvr/seqsvr/store/client"
)

var (
	_loadMaxSeqsDataReq = &seqsvr.TLStoreLoadMaxSeqsData{
		Constructor: seqsvr.CRC32_store_loadMaxSeqsData,
	}

	_loadRouteTableReq = &seqsvr.TLStoreLoadRouteTable{
		Constructor: seqsvr.CRC32_store_loadRouteTable,
	}
)

type StoreManager struct {
	store_client.StoreClient
}

func MustNewStoreManager(c config.Config) *StoreManager {
	return &StoreManager{
		StoreClient: store_client.NewStoreClient(rpcx.GetCachedRpcClient(c.StoreClient)),
	}
}

func (m *StoreManager) LoadMaxSeqsData(ctx context.Context) (*seqsvr.MaxSeqsData, error) {
	return m.StoreClient.StoreLoadMaxSeqsData(ctx, _loadMaxSeqsDataReq)
}

func (m *StoreManager) SaveMaxSeq(ctx context.Context, id int32, maxSeq int64) (int64, error) {
	nextSeq, err := m.StoreClient.StoreSaveMaxSeq(ctx, &seqsvr.TLStoreSaveMaxSeq{
		Constructor: seqsvr.CRC32_store_saveMaxSeq,
		Id:          id,
		MaxSeq:      maxSeq,
	})
	if err != nil {
		return 0, err
	}

	return nextSeq.GetV(), nil
}

// LoadRouteTable R=N
func (m *StoreManager) LoadRouteTable(ctx context.Context) (*seqsvr.Router, error) {
	return m.StoreClient.StoreLoadRouteTable(ctx, _loadRouteTableReq)
}

func (m *StoreManager) SaveRouteTable(ctx context.Context, router *seqsvr.Router) (bool, error) {
	rV, err := m.StoreSaveRouteTable(ctx, &seqsvr.TLStoreSaveRouteTable{
		Constructor: seqsvr.CRC32_store_saveRouteTable,
		Router:      router,
	})

	if err != nil {
		return false, err
	}

	return seqsvr.FromBool(rV), nil
}

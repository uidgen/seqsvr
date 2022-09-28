/*
 * WARNING! All changes made in this file will be lost!
 * Created from 'scheme.tl' by 'mtprotoc'
 *
 * Copyright 2022 Teamgram Authors.
 *  All rights reserved.
 *
 * Author: teamgramio (teamgram.io@gmail.com)
 */

package alloc_client

import (
	"context"

	"github.com/teamgram/seqsvr/proto/seqsvr"

	"github.com/zeromicro/go-zero/zrpc"
)

type AllocClient interface {
	AllocFetchNextSequence(ctx context.Context, in *seqsvr.TLAllocFetchNextSequence) (*seqsvr.Sequence, error)
	AllocGetCurrentSequence(ctx context.Context, in *seqsvr.TLAllocGetCurrentSequence) (*seqsvr.Sequence, error)
}

type defaultAllocClient struct {
	cli zrpc.Client
}

func NewAllocClient(cli zrpc.Client) AllocClient {
	return &defaultAllocClient{
		cli: cli,
	}
}

// AllocFetchNextSequence
// alloc.fetchNextSequence id:int version:int = Sequence;
func (m *defaultAllocClient) AllocFetchNextSequence(ctx context.Context, in *seqsvr.TLAllocFetchNextSequence) (*seqsvr.Sequence, error) {
	client := seqsvr.NewRPCAllocClient(m.cli.Conn())
	return client.AllocFetchNextSequence(ctx, in)
}

// AllocGetCurrentSequence
// alloc.getCurrentSequence id:int version:int = Sequence;
func (m *defaultAllocClient) AllocGetCurrentSequence(ctx context.Context, in *seqsvr.TLAllocGetCurrentSequence) (*seqsvr.Sequence, error) {
	client := seqsvr.NewRPCAllocClient(m.cli.Conn())
	return client.AllocGetCurrentSequence(ctx, in)
}

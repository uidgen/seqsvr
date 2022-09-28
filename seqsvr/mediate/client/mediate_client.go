/*
 * WARNING! All changes made in this file will be lost!
 * Created from 'scheme.tl' by 'mtprotoc'
 *
 * Copyright 2022 Teamgram Authors.
 *  All rights reserved.
 *
 * Author: teamgramio (teamgram.io@gmail.com)
 */

package mediate_client

import (
	"context"

	"github.com/teamgram/seqsvr/proto/seqsvr"

	"github.com/zeromicro/go-zero/zrpc"
)

type MediateClient interface {
	MediateRegisterAllocSvr(ctx context.Context, in *seqsvr.TLMediateRegisterAllocSvr) (*seqsvr.Bool2, error)
	MediateUnRegisterAllocSvr(ctx context.Context, in *seqsvr.TLMediateUnRegisterAllocSvr) (*seqsvr.Bool2, error)
}

type defaultMediateClient struct {
	cli zrpc.Client
}

func NewMediateClient(cli zrpc.Client) MediateClient {
	return &defaultMediateClient{
		cli: cli,
	}
}

// MediateRegisterAllocSvr
// mediate.registerAllocSvr node_addr:NodeAddrInfo = Bool2;
func (m *defaultMediateClient) MediateRegisterAllocSvr(ctx context.Context, in *seqsvr.TLMediateRegisterAllocSvr) (*seqsvr.Bool2, error) {
	client := seqsvr.NewRPCMediateClient(m.cli.Conn())
	return client.MediateRegisterAllocSvr(ctx, in)
}

// MediateUnRegisterAllocSvr
// mediate.unRegisterAllocSvr node_addr:NodeAddrInfo = Bool2;
func (m *defaultMediateClient) MediateUnRegisterAllocSvr(ctx context.Context, in *seqsvr.TLMediateUnRegisterAllocSvr) (*seqsvr.Bool2, error) {
	client := seqsvr.NewRPCMediateClient(m.cli.Conn())
	return client.MediateUnRegisterAllocSvr(ctx, in)
}

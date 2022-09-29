/*
 * WARNING! All changes made in this file will be lost!
 * Created from 'scheme.tl' by 'mtprotoc'
 *
 * Copyright 2022 Teamgram Authors.
 *  All rights reserved.
 *
 * Author: teamgramio (teamgram.io@gmail.com)
 */

package grpc

import (
	"github.com/teamgram/seqsvr/proto/seqsvr"
	"github.com/teamgram/seqsvr/seqsvr/mediate/internal/server/grpc/service"
	"github.com/teamgram/seqsvr/seqsvr/mediate/internal/svc"

	"github.com/zeromicro/go-zero/core/logx"
	"github.com/zeromicro/go-zero/zrpc"
	"google.golang.org/grpc"
)

// New new a grpc server.
func New(ctx *svc.ServiceContext, c zrpc.RpcServerConf) *zrpc.RpcServer {
	s, err := zrpc.NewServer(c, func(grpcServer *grpc.Server) {
		seqsvr.RegisterRPCMediateServer(grpcServer, service.New(ctx))
	})
	logx.Must(err)
	return s
}
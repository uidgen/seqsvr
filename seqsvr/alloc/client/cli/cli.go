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

package main

import (
	"context"
	"flag"
	"fmt"
	"log"

	"github.com/teamgram/seqsvr/proto/seqsvr"
	alloc_client "github.com/teamgram/seqsvr/seqsvr/alloc/client"

	"github.com/zeromicro/go-zero/core/discov"
	"github.com/zeromicro/go-zero/zrpc"
)

var (
	id   int
	next bool
)

func init() {
	flag.IntVar(&id, "id", 1, "id")
	flag.BoolVar(&next, "next", false, "next")
}

func main() {
	flag.Parse()

	c := zrpc.RpcClientConf{
		Etcd:      discov.EtcdConf{},
		Endpoints: []string{"127.0.0.1:10100"},
		Target:    "",
		App:       "",
		Token:     "",
		NonBlock:  false,
		Timeout:   0,
	}

	cli := alloc_client.NewAllocClient(zrpc.MustNewClient(c))
	if next {
		v, err := cli.AllocFetchNextSequence(context.TODO(), &seqsvr.TLAllocFetchNextSequence{
			Id:      int32(id),
			Version: 0,
		})
		if err != nil {
			fmt.Println("alloc.fetchNextSequence - error: ", err)
		} else {
			log.Printf("{id: %d}", v.Seq)
		}
	} else {
		v, err := cli.AllocGetCurrentSequence(context.TODO(), &seqsvr.TLAllocGetCurrentSequence{
			Id:      int32(id),
			Version: 0,
		})
		if err != nil {
			fmt.Println("alloc.getCurrentSequence - error: ", err)
		} else {
			log.Printf("{id: %d}", v.Seq)
		}
	}
}

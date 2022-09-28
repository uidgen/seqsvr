/*
 * WARNING! All changes made in this file will be lost!
 * Created from 'scheme.tl' by 'mtprotoc'
 *
 * Copyright (c) 2022-present,  Teamgram Authors.
 *  All rights reserved.
 *
 * Author: Benqi (wubenqi@gmail.com)
 */

package seqsvr

import (
	"reflect"
)

type newRPCReplyFunc func() interface{}

type RPCContextTuple struct {
	Method       string
	NewReplyFunc newRPCReplyFunc
}

var rpcContextRegisters = map[string]RPCContextTuple{
	"TLMediateRegisterAllocSvr":   RPCContextTuple{"/seqsvr.RPCMediate/mediate_registerAllocSvr", func() interface{} { return new(Bool2) }},
	"TLMediateUnRegisterAllocSvr": RPCContextTuple{"/seqsvr.RPCMediate/mediate_unRegisterAllocSvr", func() interface{} { return new(Bool2) }},

	"TLAllocFetchNextSequence":  RPCContextTuple{"/seqsvr.RPCAlloc/alloc_fetchNextSequence", func() interface{} { return new(Sequence) }},
	"TLAllocGetCurrentSequence": RPCContextTuple{"/seqsvr.RPCAlloc/alloc_getCurrentSequence", func() interface{} { return new(Sequence) }},

	"TLStoreLoadMaxSeqsData": RPCContextTuple{"/seqsvr.RPCStore/store_loadMaxSeqsData", func() interface{} { return new(MaxSeqsData) }},
	"TLStoreSaveMaxSeq":      RPCContextTuple{"/seqsvr.RPCStore/store_saveMaxSeq", func() interface{} { return new(Int64V) }},
	"TLStoreLoadRouteTable":  RPCContextTuple{"/seqsvr.RPCStore/store_loadRouteTable", func() interface{} { return new(Router) }},
	"TLStoreSaveRouteTable":  RPCContextTuple{"/seqsvr.RPCStore/store_saveRouteTable", func() interface{} { return new(Bool2) }},
}

func FindRPCContextTuple(t interface{}) *RPCContextTuple {
	rt := reflect.TypeOf(t)
	if rt.Kind() == reflect.Ptr {
		rt = rt.Elem()
	}

	m, ok := rpcContextRegisters[rt.Name()]
	if !ok {
		// log.Errorf("Can't find name: %s", rt.Name())
		return nil
	}
	return &m
}

func GetRPCContextRegisters() map[string]RPCContextTuple {
	return rpcContextRegisters
}

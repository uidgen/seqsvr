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

const (
	Predicate_int32V                     = "int32V"
	Predicate_int64V                     = "int64V"
	Predicate_boolFalse2                 = "boolFalse2"
	Predicate_boolTrue2                  = "boolTrue2"
	Predicate_void                       = "void"
	Predicate_nodeAddrInfo               = "nodeAddrInfo"
	Predicate_rangeId                    = "rangeId"
	Predicate_routerNode                 = "routerNode"
	Predicate_router                     = "router"
	Predicate_setNodeInfo                = "setNodeInfo"
	Predicate_setNodeInfoList            = "setNodeInfoList"
	Predicate_perSetRouterTable          = "perSetRouterTable"
	Predicate_sequence                   = "sequence"
	Predicate_maxSeqsData                = "maxSeqsData"
	Predicate_alloc_fetchNextSequence    = "alloc_fetchNextSequence"
	Predicate_alloc_getCurrentSequence   = "alloc_getCurrentSequence"
	Predicate_store_loadMaxSeqsData      = "store_loadMaxSeqsData"
	Predicate_store_saveMaxSeq           = "store_saveMaxSeq"
	Predicate_store_loadRouteTable       = "store_loadRouteTable"
	Predicate_store_saveRouteTable       = "store_saveRouteTable"
	Predicate_mediate_registerAllocSvr   = "mediate_registerAllocSvr"
	Predicate_mediate_unRegisterAllocSvr = "mediate_unRegisterAllocSvr"
)

var clazzNameRegisters2 = map[string]map[int]int32{
	Predicate_int32V: {
		0: -816401434, // 0xcf56b3e6

	},
	Predicate_int64V: {
		0: 1560852317, // 0x5d08b75d

	},
	Predicate_boolFalse2: {
		0: -1551878720, // 0xa38035c0

	},
	Predicate_boolTrue2: {
		0: -466523818, // 0xe4316956

	},
	Predicate_void: {
		0: 852168407, // 0x32cb0ed7

	},
	Predicate_nodeAddrInfo: {
		0: 1327779405, // 0x4f244e4d

	},
	Predicate_rangeId: {
		0: 753683407, // 0x2cec4bcf

	},
	Predicate_routerNode: {
		0: -494247970, // 0xe28a5fde

	},
	Predicate_router: {
		0: -1308968200, // 0xb1fabaf8

	},
	Predicate_setNodeInfo: {
		0: -2003449992, // 0x8895c778

	},
	Predicate_setNodeInfoList: {
		0: 260619155, // 0xf88bb93

	},
	Predicate_perSetRouterTable: {
		0: -720815251, // 0xd5093b6d

	},
	Predicate_sequence: {
		0: 1719366935, // 0x667b7517

	},
	Predicate_maxSeqsData: {
		0: 1841312951, // 0x6dc034b7

	},
	Predicate_alloc_fetchNextSequence: {
		0: -580368098, // 0xdd68491e

	},
	Predicate_alloc_getCurrentSequence: {
		0: 1382426801, // 0x526628b1

	},
	Predicate_store_loadMaxSeqsData: {
		0: -540991348, // 0xdfc1208c

	},
	Predicate_store_saveMaxSeq: {
		0: 95451525, // 0x5b07985

	},
	Predicate_store_loadRouteTable: {
		0: -857598577, // 0xcce2158f

	},
	Predicate_store_saveRouteTable: {
		0: 1038739636, // 0x3de9e8b4

	},
	Predicate_mediate_registerAllocSvr: {
		0: -1497386444, // 0xa6bfb234

	},
	Predicate_mediate_unRegisterAllocSvr: {
		0: 1637414201, // 0x6198f539

	},
}

var clazzIdNameRegisters2 = map[int32]string{
	-816401434:  Predicate_int32V,                     // 0xcf56b3e6
	1560852317:  Predicate_int64V,                     // 0x5d08b75d
	-1551878720: Predicate_boolFalse2,                 // 0xa38035c0
	-466523818:  Predicate_boolTrue2,                  // 0xe4316956
	852168407:   Predicate_void,                       // 0x32cb0ed7
	1327779405:  Predicate_nodeAddrInfo,               // 0x4f244e4d
	753683407:   Predicate_rangeId,                    // 0x2cec4bcf
	-494247970:  Predicate_routerNode,                 // 0xe28a5fde
	-1308968200: Predicate_router,                     // 0xb1fabaf8
	-2003449992: Predicate_setNodeInfo,                // 0x8895c778
	260619155:   Predicate_setNodeInfoList,            // 0xf88bb93
	-720815251:  Predicate_perSetRouterTable,          // 0xd5093b6d
	1719366935:  Predicate_sequence,                   // 0x667b7517
	1841312951:  Predicate_maxSeqsData,                // 0x6dc034b7
	-580368098:  Predicate_alloc_fetchNextSequence,    // 0xdd68491e
	1382426801:  Predicate_alloc_getCurrentSequence,   // 0x526628b1
	-540991348:  Predicate_store_loadMaxSeqsData,      // 0xdfc1208c
	95451525:    Predicate_store_saveMaxSeq,           // 0x5b07985
	-857598577:  Predicate_store_loadRouteTable,       // 0xcce2158f
	1038739636:  Predicate_store_saveRouteTable,       // 0x3de9e8b4
	-1497386444: Predicate_mediate_registerAllocSvr,   // 0xa6bfb234
	1637414201:  Predicate_mediate_unRegisterAllocSvr, // 0x6198f539

}

func GetClazzID(clazzName string, layer int) int32 {
	if m, ok := clazzNameRegisters2[clazzName]; ok {
		m2, ok2 := m[layer]
		if ok2 {
			return m2
		}
		m2, ok2 = m[0]
		if ok2 {
			return m2
		}
	}
	return 0
}

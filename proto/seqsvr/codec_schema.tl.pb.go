/*
 * WARNING! All changes made in this file will be lost!
 * Created from 'scheme.tl' by 'mtprotoc'
 *
 * Copyright (c) 2022-present,  Teamgram Authors.
 *  All rights reserved.
 *
 * Author: Benqi (wubenqi@gmail.com)
 */

// ConstructorList
// RequestList

package seqsvr

import (
	"fmt"

	"github.com/gogo/protobuf/jsonpb"
	"github.com/gogo/protobuf/types"
)

//////////////////////////////////////////////////////////////////////////////////////////

var _ *types.Int32Value
var _ fmt.GoStringer

var clazzIdRegisters2 = map[int32]func() TLObject{
	// Constructor
	-1551878720: func() TLObject { // 0xa38035c0
		o := MakeTLBoolFalse2(nil)
		o.Data2.Constructor = -1551878720
		return o
	},
	-466523818: func() TLObject { // 0xe4316956
		o := MakeTLBoolTrue2(nil)
		o.Data2.Constructor = -466523818
		return o
	},
	-816401434: func() TLObject { // 0xcf56b3e6
		o := MakeTLInt32V(nil)
		o.Data2.Constructor = -816401434
		return o
	},
	1560852317: func() TLObject { // 0x5d08b75d
		o := MakeTLInt64V(nil)
		o.Data2.Constructor = 1560852317
		return o
	},
	1841312951: func() TLObject { // 0x6dc034b7
		o := MakeTLMaxSeqsData(nil)
		o.Data2.Constructor = 1841312951
		return o
	},
	1327779405: func() TLObject { // 0x4f244e4d
		o := MakeTLNodeAddrInfo(nil)
		o.Data2.Constructor = 1327779405
		return o
	},
	-720815251: func() TLObject { // 0xd5093b6d
		o := MakeTLPerSetRouterTable(nil)
		o.Data2.Constructor = -720815251
		return o
	},
	753683407: func() TLObject { // 0x2cec4bcf
		o := MakeTLRangeId(nil)
		o.Data2.Constructor = 753683407
		return o
	},
	-1308968200: func() TLObject { // 0xb1fabaf8
		o := MakeTLRouter(nil)
		o.Data2.Constructor = -1308968200
		return o
	},
	-494247970: func() TLObject { // 0xe28a5fde
		o := MakeTLRouterNode(nil)
		o.Data2.Constructor = -494247970
		return o
	},
	1719366935: func() TLObject { // 0x667b7517
		o := MakeTLSequence(nil)
		o.Data2.Constructor = 1719366935
		return o
	},
	-2003449992: func() TLObject { // 0x8895c778
		o := MakeTLSetNodeInfo(nil)
		o.Data2.Constructor = -2003449992
		return o
	},
	260619155: func() TLObject { // 0xf88bb93
		o := MakeTLSetNodeInfoList(nil)
		o.Data2.Constructor = 260619155
		return o
	},
	852168407: func() TLObject { // 0x32cb0ed7
		o := MakeTLVoid(nil)
		o.Data2.Constructor = 852168407
		return o
	},

	// Method
	-580368098: func() TLObject { // 0xdd68491e
		return &TLAllocFetchNextSequence{
			Constructor: -580368098,
		}
	},
	1382426801: func() TLObject { // 0x526628b1
		return &TLAllocGetCurrentSequence{
			Constructor: 1382426801,
		}
	},
	-540991348: func() TLObject { // 0xdfc1208c
		return &TLStoreLoadMaxSeqsData{
			Constructor: -540991348,
		}
	},
	95451525: func() TLObject { // 0x5b07985
		return &TLStoreSaveMaxSeq{
			Constructor: 95451525,
		}
	},
	-857598577: func() TLObject { // 0xcce2158f
		return &TLStoreLoadRouteTable{
			Constructor: -857598577,
		}
	},
	1038739636: func() TLObject { // 0x3de9e8b4
		return &TLStoreSaveRouteTable{
			Constructor: 1038739636,
		}
	},
	-1497386444: func() TLObject { // 0xa6bfb234
		return &TLMediateRegisterAllocSvr{
			Constructor: -1497386444,
		}
	},
	1637414201: func() TLObject { // 0x6198f539
		return &TLMediateUnRegisterAllocSvr{
			Constructor: 1637414201,
		}
	},
}

func NewTLObjectByClassID(classId int32) TLObject {
	f, ok := clazzIdRegisters2[classId]
	if !ok {
		return nil
	}
	return f()
}

func CheckClassID(classId int32) (ok bool) {
	_, ok = clazzIdRegisters2[classId]
	return
}

//----------------------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Bool2 <--
//  + TL_BoolFalse2
//  + TL_BoolTrue2
//

func (m *Bool2) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_boolFalse2:
		t := m.To_BoolFalse2()
		xBuf = t.Encode(layer)
	case Predicate_boolTrue2:
		t := m.To_BoolTrue2()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *Bool2) CalcByteSize(layer int32) int {
	return 0
}

func (m *Bool2) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0xa38035c0:
		m2 := MakeTLBoolFalse2(m)
		m2.Decode(dBuf)
	case 0xe4316956:
		m2 := MakeTLBoolTrue2(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *Bool2) DebugString() string {
	switch m.PredicateName {
	case Predicate_boolFalse2:
		t := m.To_BoolFalse2()
		return t.DebugString()
	case Predicate_boolTrue2:
		t := m.To_BoolTrue2()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_BoolFalse2
func (m *Bool2) To_BoolFalse2() *TLBoolFalse2 {
	m.PredicateName = Predicate_boolFalse2
	return &TLBoolFalse2{
		Data2: m,
	}
}

// To_BoolTrue2
func (m *Bool2) To_BoolTrue2() *TLBoolTrue2 {
	m.PredicateName = Predicate_boolTrue2
	return &TLBoolTrue2{
		Data2: m,
	}
}

// MakeTLBoolFalse2
func MakeTLBoolFalse2(data2 *Bool2) *TLBoolFalse2 {
	if data2 == nil {
		return &TLBoolFalse2{Data2: &Bool2{
			PredicateName: Predicate_boolFalse2,
		}}
	} else {
		data2.PredicateName = Predicate_boolFalse2
		return &TLBoolFalse2{Data2: data2}
	}
}

func (m *TLBoolFalse2) To_Bool2() *Bool2 {
	m.Data2.PredicateName = Predicate_boolFalse2
	return m.Data2
}

func (m *TLBoolFalse2) GetPredicateName() string {
	return Predicate_boolFalse2
}

func (m *TLBoolFalse2) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0xa38035c0: func() []byte {
			x.UInt(0xa38035c0)

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_boolFalse2, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_boolFalse2, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLBoolFalse2) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLBoolFalse2) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0xa38035c0: func() error {
			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLBoolFalse2) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// MakeTLBoolTrue2
func MakeTLBoolTrue2(data2 *Bool2) *TLBoolTrue2 {
	if data2 == nil {
		return &TLBoolTrue2{Data2: &Bool2{
			PredicateName: Predicate_boolTrue2,
		}}
	} else {
		data2.PredicateName = Predicate_boolTrue2
		return &TLBoolTrue2{Data2: data2}
	}
}

func (m *TLBoolTrue2) To_Bool2() *Bool2 {
	m.Data2.PredicateName = Predicate_boolTrue2
	return m.Data2
}

func (m *TLBoolTrue2) GetPredicateName() string {
	return Predicate_boolTrue2
}

func (m *TLBoolTrue2) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0xe4316956: func() []byte {
			x.UInt(0xe4316956)

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_boolTrue2, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_boolTrue2, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLBoolTrue2) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLBoolTrue2) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0xe4316956: func() error {
			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLBoolTrue2) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// Int32V <--
//  + TL_Int32V
//

func (m *Int32V) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_int32V:
		t := m.To_Int32V()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *Int32V) CalcByteSize(layer int32) int {
	return 0
}

func (m *Int32V) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0xcf56b3e6:
		m2 := MakeTLInt32V(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *Int32V) DebugString() string {
	switch m.PredicateName {
	case Predicate_int32V:
		t := m.To_Int32V()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_Int32V
func (m *Int32V) To_Int32V() *TLInt32V {
	m.PredicateName = Predicate_int32V
	return &TLInt32V{
		Data2: m,
	}
}

// MakeTLInt32V
func MakeTLInt32V(data2 *Int32V) *TLInt32V {
	if data2 == nil {
		return &TLInt32V{Data2: &Int32V{
			PredicateName: Predicate_int32V,
		}}
	} else {
		data2.PredicateName = Predicate_int32V
		return &TLInt32V{Data2: data2}
	}
}

func (m *TLInt32V) To_Int32V() *Int32V {
	m.Data2.PredicateName = Predicate_int32V
	return m.Data2
}

func (m *TLInt32V) SetV(v int32) { m.Data2.V = v }
func (m *TLInt32V) GetV() int32  { return m.Data2.V }

func (m *TLInt32V) GetPredicateName() string {
	return Predicate_int32V
}

func (m *TLInt32V) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0xcf56b3e6: func() []byte {
			x.UInt(0xcf56b3e6)

			x.Int(m.GetV())
			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_int32V, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_int32V, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLInt32V) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLInt32V) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0xcf56b3e6: func() error {
			m.SetV(dBuf.Int())
			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLInt32V) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// Int64V <--
//  + TL_Int64V
//

func (m *Int64V) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_int64V:
		t := m.To_Int64V()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *Int64V) CalcByteSize(layer int32) int {
	return 0
}

func (m *Int64V) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0x5d08b75d:
		m2 := MakeTLInt64V(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *Int64V) DebugString() string {
	switch m.PredicateName {
	case Predicate_int64V:
		t := m.To_Int64V()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_Int64V
func (m *Int64V) To_Int64V() *TLInt64V {
	m.PredicateName = Predicate_int64V
	return &TLInt64V{
		Data2: m,
	}
}

// MakeTLInt64V
func MakeTLInt64V(data2 *Int64V) *TLInt64V {
	if data2 == nil {
		return &TLInt64V{Data2: &Int64V{
			PredicateName: Predicate_int64V,
		}}
	} else {
		data2.PredicateName = Predicate_int64V
		return &TLInt64V{Data2: data2}
	}
}

func (m *TLInt64V) To_Int64V() *Int64V {
	m.Data2.PredicateName = Predicate_int64V
	return m.Data2
}

func (m *TLInt64V) SetV(v int64) { m.Data2.V = v }
func (m *TLInt64V) GetV() int64  { return m.Data2.V }

func (m *TLInt64V) GetPredicateName() string {
	return Predicate_int64V
}

func (m *TLInt64V) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0x5d08b75d: func() []byte {
			x.UInt(0x5d08b75d)

			x.Long(m.GetV())
			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_int64V, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_int64V, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLInt64V) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLInt64V) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0x5d08b75d: func() error {
			m.SetV(dBuf.Long())
			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLInt64V) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// MaxSeqsData <--
//  + TL_MaxSeqsData
//

func (m *MaxSeqsData) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_maxSeqsData:
		t := m.To_MaxSeqsData()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *MaxSeqsData) CalcByteSize(layer int32) int {
	return 0
}

func (m *MaxSeqsData) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0x6dc034b7:
		m2 := MakeTLMaxSeqsData(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *MaxSeqsData) DebugString() string {
	switch m.PredicateName {
	case Predicate_maxSeqsData:
		t := m.To_MaxSeqsData()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_MaxSeqsData
func (m *MaxSeqsData) To_MaxSeqsData() *TLMaxSeqsData {
	m.PredicateName = Predicate_maxSeqsData
	return &TLMaxSeqsData{
		Data2: m,
	}
}

// MakeTLMaxSeqsData
func MakeTLMaxSeqsData(data2 *MaxSeqsData) *TLMaxSeqsData {
	if data2 == nil {
		return &TLMaxSeqsData{Data2: &MaxSeqsData{
			PredicateName: Predicate_maxSeqsData,
		}}
	} else {
		data2.PredicateName = Predicate_maxSeqsData
		return &TLMaxSeqsData{Data2: data2}
	}
}

func (m *TLMaxSeqsData) To_MaxSeqsData() *MaxSeqsData {
	m.Data2.PredicateName = Predicate_maxSeqsData
	return m.Data2
}

func (m *TLMaxSeqsData) SetSetId(v *RangeId) { m.Data2.SetId = v }
func (m *TLMaxSeqsData) GetSetId() *RangeId  { return m.Data2.SetId }

func (m *TLMaxSeqsData) SetMaxSeqs(v []int64) { m.Data2.MaxSeqs = v }
func (m *TLMaxSeqsData) GetMaxSeqs() []int64  { return m.Data2.MaxSeqs }

func (m *TLMaxSeqsData) GetPredicateName() string {
	return Predicate_maxSeqsData
}

func (m *TLMaxSeqsData) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0x6dc034b7: func() []byte {
			x.UInt(0x6dc034b7)

			x.Bytes(m.GetSetId().Encode(layer))

			x.VectorLong(m.GetMaxSeqs())

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_maxSeqsData, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_maxSeqsData, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLMaxSeqsData) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLMaxSeqsData) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0x6dc034b7: func() error {

			m0 := &RangeId{}
			m0.Decode(dBuf)
			m.SetSetId(m0)

			m.SetMaxSeqs(dBuf.VectorLong())

			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLMaxSeqsData) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// NodeAddrInfo <--
//  + TL_NodeAddrInfo
//

func (m *NodeAddrInfo) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_nodeAddrInfo:
		t := m.To_NodeAddrInfo()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *NodeAddrInfo) CalcByteSize(layer int32) int {
	return 0
}

func (m *NodeAddrInfo) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0x4f244e4d:
		m2 := MakeTLNodeAddrInfo(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *NodeAddrInfo) DebugString() string {
	switch m.PredicateName {
	case Predicate_nodeAddrInfo:
		t := m.To_NodeAddrInfo()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_NodeAddrInfo
func (m *NodeAddrInfo) To_NodeAddrInfo() *TLNodeAddrInfo {
	m.PredicateName = Predicate_nodeAddrInfo
	return &TLNodeAddrInfo{
		Data2: m,
	}
}

// MakeTLNodeAddrInfo
func MakeTLNodeAddrInfo(data2 *NodeAddrInfo) *TLNodeAddrInfo {
	if data2 == nil {
		return &TLNodeAddrInfo{Data2: &NodeAddrInfo{
			PredicateName: Predicate_nodeAddrInfo,
		}}
	} else {
		data2.PredicateName = Predicate_nodeAddrInfo
		return &TLNodeAddrInfo{Data2: data2}
	}
}

func (m *TLNodeAddrInfo) To_NodeAddrInfo() *NodeAddrInfo {
	m.Data2.PredicateName = Predicate_nodeAddrInfo
	return m.Data2
}

func (m *TLNodeAddrInfo) SetIp(v string) { m.Data2.Ip = v }
func (m *TLNodeAddrInfo) GetIp() string  { return m.Data2.Ip }

func (m *TLNodeAddrInfo) SetPort(v int32) { m.Data2.Port = v }
func (m *TLNodeAddrInfo) GetPort() int32  { return m.Data2.Port }

func (m *TLNodeAddrInfo) GetPredicateName() string {
	return Predicate_nodeAddrInfo
}

func (m *TLNodeAddrInfo) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0x4f244e4d: func() []byte {
			x.UInt(0x4f244e4d)

			x.String(m.GetIp())
			x.Int(m.GetPort())
			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_nodeAddrInfo, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_nodeAddrInfo, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLNodeAddrInfo) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLNodeAddrInfo) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0x4f244e4d: func() error {
			m.SetIp(dBuf.String())
			m.SetPort(dBuf.Int())
			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLNodeAddrInfo) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// PerSetRouterTable <--
//  + TL_PerSetRouterTable
//

func (m *PerSetRouterTable) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_perSetRouterTable:
		t := m.To_PerSetRouterTable()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *PerSetRouterTable) CalcByteSize(layer int32) int {
	return 0
}

func (m *PerSetRouterTable) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0xd5093b6d:
		m2 := MakeTLPerSetRouterTable(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *PerSetRouterTable) DebugString() string {
	switch m.PredicateName {
	case Predicate_perSetRouterTable:
		t := m.To_PerSetRouterTable()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_PerSetRouterTable
func (m *PerSetRouterTable) To_PerSetRouterTable() *TLPerSetRouterTable {
	m.PredicateName = Predicate_perSetRouterTable
	return &TLPerSetRouterTable{
		Data2: m,
	}
}

// MakeTLPerSetRouterTable
func MakeTLPerSetRouterTable(data2 *PerSetRouterTable) *TLPerSetRouterTable {
	if data2 == nil {
		return &TLPerSetRouterTable{Data2: &PerSetRouterTable{
			PredicateName: Predicate_perSetRouterTable,
		}}
	} else {
		data2.PredicateName = Predicate_perSetRouterTable
		return &TLPerSetRouterTable{Data2: data2}
	}
}

func (m *TLPerSetRouterTable) To_PerSetRouterTable() *PerSetRouterTable {
	m.Data2.PredicateName = Predicate_perSetRouterTable
	return m.Data2
}

func (m *TLPerSetRouterTable) SetSetId(v *RangeId) { m.Data2.SetId = v }
func (m *TLPerSetRouterTable) GetSetId() *RangeId  { return m.Data2.SetId }

func (m *TLPerSetRouterTable) SetNodeList(v []*RouterNode) { m.Data2.NodeList = v }
func (m *TLPerSetRouterTable) GetNodeList() []*RouterNode  { return m.Data2.NodeList }

func (m *TLPerSetRouterTable) GetPredicateName() string {
	return Predicate_perSetRouterTable
}

func (m *TLPerSetRouterTable) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0xd5093b6d: func() []byte {
			x.UInt(0xd5093b6d)

			x.Bytes(m.GetSetId().Encode(layer))

			x.Int(int32(CRC32_vector))
			x.Int(int32(len(m.GetNodeList())))
			for _, v := range m.GetNodeList() {
				x.Bytes((*v).Encode(layer))
			}

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_perSetRouterTable, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_perSetRouterTable, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLPerSetRouterTable) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLPerSetRouterTable) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0xd5093b6d: func() error {

			m0 := &RangeId{}
			m0.Decode(dBuf)
			m.SetSetId(m0)

			c1 := dBuf.Int()
			if c1 != int32(CRC32_vector) {
				// dBuf.err = fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
				return fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
			}
			l1 := dBuf.Int()
			v1 := make([]*RouterNode, l1)
			for i := int32(0); i < l1; i++ {
				v1[i] = &RouterNode{}
				v1[i].Decode(dBuf)
			}
			m.SetNodeList(v1)

			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLPerSetRouterTable) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// RangeId <--
//  + TL_RangeId
//

func (m *RangeId) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_rangeId:
		t := m.To_RangeId()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *RangeId) CalcByteSize(layer int32) int {
	return 0
}

func (m *RangeId) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0x2cec4bcf:
		m2 := MakeTLRangeId(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *RangeId) DebugString() string {
	switch m.PredicateName {
	case Predicate_rangeId:
		t := m.To_RangeId()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_RangeId
func (m *RangeId) To_RangeId() *TLRangeId {
	m.PredicateName = Predicate_rangeId
	return &TLRangeId{
		Data2: m,
	}
}

// MakeTLRangeId
func MakeTLRangeId(data2 *RangeId) *TLRangeId {
	if data2 == nil {
		return &TLRangeId{Data2: &RangeId{
			PredicateName: Predicate_rangeId,
		}}
	} else {
		data2.PredicateName = Predicate_rangeId
		return &TLRangeId{Data2: data2}
	}
}

func (m *TLRangeId) To_RangeId() *RangeId {
	m.Data2.PredicateName = Predicate_rangeId
	return m.Data2
}

func (m *TLRangeId) SetIdBegin(v int32) { m.Data2.IdBegin = v }
func (m *TLRangeId) GetIdBegin() int32  { return m.Data2.IdBegin }

func (m *TLRangeId) SetSize2(v int32) { m.Data2.Size2 = v }
func (m *TLRangeId) GetSize2() int32  { return m.Data2.Size2 }

func (m *TLRangeId) GetPredicateName() string {
	return Predicate_rangeId
}

func (m *TLRangeId) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0x2cec4bcf: func() []byte {
			x.UInt(0x2cec4bcf)

			x.Int(m.GetIdBegin())
			x.Int(m.GetSize2())
			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_rangeId, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_rangeId, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLRangeId) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLRangeId) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0x2cec4bcf: func() error {
			m.SetIdBegin(dBuf.Int())
			m.SetSize2(dBuf.Int())
			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLRangeId) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// Router <--
//  + TL_Router
//

func (m *Router) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_router:
		t := m.To_Router()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *Router) CalcByteSize(layer int32) int {
	return 0
}

func (m *Router) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0xb1fabaf8:
		m2 := MakeTLRouter(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *Router) DebugString() string {
	switch m.PredicateName {
	case Predicate_router:
		t := m.To_Router()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_Router
func (m *Router) To_Router() *TLRouter {
	m.PredicateName = Predicate_router
	return &TLRouter{
		Data2: m,
	}
}

// MakeTLRouter
func MakeTLRouter(data2 *Router) *TLRouter {
	if data2 == nil {
		return &TLRouter{Data2: &Router{
			PredicateName: Predicate_router,
		}}
	} else {
		data2.PredicateName = Predicate_router
		return &TLRouter{Data2: data2}
	}
}

func (m *TLRouter) To_Router() *Router {
	m.Data2.PredicateName = Predicate_router
	return m.Data2
}

func (m *TLRouter) SetVersion(v int32) { m.Data2.Version = v }
func (m *TLRouter) GetVersion() int32  { return m.Data2.Version }

func (m *TLRouter) SetNodeList(v []*RouterNode) { m.Data2.NodeList = v }
func (m *TLRouter) GetNodeList() []*RouterNode  { return m.Data2.NodeList }

func (m *TLRouter) GetPredicateName() string {
	return Predicate_router
}

func (m *TLRouter) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0xb1fabaf8: func() []byte {
			x.UInt(0xb1fabaf8)

			x.Int(m.GetVersion())

			x.Int(int32(CRC32_vector))
			x.Int(int32(len(m.GetNodeList())))
			for _, v := range m.GetNodeList() {
				x.Bytes((*v).Encode(layer))
			}

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_router, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_router, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLRouter) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLRouter) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0xb1fabaf8: func() error {
			m.SetVersion(dBuf.Int())
			c1 := dBuf.Int()
			if c1 != int32(CRC32_vector) {
				// dBuf.err = fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
				return fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
			}
			l1 := dBuf.Int()
			v1 := make([]*RouterNode, l1)
			for i := int32(0); i < l1; i++ {
				v1[i] = &RouterNode{}
				v1[i].Decode(dBuf)
			}
			m.SetNodeList(v1)

			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLRouter) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// RouterNode <--
//  + TL_RouterNode
//

func (m *RouterNode) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_routerNode:
		t := m.To_RouterNode()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *RouterNode) CalcByteSize(layer int32) int {
	return 0
}

func (m *RouterNode) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0xe28a5fde:
		m2 := MakeTLRouterNode(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *RouterNode) DebugString() string {
	switch m.PredicateName {
	case Predicate_routerNode:
		t := m.To_RouterNode()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_RouterNode
func (m *RouterNode) To_RouterNode() *TLRouterNode {
	m.PredicateName = Predicate_routerNode
	return &TLRouterNode{
		Data2: m,
	}
}

// MakeTLRouterNode
func MakeTLRouterNode(data2 *RouterNode) *TLRouterNode {
	if data2 == nil {
		return &TLRouterNode{Data2: &RouterNode{
			PredicateName: Predicate_routerNode,
		}}
	} else {
		data2.PredicateName = Predicate_routerNode
		return &TLRouterNode{Data2: data2}
	}
}

func (m *TLRouterNode) To_RouterNode() *RouterNode {
	m.Data2.PredicateName = Predicate_routerNode
	return m.Data2
}

func (m *TLRouterNode) SetNodeAddr(v *NodeAddrInfo) { m.Data2.NodeAddr = v }
func (m *TLRouterNode) GetNodeAddr() *NodeAddrInfo  { return m.Data2.NodeAddr }

func (m *TLRouterNode) SetSectionRanges(v []*RangeId) { m.Data2.SectionRanges = v }
func (m *TLRouterNode) GetSectionRanges() []*RangeId  { return m.Data2.SectionRanges }

func (m *TLRouterNode) GetPredicateName() string {
	return Predicate_routerNode
}

func (m *TLRouterNode) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0xe28a5fde: func() []byte {
			x.UInt(0xe28a5fde)

			x.Bytes(m.GetNodeAddr().Encode(layer))

			x.Int(int32(CRC32_vector))
			x.Int(int32(len(m.GetSectionRanges())))
			for _, v := range m.GetSectionRanges() {
				x.Bytes((*v).Encode(layer))
			}

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_routerNode, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_routerNode, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLRouterNode) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLRouterNode) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0xe28a5fde: func() error {

			m0 := &NodeAddrInfo{}
			m0.Decode(dBuf)
			m.SetNodeAddr(m0)

			c1 := dBuf.Int()
			if c1 != int32(CRC32_vector) {
				// dBuf.err = fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
				return fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
			}
			l1 := dBuf.Int()
			v1 := make([]*RangeId, l1)
			for i := int32(0); i < l1; i++ {
				v1[i] = &RangeId{}
				v1[i].Decode(dBuf)
			}
			m.SetSectionRanges(v1)

			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLRouterNode) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// Sequence <--
//  + TL_Sequence
//

func (m *Sequence) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_sequence:
		t := m.To_Sequence()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *Sequence) CalcByteSize(layer int32) int {
	return 0
}

func (m *Sequence) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0x667b7517:
		m2 := MakeTLSequence(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *Sequence) DebugString() string {
	switch m.PredicateName {
	case Predicate_sequence:
		t := m.To_Sequence()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_Sequence
func (m *Sequence) To_Sequence() *TLSequence {
	m.PredicateName = Predicate_sequence
	return &TLSequence{
		Data2: m,
	}
}

// MakeTLSequence
func MakeTLSequence(data2 *Sequence) *TLSequence {
	if data2 == nil {
		return &TLSequence{Data2: &Sequence{
			PredicateName: Predicate_sequence,
		}}
	} else {
		data2.PredicateName = Predicate_sequence
		return &TLSequence{Data2: data2}
	}
}

func (m *TLSequence) To_Sequence() *Sequence {
	m.Data2.PredicateName = Predicate_sequence
	return m.Data2
}

func (m *TLSequence) SetSeq(v int64) { m.Data2.Seq = v }
func (m *TLSequence) GetSeq() int64  { return m.Data2.Seq }

func (m *TLSequence) SetRouter(v *Router) { m.Data2.Router = v }
func (m *TLSequence) GetRouter() *Router  { return m.Data2.Router }

func (m *TLSequence) GetPredicateName() string {
	return Predicate_sequence
}

func (m *TLSequence) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0x667b7517: func() []byte {
			x.UInt(0x667b7517)

			x.Long(m.GetSeq())
			x.Bytes(m.GetRouter().Encode(layer))
			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_sequence, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_sequence, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLSequence) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLSequence) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0x667b7517: func() error {
			m.SetSeq(dBuf.Long())

			m1 := &Router{}
			m1.Decode(dBuf)
			m.SetRouter(m1)

			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLSequence) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// SetNodeInfo <--
//  + TL_SetNodeInfo
//

func (m *SetNodeInfo) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_setNodeInfo:
		t := m.To_SetNodeInfo()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *SetNodeInfo) CalcByteSize(layer int32) int {
	return 0
}

func (m *SetNodeInfo) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0x8895c778:
		m2 := MakeTLSetNodeInfo(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *SetNodeInfo) DebugString() string {
	switch m.PredicateName {
	case Predicate_setNodeInfo:
		t := m.To_SetNodeInfo()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_SetNodeInfo
func (m *SetNodeInfo) To_SetNodeInfo() *TLSetNodeInfo {
	m.PredicateName = Predicate_setNodeInfo
	return &TLSetNodeInfo{
		Data2: m,
	}
}

// MakeTLSetNodeInfo
func MakeTLSetNodeInfo(data2 *SetNodeInfo) *TLSetNodeInfo {
	if data2 == nil {
		return &TLSetNodeInfo{Data2: &SetNodeInfo{
			PredicateName: Predicate_setNodeInfo,
		}}
	} else {
		data2.PredicateName = Predicate_setNodeInfo
		return &TLSetNodeInfo{Data2: data2}
	}
}

func (m *TLSetNodeInfo) To_SetNodeInfo() *SetNodeInfo {
	m.Data2.PredicateName = Predicate_setNodeInfo
	return m.Data2
}

func (m *TLSetNodeInfo) SetSetId(v *RangeId) { m.Data2.SetId = v }
func (m *TLSetNodeInfo) GetSetId() *RangeId  { return m.Data2.SetId }

func (m *TLSetNodeInfo) SetNodeAddr(v *NodeAddrInfo) { m.Data2.NodeAddr = v }
func (m *TLSetNodeInfo) GetNodeAddr() *NodeAddrInfo  { return m.Data2.NodeAddr }

func (m *TLSetNodeInfo) GetPredicateName() string {
	return Predicate_setNodeInfo
}

func (m *TLSetNodeInfo) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0x8895c778: func() []byte {
			x.UInt(0x8895c778)

			x.Bytes(m.GetSetId().Encode(layer))
			x.Bytes(m.GetNodeAddr().Encode(layer))
			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_setNodeInfo, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_setNodeInfo, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLSetNodeInfo) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLSetNodeInfo) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0x8895c778: func() error {

			m0 := &RangeId{}
			m0.Decode(dBuf)
			m.SetSetId(m0)

			m1 := &NodeAddrInfo{}
			m1.Decode(dBuf)
			m.SetNodeAddr(m1)

			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLSetNodeInfo) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// SetNodeInfoList <--
//  + TL_SetNodeInfoList
//

func (m *SetNodeInfoList) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_setNodeInfoList:
		t := m.To_SetNodeInfoList()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *SetNodeInfoList) CalcByteSize(layer int32) int {
	return 0
}

func (m *SetNodeInfoList) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0xf88bb93:
		m2 := MakeTLSetNodeInfoList(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *SetNodeInfoList) DebugString() string {
	switch m.PredicateName {
	case Predicate_setNodeInfoList:
		t := m.To_SetNodeInfoList()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_SetNodeInfoList
func (m *SetNodeInfoList) To_SetNodeInfoList() *TLSetNodeInfoList {
	m.PredicateName = Predicate_setNodeInfoList
	return &TLSetNodeInfoList{
		Data2: m,
	}
}

// MakeTLSetNodeInfoList
func MakeTLSetNodeInfoList(data2 *SetNodeInfoList) *TLSetNodeInfoList {
	if data2 == nil {
		return &TLSetNodeInfoList{Data2: &SetNodeInfoList{
			PredicateName: Predicate_setNodeInfoList,
		}}
	} else {
		data2.PredicateName = Predicate_setNodeInfoList
		return &TLSetNodeInfoList{Data2: data2}
	}
}

func (m *TLSetNodeInfoList) To_SetNodeInfoList() *SetNodeInfoList {
	m.Data2.PredicateName = Predicate_setNodeInfoList
	return m.Data2
}

func (m *TLSetNodeInfoList) SetSetId(v *RangeId) { m.Data2.SetId = v }
func (m *TLSetNodeInfoList) GetSetId() *RangeId  { return m.Data2.SetId }

func (m *TLSetNodeInfoList) SetNodeAddrs(v []*NodeAddrInfo) { m.Data2.NodeAddrs = v }
func (m *TLSetNodeInfoList) GetNodeAddrs() []*NodeAddrInfo  { return m.Data2.NodeAddrs }

func (m *TLSetNodeInfoList) GetPredicateName() string {
	return Predicate_setNodeInfoList
}

func (m *TLSetNodeInfoList) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0xf88bb93: func() []byte {
			x.UInt(0xf88bb93)

			x.Bytes(m.GetSetId().Encode(layer))

			x.Int(int32(CRC32_vector))
			x.Int(int32(len(m.GetNodeAddrs())))
			for _, v := range m.GetNodeAddrs() {
				x.Bytes((*v).Encode(layer))
			}

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_setNodeInfoList, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_setNodeInfoList, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLSetNodeInfoList) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLSetNodeInfoList) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0xf88bb93: func() error {

			m0 := &RangeId{}
			m0.Decode(dBuf)
			m.SetSetId(m0)

			c1 := dBuf.Int()
			if c1 != int32(CRC32_vector) {
				// dBuf.err = fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
				return fmt.Errorf("invalid CRC32_vector, c%d: %d", 1, c1)
			}
			l1 := dBuf.Int()
			v1 := make([]*NodeAddrInfo, l1)
			for i := int32(0); i < l1; i++ {
				v1[i] = &NodeAddrInfo{}
				v1[i].Decode(dBuf)
			}
			m.SetNodeAddrs(v1)

			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLSetNodeInfoList) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

///////////////////////////////////////////////////////////////////////////////
// Void2 <--
//  + TL_Void
//

func (m *Void2) Encode(layer int32) []byte {
	predicateName := m.PredicateName
	if predicateName == "" {
		if n, ok := clazzIdNameRegisters2[int32(m.Constructor)]; ok {
			predicateName = n
		}
	}

	var (
		xBuf []byte
	)

	switch predicateName {
	case Predicate_void:
		t := m.To_Void()
		xBuf = t.Encode(layer)

	default:
		// logx.Errorf("invalid predicate error: %s",  m.PredicateName)
		return []byte{}
	}

	return xBuf
}

func (m *Void2) CalcByteSize(layer int32) int {
	return 0
}

func (m *Void2) Decode(dBuf *DecodeBuf) error {
	m.Constructor = TLConstructor(dBuf.Int())
	switch uint32(m.Constructor) {
	case 0x32cb0ed7:
		m2 := MakeTLVoid(m)
		m2.Decode(dBuf)

	default:
		return fmt.Errorf("invalid constructorId: 0x%x", uint32(m.Constructor))
	}
	return dBuf.GetError()
}

func (m *Void2) DebugString() string {
	switch m.PredicateName {
	case Predicate_void:
		t := m.To_Void()
		return t.DebugString()

	default:
		return "{}"
	}
}

// To_Void
func (m *Void2) To_Void() *TLVoid {
	m.PredicateName = Predicate_void
	return &TLVoid{
		Data2: m,
	}
}

// MakeTLVoid
func MakeTLVoid(data2 *Void2) *TLVoid {
	if data2 == nil {
		return &TLVoid{Data2: &Void2{
			PredicateName: Predicate_void,
		}}
	} else {
		data2.PredicateName = Predicate_void
		return &TLVoid{Data2: data2}
	}
}

func (m *TLVoid) To_Void2() *Void2 {
	m.Data2.PredicateName = Predicate_void
	return m.Data2
}

func (m *TLVoid) GetPredicateName() string {
	return Predicate_void
}

func (m *TLVoid) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)

	var encodeF = map[uint32]func() []byte{
		0x32cb0ed7: func() []byte {
			x.UInt(0x32cb0ed7)

			return x.GetBuf()
		},
	}

	clazzId := GetClazzID(Predicate_void, int(layer))
	if f, ok := encodeF[uint32(clazzId)]; ok {
		return f()
	} else {
		// TODO(@benqi): handle error
		// log.Errorf("not found clazzId by (%s, %d)", Predicate_void, layer)
		return x.GetBuf()
	}

	return x.GetBuf()
}

func (m *TLVoid) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLVoid) Decode(dBuf *DecodeBuf) error {
	var decodeF = map[uint32]func() error{
		0x32cb0ed7: func() error {
			return dBuf.GetError()
		},
	}

	if f, ok := decodeF[uint32(m.Data2.Constructor)]; ok {
		return f()
	} else {
		return fmt.Errorf("invalid constructor: %x", uint32(m.Data2.Constructor))
	}
}

func (m *TLVoid) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

//----------------------------------------------------------------------------------------------------------------
// TLAllocFetchNextSequence
///////////////////////////////////////////////////////////////////////////////

func (m *TLAllocFetchNextSequence) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_alloc_fetchNextSequence))

	switch uint32(m.Constructor) {
	case 0xdd68491e:
		x.UInt(0xdd68491e)

		// no flags

		x.Int(m.GetId())
		x.Int(m.GetVersion())

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLAllocFetchNextSequence) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLAllocFetchNextSequence) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0xdd68491e:

		// not has flags

		m.Id = dBuf.Int()
		m.Version = dBuf.Int()
		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLAllocFetchNextSequence) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// TLAllocGetCurrentSequence
///////////////////////////////////////////////////////////////////////////////

func (m *TLAllocGetCurrentSequence) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_alloc_getCurrentSequence))

	switch uint32(m.Constructor) {
	case 0x526628b1:
		x.UInt(0x526628b1)

		// no flags

		x.Int(m.GetId())
		x.Int(m.GetVersion())

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLAllocGetCurrentSequence) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLAllocGetCurrentSequence) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0x526628b1:

		// not has flags

		m.Id = dBuf.Int()
		m.Version = dBuf.Int()
		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLAllocGetCurrentSequence) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// TLStoreLoadMaxSeqsData
///////////////////////////////////////////////////////////////////////////////

func (m *TLStoreLoadMaxSeqsData) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_store_loadMaxSeqsData))

	switch uint32(m.Constructor) {
	case 0xdfc1208c:
		x.UInt(0xdfc1208c)

		// no flags

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLStoreLoadMaxSeqsData) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLStoreLoadMaxSeqsData) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0xdfc1208c:

		// not has flags

		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLStoreLoadMaxSeqsData) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// TLStoreSaveMaxSeq
///////////////////////////////////////////////////////////////////////////////

func (m *TLStoreSaveMaxSeq) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_store_saveMaxSeq))

	switch uint32(m.Constructor) {
	case 0x5b07985:
		x.UInt(0x5b07985)

		// no flags

		x.Int(m.GetId())
		x.Long(m.GetMaxSeq())

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLStoreSaveMaxSeq) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLStoreSaveMaxSeq) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0x5b07985:

		// not has flags

		m.Id = dBuf.Int()
		m.MaxSeq = dBuf.Long()
		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLStoreSaveMaxSeq) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// TLStoreLoadRouteTable
///////////////////////////////////////////////////////////////////////////////

func (m *TLStoreLoadRouteTable) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_store_loadRouteTable))

	switch uint32(m.Constructor) {
	case 0xcce2158f:
		x.UInt(0xcce2158f)

		// no flags

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLStoreLoadRouteTable) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLStoreLoadRouteTable) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0xcce2158f:

		// not has flags

		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLStoreLoadRouteTable) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// TLStoreSaveRouteTable
///////////////////////////////////////////////////////////////////////////////

func (m *TLStoreSaveRouteTable) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_store_saveRouteTable))

	switch uint32(m.Constructor) {
	case 0x3de9e8b4:
		x.UInt(0x3de9e8b4)

		// no flags

		x.Bytes(m.GetRouter().Encode(layer))

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLStoreSaveRouteTable) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLStoreSaveRouteTable) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0x3de9e8b4:

		// not has flags

		m1 := &Router{}
		m1.Decode(dBuf)
		m.Router = m1

		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLStoreSaveRouteTable) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// TLMediateRegisterAllocSvr
///////////////////////////////////////////////////////////////////////////////

func (m *TLMediateRegisterAllocSvr) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_mediate_registerAllocSvr))

	switch uint32(m.Constructor) {
	case 0xa6bfb234:
		x.UInt(0xa6bfb234)

		// no flags

		x.Bytes(m.GetNodeAddr().Encode(layer))

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLMediateRegisterAllocSvr) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLMediateRegisterAllocSvr) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0xa6bfb234:

		// not has flags

		m1 := &NodeAddrInfo{}
		m1.Decode(dBuf)
		m.NodeAddr = m1

		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLMediateRegisterAllocSvr) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

// TLMediateUnRegisterAllocSvr
///////////////////////////////////////////////////////////////////////////////

func (m *TLMediateUnRegisterAllocSvr) Encode(layer int32) []byte {
	x := NewEncodeBuf(512)
	// x.Int(int32(CRC32_mediate_unRegisterAllocSvr))

	switch uint32(m.Constructor) {
	case 0x6198f539:
		x.UInt(0x6198f539)

		// no flags

		x.Bytes(m.GetNodeAddr().Encode(layer))

	default:
		// log.Errorf("")
	}

	return x.GetBuf()
}

func (m *TLMediateUnRegisterAllocSvr) CalcByteSize(layer int32) int {
	return 0
}

func (m *TLMediateUnRegisterAllocSvr) Decode(dBuf *DecodeBuf) error {
	switch uint32(m.Constructor) {
	case 0x6198f539:

		// not has flags

		m1 := &NodeAddrInfo{}
		m1.Decode(dBuf)
		m.NodeAddr = m1

		return dBuf.GetError()

	default:
		// log.Errorf("")
	}
	return dBuf.GetError()
}

func (m *TLMediateUnRegisterAllocSvr) DebugString() string {
	jsonm := &jsonpb.Marshaler{OrigName: true}
	dbgString, _ := jsonm.MarshalToString(m)
	return dbgString
}

//----------------------------------------------------------------------------------------------------------------

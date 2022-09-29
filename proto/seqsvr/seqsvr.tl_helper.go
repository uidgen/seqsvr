// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Author: Benqi (wubenqi@gmail.com)
//

package seqsvr

var (
	BoolFalse = MakeTLBoolFalse2(nil).To_Bool2()
	BoolTrue  = MakeTLBoolTrue2(nil).To_Bool2()
)

func MakeInt64V(v int64) *Int64V {
	return MakeTLInt64V(
		&Int64V{
			V: v,
		}).To_Int64V()
}

//// RangeID
//// ID范围
//type RangeID struct {
//	IdBegin int32 // 整个空间
//	Size    int   // set集合里整个空间大小
//}

// CalcSetSectionSize
//// 计算出set里可以分配多少号段
func (m *RangeId) CalcSetSectionSize(perRangeSize int) int {
	return CalcSetSectionSize(int(m.Size2), perRangeSize)
}

// CheckIDByRange
// 检查id是否在当前set里
func (m *RangeId) CheckIDByRange(id int32) bool {
	return id >= m.IdBegin && id < m.IdBegin+int32(m.Size2)
}

// CalcSectionID
// 存储里会用到
// 由id计算出属于set里哪个section
func (m *RangeId) CalcSectionID(id int32) (bool, int32) {
	if !m.CheckIDByRange(id) {
		return false, 0
	}

	return true, (id - m.IdBegin) / PerSectionIdSize
}

// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Author: Benqi (wubenqi@gmail.com)
//

package seqsvr

// 整个系统又按uid范围进行分Set，每个Set都是一个完整的、独立的StoreSvr+AllocSvr子系统。
// 分Set设计目的是为了做灾难隔离，一个Set出现故障只会影响该Set内的用户，而不会影响到其它用户
//
// @benqi: 实际部署时，按实际服务能力需配置Set数，Set最好由配置文件来定义
//
//
// set标识符
// id_begin为set第一个id
//
// 号段一个简单例子
// 假设用户号段为1～16，有2个set，每个set里分配了2个section，每个section有4个号段
//
// |0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|
// |sec_0  |sec_4  |sec_8    |sec_12     |
// |    set_0      |    set_8            |
//
// 约定，任一服务所有的set_idx, section_idx, id都是基于整个号段空间的
//

// RangeID
// ID范围
type RangeID struct {
	IdBegin int32 // 整个空间
	Size    int   // set集合里整个空间大小
}

// CalcSetSectionSize
//// 计算出set里可以分配多少号段
func (r RangeID) CalcSetSectionSize(perRangeSize int) int {
	return CalcSetSectionSize(r.Size, perRangeSize)
}

// CheckIDByRange
// 检查id是否在当前set里
func (r RangeID) CheckIDByRange(id int32) bool {
	return id >= r.IdBegin && id < r.IdBegin+int32(r.Size)
}

// CalcSectionID
// 存储里会用到
// 由id计算出属于set里哪个section
func (r RangeID) CalcSectionID(id int32) (bool, int32) {
	if !r.CheckIDByRange(id) {
		return false, 0
	}

	return true, (id - r.IdBegin) / PerSectionIdSize
}

// SectionID
// section标识符
// id_begin为section里第一个id
type SectionID = RangeID

type SetID = RangeID // set标识符

// CalcSetSectionSize
// 计算出set里可以分配多少号段
func CalcSetSectionSize(rangeSize int, perRangeSize int) int {
	m := rangeSize % perRangeSize
	sz := rangeSize / perRangeSize
	if m == 0 {
		return sz
	} else {
		return sz + 1
	}
}

func CalcSectionID(begin int32, size int, id int32) (bool, int32) {
	rId := RangeID{begin, size}
	return rId.CalcSectionID(id)
}

// Section
//// Section只关注是第几个section
type Section struct {
	//Section(uint32_t id_begin, uint64_t* id_max_seq)
	//: section_id(id_begin),
	//max_seq(id_max_seq) {
	//
	//// 命名：section_name
	//folly::format(section_name, "section_{}_{}", kMaxSectionSize, id_begin);
	//}
	//
	//////////////////////////////////////////////////////////////////////////
	// section里分配的第一个id做为section_id
	SectionId int32
	// section_name命名规则：section_begin(id)_end(id+slot_size-1)
	SectionName string
	// section里的max_seq值
	MaxSeq *int32

	// 一个section分配了多少个ID
	// 不能放到配置文件里，因为是全局共享的，如果配置文件写错了
	// static uint32_t kIDSize;
}

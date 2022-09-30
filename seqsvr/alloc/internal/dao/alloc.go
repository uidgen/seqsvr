// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Author: Benqi (wubenqi@gmail.com)
//

package dao

import (
	"context"
	"fmt"
	"sync"

	"github.com/teamgram/seqsvr/proto/seqsvr"

	"github.com/gogo/protobuf/proto"
)

const (
	allocNone           = 0
	allocWaitRouteTable = 1
	allocWaitLoad       = 2
	allocWaitLoadSeq    = 3
	allocInited         = 4
	allocError          = 5
)

// AllocManager
// allocsvr流程：
// 1. 从存储系统里取出路由表
// 2. 通过本机Addr信息到路由表里获取当前服务号段列表
// 3. 启动租约服务
// 4. 从存储系统里取出max_seqs集合
// 5. 开始对客户端提供服务
//
// TODO(@benqi): 单机模拟set的allocsvr和storesvr
// 未加载成功重试加载
type AllocManager struct {
	*StoreManager
	LeaseCallback

	//   ////////////////////////////////////////////////////////////////////////////
	//  // alloc_addr: allocsvr的本机地址信息
	//  // store_addr_list: storesvr地址列表
	//  IpAddrInfo alloc_addr_;
	//  IpAddrInfoList store_addr_list_;
	//
	//  // store客户端管理器，存储storesvr
	//  std::unique_ptr<ClientManager> client_;
	//
	// 状态
	state int // {kAllocNone};
	//
	// 路由表
	table *seqsvr.Router
	// 保护Router
	// TODO(@benqi):
	// 1. 使用读写锁，
	// 2. 对router的操作封装成wrapper，预防忘记线程保护
	tableLock sync.Mutex

	// 本机路由节点
	cacheMyNode *seqsvr.RouterNode

	// 路由表租约
	lease *Lease

	// 同个号段内的用户共享一个max_seq
	// 号段对应max_seq
	sectionMaxSeqs []int64

	// 用户当前cur_seqs_
	curSeqs []int64
}

/*
 1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
 2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：
    如果cur_seq > max_seq，将分配上限提升一个步长max_seq += step，并持久化max_seq
 3. 重启时，读出持久化的max_seq，赋值给cur_seq
*/

func MustNewAllocManager(store *StoreManager) *AllocManager {
	cacheMyNode := &seqsvr.RouterNode{
		NodeAddr: &seqsvr.NodeAddrInfo{
			Ip:   "127.0.0.1",
			Port: 10100,
		},
		SectionRanges: []*seqsvr.RangeId{
			&seqsvr.RangeId{
				IdBegin: 0,
				Size2:   10240,
			},
		},
	}

	table := &seqsvr.Router{
		Version: 0,
		NodeList: []*seqsvr.RouterNode{
			cacheMyNode,
		},
	}

	alloc := &AllocManager{
		StoreManager:   store,
		LeaseCallback:  nil,
		state:          allocNone,
		table:          table,
		tableLock:      sync.Mutex{},
		cacheMyNode:    cacheMyNode,
		lease:          nil,
		sectionMaxSeqs: nil,
		curSeqs:        nil,
	}

	alloc.LoadMaxSeq()

	return alloc
}

// LoadMaxSeq
//////////////////////////////////////////////////////////////////////////////////////////////
// bytes
func (m *AllocManager) LoadMaxSeq() {
	//// TODO(@beqni): NWR读
	//// 2. 去storesvr加载max_seqs
	//state_ = kAllocWaitRouteTable;
	//
	//seqsvr::MaxSeqsData max_seqs_data;
	//client_->LoadMaxSeqsData(max_seqs_data);
	//OnMaxSeqLoaded(max_seqs_data);
	m.state = allocWaitRouteTable

	maxSeqs, err := m.StoreManager.LoadMaxSeqsData(context.Background())
	if err != nil {
		return
	}
	m.OnMaxSeqLoaded(maxSeqs)
}

func (m *AllocManager) SaveMaxSeq(id int32, sectionMaxSeq int64) {
	//// TODO(@beqni): NWR写
	//
	m.StoreManager.SaveMaxSeq(context.Background(), id, sectionMaxSeq)
}

func (m *AllocManager) OnMaxSeqLoaded(data *seqsvr.MaxSeqsData) {
	// TODO(@benqi): 检查数据是否合法
	// 复制数据
	// LOG(INFO) << "OnMaxSeqLoaded - data len = " << data.length();
	m.sectionMaxSeqs = data.MaxSeqs
	// 将cur_seq设置为max_seq
	m.curSeqs = make([]int64, len(m.sectionMaxSeqs)*seqsvr.PerSectionIdSize)
	for i := 0; i < len(m.sectionMaxSeqs); i++ {
		for j := i * seqsvr.PerSectionIdSize; j < seqsvr.PerSectionIdSize; j++ {
			m.curSeqs[j] = m.sectionMaxSeqs[i]
		}
	}

	m.state = allocInited
}

func (m *AllocManager) OnMaxSeqSaved() {
}

// GetCurrentSequence - GetCurrentSequence
func (m *AllocManager) GetCurrentSequence(id, clientVersion int32) (*seqsvr.Sequence, error) {
	if m.cacheMyNode == nil {
		//(!cache_my_node_)
		return nil, fmt.Errorf("cacheMyNode is nil")
	}

	if m.state != allocInited {
		return nil, fmt.Errorf("state != allocInited")
	}

	for _, v := range m.cacheMyNode.SectionRanges {
		ok, _ := seqsvr.CalcSectionID(v.IdBegin, int(v.Size2), id)
		if ok {
			seq := seqsvr.MakeTLSequence(&seqsvr.Sequence{
				Seq:    m.curSeqs[id-v.IdBegin],
				Router: nil,
			}).To_Sequence()
			if clientVersion < m.table.Version {
				seq.Router = proto.Clone(m.table).(*seqsvr.Router)
			}

			return seq, nil
		}
	}

	return nil, fmt.Errorf("invalid id - (%v, %d)", id, clientVersion)
}

// FetchNextSequence
// id计算公式
//idx =  (id-node.id_begin)
func (m *AllocManager) FetchNextSequence(id, clientVersion int32) (*seqsvr.Sequence, error) {
	if m.cacheMyNode == nil {
		//(!cache_my_node_)
		return nil, fmt.Errorf("cacheMyNode is nil")
	}

	if m.state != allocInited {
		return nil, fmt.Errorf("state != allocInited")
	}

	for _, v := range m.cacheMyNode.SectionRanges {
		ok, idx := seqsvr.CalcSectionID(v.IdBegin, int(v.Size2), id)
		if ok {
			m.curSeqs[id-v.IdBegin]++
			seq := seqsvr.MakeTLSequence(&seqsvr.Sequence{
				Seq:    m.curSeqs[id-v.IdBegin],
				Router: nil,
			}).To_Sequence()

			if seq.Seq > m.sectionMaxSeqs[idx] {
				m.SaveMaxSeq(id, seq.Seq)
				// TODO(@benqi): 使用返回值填充
				m.sectionMaxSeqs[idx] += seqsvr.SeqStep
			}
			if clientVersion < m.table.Version {
				seq.Router = proto.Clone(m.table).(*seqsvr.Router)
			}
			return seq, nil
		}
	}

	//
	//  LOG(INFO) << "FetchNextSequence - id: " << id
	//    << ", client_version: " << client_version
	//    << ", sequence: " << o.sequence;
	//
	//  return false;
	return nil, fmt.Errorf("invalid id - (%v, %d)", id, clientVersion)
}

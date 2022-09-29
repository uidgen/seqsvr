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

package seqsvr

const (
	CRC32_vector TLConstructor = 481674261
)

const (
	// MaxIDSize
	//#ifndef DEBUG_TEST
	// 生产环境，按uint32_t整个空间来分
	MaxIDSize = 0xffffffff // uint32_t范围的id

	// DebugMaxIDSize
	//#else
	// 测试时，为了在开发机上能快速启动、运行和调试
	DebugMaxIDSize = 1 << 20 // 1MB
	//#endif

	// SeqStep
	// ## 预分配中间层
	// - sequence只要求递增，并没有要求连续，也就是说出现一大段跳跃是允许的
	// （例如分配出的sequence序列：1,2,3,10,100,101）。
	// 于是我们实现了一个简单优雅的策略：
	//  1. 内存中储存最近一个分配出去的sequence：cur_seq，以及分配上限：max_seq
	//  2. 分配sequence时，将cur_seq++，同时与分配上限max_seq比较：如果cur_seq > max_seq，
	//     将分配上限提升一个步长max_seq += step，并持久化max_seq
	//  3. 重启时，读出持久化的max_seq，赋值给cur_seq
	//
	// - 实际应用中每次提升的步长为10000
	SeqStep = 10000

	// PerSectionIdSize
	// ## 分号段共享存储
	// > 重启时要读取大量的max_seq数据加载到内存中。
	// - uid相邻的一段用户属于一个号段，而同个号段内的用户共享一个max_seq，
	//   这样大幅减少了max_seq数据的大小，同时也降低了IO次数。
	//
	// - 目前seqsvr一个Section包含10万个uid，max_seq数据只有300+KB
	//   一个Section包含10万个uid，一个uint32_t空间共有42950个Section
	PerSectionIdSize = 100000

	// MaxSectionSize
	// 整个uid空间内有多少个section
	MaxSectionSize = DebugMaxIDSize/PerSectionIdSize + 1

	//(kMaxIDSize/kPerSectionIdSize) :
	//(kMaxIDSize/kPerSectionIdSize)+1;

	// MaxSectionMemSize
	// 整个uid空间总计占用section内存
	// 号段为uint64_t
	MaxSectionMemSize = MaxSectionSize << 3
)

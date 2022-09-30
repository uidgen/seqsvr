// Copyright 2022 Teamgram Authors
//  All rights reserved.
//
// Author: Benqi (wubenqi@gmail.com)
//

package dao

import (
	"bytes"
	"errors"
	"fmt"
	"io/ioutil"
	"os"

	"github.com/teamgram/seqsvr/proto/seqsvr"

	"github.com/edsrzf/mmap-go"
	"github.com/gogo/protobuf/jsonpb"
)

func checkFileExists(filePath string) bool {
	_, err := os.Stat(filePath)
	//return !os.IsNotExist(err)
	return !errors.Is(err, os.ErrNotExist)
}

func fileSize(path string) int64 {
	fi, err := os.Stat(path)
	if err != nil {
		// log.Fatal(err)
		return 0
	}

	return fi.Size()
}

type StoreManager struct {
	// Set seqsvr.SetNodeInfo
	// std::string seq_file_path_;
	// set
	set seqsvr.Set

	// 使用内存映射文件文件操作section文件
	// int section_fd_ {-1};
	// folly::MemoryMapping* section_max_seqs_mapping_ {nullptr};
	// folly::Range<uint64_t*> mapping_mem_;
	sectionFd             *os.File
	sectionMaxSeqsMapping mmap.MMap

	// 路由表存储文件
	routeTableFilePath string
	cacheRouter        *seqsvr.Router
}

func MustNewStoreManager(setId seqsvr.SetID, filePath string) *StoreManager {
	// set_id_ = set_id;
	seqFilePath := fmt.Sprintf("%s/set_%d_%d.db",
		filePath,
		setId.IdBegin,
		setId.Size)

	sectionSize := setId.CalcSetSectionSize(seqsvr.PerSectionIdSize)
	sectionMemSize := sectionSize << 3

	var (
		isFirst   = false
		sectionFd *os.File
		err       error
	)
	if !checkFileExists(seqFilePath) {
		sectionFd, err = os.OpenFile(seqFilePath, os.O_RDWR|os.O_CREATE, 0755)
		if err != nil {
			panic(err)
		}
		err = sectionFd.Truncate(int64(sectionMemSize))
		if err != nil {
			panic(err)
		}
		isFirst = true
	} else {
		sz := fileSize(seqFilePath)
		//boost::filesystem::file_size(f);
		if sz != int64(sectionMemSize) {
			panic(fmt.Sprintf("section's file size invalid, sz = %d", sz))
		} else {
			sectionFd, err = os.OpenFile(seqFilePath, os.O_RDWR, 0755)
			if err != nil {
				panic(err)
			}
		}
	}

	sectionMaxSeqsMapping, err := mmap.Map(sectionFd, mmap.RDWR, 0)
	if err != nil {
		panic("error mapping: %s" + err.Error())
	}

	if isFirst {
		for i := 0; i < len(sectionMaxSeqsMapping); i++ {
			sectionMaxSeqsMapping[i] = 0
		}
	}

	// 至此，初始化set成功
	set := seqsvr.Set{
		SetId:          setId,
		SetMaxSeqsData: seqsvr.SliceInt64{MMap: sectionMaxSeqsMapping},
	}
	//std::make_unique<Set>(set_id, mapping_mem);

	var cacheRouter *seqsvr.Router

	routeTableFilePath := fmt.Sprintf("%s/router_%d_%d.dat",
		filePath,
		setId.IdBegin,
		setId.Size)
	if checkFileExists(routeTableFilePath) {
		var (
			routerData []byte
		)
		routerData, err = ioutil.ReadFile(routeTableFilePath)
		if err != nil {
			panic(err)
		}
		if len(routerData) > 0 {
			cacheRouter = new(seqsvr.Router)
			err = jsonpb.Unmarshal(bytes.NewBuffer(routerData), cacheRouter)
			if err != nil {
				fmt.Println(err)
			}
		}
	}

	if cacheRouter == nil {
		cacheRouter = seqsvr.MakeTLRouter(nil).To_Router()
	}

	return &StoreManager{
		set:                   set,
		sectionFd:             sectionFd,
		sectionMaxSeqsMapping: sectionMaxSeqsMapping,
		routeTableFilePath:    routeTableFilePath,
		cacheRouter:           cacheRouter,
	}
}

func (s *StoreManager) GetMaxSeqsData() (*seqsvr.MaxSeqsData, error) {
	//if (!inited_ || !set_) {
	//return false;
	//}
	//
	//// return set_->GetSectionSetData(max_seqs);
	//max_seqs.set_id.id_begin = set_->GetID().id_begin;
	//max_seqs.set_id.size = set_->GetID().size;
	//max_seqs.max_seqs.assign(
	//set_->GetMaxSeqsData().begin(),
	//set_->GetMaxSeqsData().end());
	//
	//return true;
	maxSeqs := seqsvr.MakeTLMaxSeqsData(&seqsvr.MaxSeqsData{
		SetId:   new(seqsvr.RangeId),
		MaxSeqs: nil,
	}).To_MaxSeqsData()
	maxSeqs.SetId.IdBegin = s.set.SetId.IdBegin
	maxSeqs.SetId.Size2 = int32(s.set.SetId.Size)
	for i := 0; i < s.set.SetMaxSeqsData.Len(); i++ {
		maxSeqs.MaxSeqs = append(maxSeqs.MaxSeqs, s.set.SetMaxSeqsData.GetIdx(i))
	}

	return maxSeqs, nil
}

func (s *StoreManager) SetSectionMaxSeq(id int32, maxSeq int64) int64 {
	//CHECK(inited_);
	//CHECK(set_);
	//
	return s.set.SetMaxSeq(id, maxSeq)
}

func (s *StoreManager) GetCacheRouter() *seqsvr.Router {
	return s.cacheRouter
}

func (s *StoreManager) SaveCacheRouter(router *seqsvr.Router) error {
	data, err := new(jsonpb.Marshaler).MarshalToString(router)
	if err != nil {
		return err
	}

	err = ioutil.WriteFile(s.routeTableFilePath, []byte(data), 0644)
	if err != nil {
		//
		return err
	}

	s.cacheRouter = router
	return nil
}

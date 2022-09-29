# seqsvr proto


```
	"TLMediateRegisterAllocSvr":   RPCContextTuple{"/seqsvr.RPCMediate/mediate_registerAllocSvr", func() interface{} { return new(Bool2) }},
	"TLMediateUnRegisterAllocSvr": RPCContextTuple{"/seqsvr.RPCMediate/mediate_unRegisterAllocSvr", func() interface{} { return new(Bool2) }},
	
	"TLAllocFetchNextSequence": RPCContextTuple{"/seqsvr.RPCAlloc/alloc_fetchNextSequence", func() interface{} { return new(Sequence) }},
    "TLAllocGetCurrentSequence": RPCContextTuple{"/seqsvr.RPCAlloc/alloc_getCurrentSequence", func() interface{} { return new(Sequence) }},

    "TLStoreLoadMaxSeqsData": RPCContextTuple{"/seqsvr.RPCStore/store_loadMaxSeqsData", func() interface{} { return new(MaxSeqsData) }},
    "TLStoreSaveMaxSeq": RPCContextTuple{"/seqsvr.RPCStore/store_saveMaxSeq", func() interface{} { return new(Int64V) }},
    "TLStoreLoadRouteTable": RPCContextTuple{"/seqsvr.RPCStore/store_loadRouteTable", func() interface{} { return new(Router) }},
    "TLStoreSaveRouteTable": RPCContextTuple{"/seqsvr.RPCStore/store_saveRouteTable", func() interface{} { return new(Bool2) }},
```


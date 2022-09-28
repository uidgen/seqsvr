#!/bin/sh
SRC_DIR=.
DST_DIR=.

#./codegen.sh
#protoc -I=$SRC_DIR --go_out=$DST_DIR/ $SRC_DIR/*.proto
#protoc -I=$SRC_DIR --go_out=plugins=grpc:$DST_DIR/ $SRC_DIR/*.proto
protoc -I=$SRC_DIR --gogo_out=plugins=grpc,Mgoogle/protobuf/wrappers.proto=github.com/gogo/protobuf/types,:$DST_DIR $SRC_DIR/*.proto

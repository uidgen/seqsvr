VERSION=v0.87.1-teamgram-server
BUILD=`date +%F`
SHELL := /bin/bash
BASEDIR = $(shell pwd)
INSTALL="deploy/build"

# build with verison infos
versionDir="github.com/teamgram/marmota/pkg/version"
gitTag=$(shell if [ "`git describe --tags --abbrev=0 2>/dev/null`" != "" ];then git describe --tags --abbrev=0; else git log --pretty=format:'%h' -n 1; fi)
gitBranch=$(shell git rev-parse --abbrev-ref HEAD)
buildDate=$(shell TZ=Asia/Shanghai date +%FT%T%z)
gitCommit=$(shell git rev-parse --short HEAD)
gitTreeState=$(shell if git status|grep -q 'clean';then echo clean; else echo dirty; fi)

ldflags="-s -w -X ${versionDir}.gitTag=${gitTag} -X ${versionDir}.buildDate=${buildDate} -X ${versionDir}.gitCommit=${gitCommit} -X ${versionDir}.gitTreeState=${gitTreeState} -X ${versionDir}.version=${VERSION} -X ${versionDir}.gitBranch=${gitBranch}"

all: alloc mediate store

alloc:
	@echo "build alloc..."
	@go build -ldflags ${ldflags} -o ${INSTALL}/bin/alloc -tags=jsoniter seqsvr/alloc/cmd/alloc/*.go

mediate:
	@echo "build mediate..."
	@go build -ldflags ${ldflags} -o ${INSTALL}/bin/mediate -tags=jsoniter seqsvr/mediate/cmd/mediate/*.go

store:
	@echo "build store..."
	@go build -ldflags ${ldflags} -o ${INSTALL}/bin/store -tags=jsoniter seqsvr/store/cmd/store/*.go

clean:
	@rm -rf ${INSTALL}/bin/alloc
	@rm -rf ${INSTALL}/bin/mediate
	@rm -rf ${INSTALL}/bin/store

/*
 *  Copyright (c) 2016, https://github.com/nebula-im
 *  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MEDIATESVR_MEDIATE_SERVICE_HANDLER_H_
#define MEDIATESVR_MEDIATE_SERVICE_HANDLER_H_

#include "proto/gen-cpp2/MediateService.h"

class MediateServiceHandler : public seqsvr::MediateServiceSvIf {
public:
  MediateServiceHandler() = default;
  ~MediateServiceHandler() override = default;

  // void RegisterAllocSvr() override;
  // void UnRegisterAllocSvr() override;
  // void UpdateRouteTable(std::unique_ptr< ::seqsvr::Router> router) override;
  
  void RegisterAllocSvr(std::unique_ptr< ::seqsvr::NodeAddrInfo> node_addr) override;
  void UnRegisterAllocSvr(std::unique_ptr< ::seqsvr::NodeAddrInfo> node_addr) override;

};

#endif

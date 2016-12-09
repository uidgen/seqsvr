/*
 *  Copyright (c) 2016, https://github.com/zhatalk
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

#include "client/client_command_handler.h"

#include <folly/Conv.h>

#include "nebula/base/time_util.h"
#include "nebula/base/id_util.h"

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"

#include "nebula/net/rpc/zrpc_service_util.h"

typedef int (*ClientCommandHandlerFunc)(const std::vector<folly::StringPiece>&);

//
struct CmdEntry {
  const char* cmd;                    // 命令名
  const char* cmd_help;               // 命令帮助信息
  uint32_t cmd_num_min;               // 最少参数
  uint32_t cmd_num_max;               // 最大参数，如果值为0，则无参数限制
  ClientCommandHandlerFunc handler;   // 命令处理函数
};

int DoFetchNextSeq(const std::vector<folly::StringPiece>& command_lines) {
  uint32_t user_id = 0;
  try {
    user_id = folly::to<uint32_t>(command_lines[1]);
  } catch (...) {
    LOG(ERROR) << "DoFetchNextSeq - user_id invalid, not a number: " << command_lines[1];
    return 0;
  }
  
  zproto::FetchNextSequenceReq fetch_next_sequence_req;
  fetch_next_sequence_req.set_user_id(user_id);
  
  auto req = MakeRpcRequest(fetch_next_sequence_req);
  LOG(INFO) << "DoFetchNextSeq - req: " << req->ToString();
  ZRpcUtil::DoClientCall("seq_client", req)
  .within(std::chrono::milliseconds(5000))
  .then([](ProtoRpcResponsePtr rsp) {
    LOG(INFO) << "DoFetchNextSeq - rsp: " << rsp->ToString();
    if (rsp->GetPackageType() == Package::RPC_OK) {
      auto seq_rsp = ToApiRpcOk<zproto::SequenceRsp>(rsp);
      LOG(INFO) << "DoFetchNextSeq - SequenceRsp: " << seq_rsp->ToString();
    }
  });
  
  return 0;
}

int DoGetCurrentSeq(const std::vector<folly::StringPiece>& command_lines) {
  uint32_t user_id = 0;
  try {
    user_id = folly::to<uint32_t>(command_lines[1]);
  } catch (...) {
    LOG(ERROR) << "DoGetCurrentSeq - user_id invalid, not a number: " << command_lines[1];
    return 0;
  }
  
  zproto::GetCurrentSequenceReq get_current_sequence_req;
  get_current_sequence_req.set_user_id(user_id);
  
  auto req = MakeRpcRequest(get_current_sequence_req);
  LOG(INFO) << "DoGetCurrentSeq - req: " << req->ToString();
  ZRpcUtil::DoClientCall("seq_client", req)
  .within(std::chrono::milliseconds(5000))
  .then([](ProtoRpcResponsePtr rsp) {
    LOG(INFO) << "DoGetCurrentSeq - rsp: " << rsp->ToString();
    if (rsp->GetPackageType() == Package::RPC_OK) {
      auto seq_rsp = ToApiRpcOk<zproto::SequenceRsp>(rsp);
      LOG(INFO) << "DoGetCurrentSeq - SequenceRsp: " << seq_rsp->ToString();
    }
  });
  
  return 0;
}


int DoQuit(const std::vector<folly::StringPiece>& command_lines) {
  // exit(0);
  return -2;
}

CmdEntry g_cmds[] = {
  // login/logout
  {"fetchnextseq", "fetchnextseq user_id", 2, 0, DoFetchNextSeq},
  {"getcurrentseq", "getcurrentseq user_id", 2, 0, DoGetCurrentSeq},
  // quit
  {"quit", "quit", 1, 0, DoQuit}
};

void PrintHelp() {
  printf("Usage(***all user_id/group_id/shop_id are string format***):\n");
  
  for (uint32_t i=0; i<sizeof(g_cmds)/sizeof(CmdEntry); ++i) {
    printf("\t %s\n", g_cmds[i].cmd_help);
  }
}


int DoClientCommand(const std::vector<folly::StringPiece>& command_lines) {
  if (command_lines.empty()) {
    return 0;
  }
  
  // init_log();
  
  CmdEntry* cmd = NULL;
  
  for (uint32_t i=0; i<sizeof(g_cmds)/sizeof(CmdEntry); ++i) {
    if (command_lines[0] == g_cmds[i].cmd) {
      cmd = &g_cmds[i];
      break;
    }
  }
  
  if (cmd == NULL) {
    PrintHelp();
    return 0;
  }
  
  uint32_t cmmand_param_num = (uint32_t)command_lines.size();
  
  // 检测输入参数
  if (cmd->cmd_num_max != 0) {
    if (cmmand_param_num < cmd->cmd_num_min || cmmand_param_num > cmd->cmd_num_max) {
      PrintHelp();
      return 0;
    }
  } else {
    if (cmmand_param_num != cmd->cmd_num_min) {
      PrintHelp();
      return 0;
    }
  }
  
  return cmd->handler(command_lines);
}




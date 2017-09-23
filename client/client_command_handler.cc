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
// #include "nebula/base/id_util.h"
// #include "nebula/net/rpc/zrpc_service_util.h"
// #include "base/message_handler_util.h"

typedef int (*ClientCommandHandlerFunc)(seqsvr::AllocServiceAsyncClient*, const std::vector<folly::StringPiece>&);

//
struct CmdEntry {
  const char* cmd;                    // 命令名
  const char* cmd_help;               // 命令帮助信息
  uint32_t cmd_num_min;               // 最少参数
  uint32_t cmd_num_max;               // 最大参数，如果值为0，则无参数限制
  ClientCommandHandlerFunc handler;   // 命令处理函数
};

int DoFetchNextSeq(seqsvr::AllocServiceAsyncClient* client, const std::vector<folly::StringPiece>& command_lines) {
  uint32_t id = 0;
  try {
    id = folly::to<uint32_t>(command_lines[1]);
  } catch (...) {
    LOG(ERROR) << "DoFetchNextSeq - user_id invalid, not a number: " << command_lines[1];
    return 0;
  }

  seqsvr::Sequence _return;
  client->sync_FetchNextSequence(_return, id, 0);
  
  LOG(INFO) << "DoFetchNextSeq - seq_rsp: " << _return.sequence;
  
  return 0;
}

int DoGetCurrentSeq(seqsvr::AllocServiceAsyncClient* client, const std::vector<folly::StringPiece>& command_lines) {
  uint32_t id = 0;
  try {
    id = folly::to<uint32_t>(command_lines[1]);
  } catch (...) {
    LOG(ERROR) << "DoFetchNextSeq - user_id invalid, not a number: " << command_lines[1];
    return 0;
  }
  
  seqsvr::Sequence _return;
  client->sync_GetCurrentSequence(_return, id, 0);
  
  LOG(INFO) << "sync_GetCurrentSequence - seq_rsp: " << _return.sequence;

  return 0;
}

int DoQuit(seqsvr::AllocServiceAsyncClient* client, const std::vector<folly::StringPiece>& command_lines) {
  // exit(0);
  return -2;
}

CmdEntry g_cmds[] = {
  // login/logout
  {"fetch_next_seq", "fetch_next_seq id", 2, 2, DoFetchNextSeq},
  {"get_current_seq", "get_current_seq id", 2, 2, DoGetCurrentSeq},
  // quit
  {"quit", "quit", 1, 0, DoQuit}
};

void PrintHelp() {
  printf("Usage(***all user_id/group_id/shop_id are string format***):\n");
  
  for (uint32_t i=0; i<sizeof(g_cmds)/sizeof(CmdEntry); ++i) {
    printf("\t %s\n", g_cmds[i].cmd_help);
  }
}


int DoClientCommand(seqsvr::AllocServiceAsyncClient* client, const std::vector<folly::StringPiece>& command_lines) {
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
  
  return cmd->handler(client, command_lines);
}




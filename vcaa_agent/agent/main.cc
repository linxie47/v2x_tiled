// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

//!
//! \file       main.cc
//! \brief      agent service implementations
//!
#include <grpcpp/grpcpp.h>
// grpc interfaces
#include "vcaa_agent.grpc.pb.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
namespace vgrpc = vcaa::grpcService;

// std interfaces
#include <iostream>
#include <memory>
#include <string>

#include "agent_flags.hpp"
#include "agent_glog.hpp"
#include "agent_server.hpp"

DEFINE_bool(h, false, help_message);

DEFINE_string(gh, "0.0.0.0", grpc_host_message);

DEFINE_int32(gp, 8080, grpc_port_message);

DEFINE_string(s, "", storage_message);

DEFINE_string(id, "agent_xxx", agent_id_message);

bool CheckCommandLine() {
    VLOG(2) << "Check command line" << std::endl;
    if (FLAGS_s.empty()) {
        LOG(FATAL) << "There is no model storage setted!" << std::endl;
        return false;
    }
    if (FLAGS_id.empty()) {
        LOG(FATAL) << "There is no agent id setted!" << std::endl;
        return false;
    }
    return true;
}

void RunServer() {
    std::string server_address = FLAGS_gh + std::string(":") + std::to_string(FLAGS_gp);

    vcaa::agent::VCAAServiceImpl agentService;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&agentService);

    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    LOG(INFO) << "gRPC Server listening on: " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char **argv) {
    gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
    vcaa::agent::AgentGlogger agentLogger(argv[0]);

    /*
      vcaa::agent::VcaaAgentController::Ptr vcaaAgent =
          vcaa::agent::VcaaAgentController::instance();
      vcaaAgent->setAgentState(vgrpc::AgentState::AGENT_INVALID);
      if (false == CheckCommandLine()) {
        vcaaAgent->setAgentState(vgrpc::AgentState::AGENT_FAILED_TO_STARTUP);
        showUsage();
        LOG(ERROR) << "Failed to check command line" << std::endl;
        goto Exit;
      }
      vcaaAgent->setAgentState(vgrpc::AgentState::AGENT_STARTING);
    */
    RunServer();
    return 0;
}

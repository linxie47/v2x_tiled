// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

//!
//! \file       agent_client.hpp
//! \brief      agent client
//!
#pragma once
#include "vcaa_agent.grpc.pb.h"
using namespace vcaa::grpcService;

// gRPC headers
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

// std headers
#include <memory>
#include <string>
#include <vector>

namespace vcaa {
namespace agent {

class VCAAClient final {
  public:
    VCAAClient(std::shared_ptr<grpc::Channel> channel);
    VCAAClient(VCAAClient &&) = default;
    VCAAClient(const VCAAClient &) = default;
    VCAAClient &operator=(VCAAClient &&) = default;
    VCAAClient &operator=(const VCAAClient &) = default;
    ~VCAAClient(){};

  public:
    bool CreateVideoAnalyticsService(const std::string &model_name, const AnalyticsConfigParam &param);

  private:
    std::unique_ptr<VCAAService::Stub> stub_;
};

} // end of namespace agent
} // end of namespace vcaa

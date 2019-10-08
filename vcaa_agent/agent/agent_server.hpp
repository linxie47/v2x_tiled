// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//
#pragma once

#include <iostream>
#include <memory>
#include <string>

// grpc headers
#include "vcaa_agent.grpc.pb.h"
#include <grpcpp/grpcpp.h>
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

#include "agent_flags.hpp"

namespace vgrpc = vcaa::grpcService;

namespace vcaa {
namespace agent {

class VCAAServiceImpl final : public vgrpc::VCAAService::Service {
  public:
    VCAAServiceImpl() = default;
    VCAAServiceImpl(VCAAServiceImpl &&) = default;
    VCAAServiceImpl(const VCAAServiceImpl &) = default;
    VCAAServiceImpl &operator=(VCAAServiceImpl &&) = default;
    VCAAServiceImpl &operator=(const VCAAServiceImpl &) = default;
    virtual ~VCAAServiceImpl(){};

  public:
    Status CreateVideoAnalyticsService(ServerContext *context, const vgrpc::VideoAnalyticsRequest *req,
                                       vgrpc::VideoAnalyticsResponse *res) override;
};

} // end namespace agent
} // end namespace vcaa
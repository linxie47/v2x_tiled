//
// Copyright 2017-2019 Intel Corporation.
//
// Author:
//
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they
// were provided to you (End User License Agreement for the Intel(R) Software
// Development Products (Version Septmeber 2018)). Unless the License provides
// otherwise, you may not use, modify, copy, publish, distribute, disclose or
// transmit this software or the related documents without Intel's prior
// written permission.
//
// This software and the related documents are provided as is, with no
// express or implied warranties, other than those that are expressly
// stated in the License.
//

//!
//! \file       agent_client.hpp
//! \brief      agent client
//!
#pragma once
#include "vcaa_agent.grpc.pb.h"
using namespace vcaa::grpcService;

//gRPC headers
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

// std headers
#include <memory>
#include <string>
#include <vector>

namespace vcaa
{
namespace agent
{

class VCAAClient final
{
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

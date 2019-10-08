// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

// local headers
#include "agent_server.hpp"
#include "agent_glog.hpp"

namespace vgrpc = vcaa::grpcService;

namespace vcaa {
namespace agent {
Status VCAAServiceImpl::CreateVideoAnalyticsService(ServerContext *context, const vgrpc::VideoAnalyticsRequest *req,
                                                    vgrpc::VideoAnalyticsResponse *res) {
    VLOG(2) << "CreateVideoAnalyticsService!" << std::endl;

    return Status::OK;
}
} // namespace agent
} // namespace vcaa
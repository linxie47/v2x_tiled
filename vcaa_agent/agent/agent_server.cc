// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

// local headers
#include "agent_server.hpp"
#include "agent_glog.hpp"
#include "deep_player.hpp"

namespace vgrpc = vcaa::grpcService;

namespace vcaa {
namespace agent {
Status VCAAServiceImpl::CreateVideoAnalyticsService(ServerContext *context, const vgrpc::VideoAnalyticsRequest *req,
                                                    vgrpc::VideoAnalyticsResponse *res) {
    VLOG(2) << "CreateVideoAnalyticsService!" << std::endl;

    LOG(INFO) << "load model:" << req->model_name() << std::endl;

    vgrpc::ResStatus *res_status = res->mutable_status();
    res_status->set_code(vgrpc::ResStatusCode::OK);
    // success, no message
    res_status->clear_message();

    const char *params[] = {"aaa", "bbb"};
    player_go(1, (char **)params);

    return Status::OK;
}

Status VCAAServiceImpl::DestroyVideoAnalyticsService(ServerContext *context,
                                                     const vgrpc::DestroyAnalyticsServiceRequest *req,
                                                     vgrpc::DestroyServiceResponse *res) {
    return Status::OK;
}

Status VCAAServiceImpl::SetMediaStreamInfo(ServerContext *context, const vgrpc::MediaStreamSettingRequest *req,
                                           vgrpc::MediaStreamSettingResponse *res) {
    return Status::OK;
}

Status VCAAServiceImpl::StartInference(ServerContext *context, const vgrpc::InferStartRequest *req,
                                       ServerWriter<vgrpc::InferResponse> *writer) {
    return Status::OK;
}

Status VCAAServiceImpl::StopInference(ServerContext *context, const vgrpc::InferStopRequest *req,
                                      vgrpc::InferStopResponse *res) {
    return Status::OK;
}

} // namespace agent
} // namespace vcaa
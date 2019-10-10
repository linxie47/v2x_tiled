// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

//!
//! \file       agent_client.cc
//! \brief      agent client
//!
#include "agent_client.hpp"
#include <chrono>
#include <fstream> // std::ifstream
#include <glog/logging.h>
#include <thread>

namespace vcaa {
namespace agent {

#ifndef UNUSED_PARAM
#define UNUSED_PARAM(x) (x)
#endif // UNUSED_PARAM
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

//
// @template make_unique_vcaa
// @brief std11 not support this feature
//
template <typename T, typename... Args>
std::unique_ptr<T> make_unique_vcaa(Args &&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

bool checkResStatus(const ResStatus &status) {
    if (status.code() != 0) {
        LOG(ERROR) << "Response err: " << status.code() << ", msg: " << status.message() << std::endl;
        return false;
    }
    return true;
}

VCAAClient::VCAAClient(std::shared_ptr<grpc::Channel> channel) : stub_(VCAAService::NewStub(channel)) {
}

bool VCAAClient::CreateVideoAnalyticsService(const std::string &model_name, const AnalyticsConfigParam &param) {
    ClientContext context;
    VideoAnalyticsRequest req;
    VideoAnalyticsResponse res;
    req.set_model_name(model_name);

    AnalyticsConfigParam *config_param = req.mutable_config();
    *config_param = param;

    Status status = stub_->CreateVideoAnalyticsService(&context, req, &res);
    if (!status.ok()) {
        LOG(ERROR) << "Failed to create analytics service for model: " << model_name << std::endl;
        return false;
    }

    if (!res.has_status() || !checkResStatus(res.status())) {
        LOG(ERROR) << "Response to create analytics service fail!"<< std::endl;
        return false;
    }

    return true;
}

} // end of namespace agent
} // end of namespace vcaa

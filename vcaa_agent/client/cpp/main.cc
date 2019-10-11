#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include "vcaa_agent.grpc.pb.h"
#include <glog/logging.h>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "agent_client.hpp"
#include "client_common.hpp"
#include "client_flags.hpp"

using namespace vcaa::agent;
using namespace vcaa::grpcService;

DEFINE_bool(h, false, help_message);

DEFINE_string(gs, "", grpc_address_message);

DEFINE_string(rtmp, "", rtmp_message);

DEFINE_string(image, "", image_message);

DEFINE_string(cf, "", config_message);

DEFINE_string(cs, "", config_message);

DEFINE_string(cc, "", config_message);

DEFINE_int32(deadline_ms, 30 * 1000, deadline_ms_message);

DEFINE_int32(unload_ms, -1, unload_ms_message);

DEFINE_int32(image_num, 100, image_num_message);

DEFINE_string(image_format, "bgra", image_format_message);

bool CheckCommandLine() {
    LOG(INFO) << "Check command line" << std::endl;
    if (FLAGS_gs.empty()) {
        LOG(ERROR) << "There is no grpc address setted!" << std::endl;
        return false;
    }
    if (FLAGS_rtmp.empty() && FLAGS_image.empty() && FLAGS_cf.empty() && FLAGS_cs.empty() && FLAGS_cc.empty()) {
        LOG(ERROR) << "There is neither rtmp url nor image file or config file path setted!" << std::endl;
        return false;
    }
    if (!FLAGS_image_format.empty()) {
        if ((FLAGS_image_format != std::string("rgb")) && (FLAGS_image_format != std::string("rgba")) &&
            (FLAGS_image_format != std::string("bgra")) && (FLAGS_image_format != std::string("bgr")) &&
            (FLAGS_image_format != std::string("jpg")) && (FLAGS_image_format != std::string("bmp")) &&
            (FLAGS_image_format != std::string("png"))) {
            return false;
        }
    }
    return true;
}

static void showUsage() {
    std::cout << std::endl;
    std::cout << "VCAA agent grpc client [OPTION]" << std::endl;
    std::cout << "Options: " << std::endl;
    std::cout << "    -h                          " << help_message << std::endl;
    std::cout << "    -gs \"<grpc address>\"      " << grpc_address_message << std::endl;
}

void testDoPipelineStreamInfer() {
    AnalyticsConfigParam param;

    std::vector<Rectangle> rects = {{0, 0, 320, 320}, {0, 320, 320, 320}, {320, 0, 320, 320}, {320, 320, 320, 320}};
    for (auto r : rects) {
        vcaa::grpcService::ROI *new_roi = param.add_crop_rect();
        new_roi->set_x(r.x);
        new_roi->set_y(r.y);
        new_roi->set_w(r.w);
        new_roi->set_h(r.h);
    }

    param.set_num_infer_requests(0);

    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(FLAGS_gs, grpc::InsecureChannelCredentials());
    VCAAClient vcaaClient(channel);

    const char *model = "yolov3";
    if (!vcaaClient.CreateVideoAnalyticsService(model, param)) {
        LOG(ERROR) << "Failed to create analytics service!" << std::endl;
        return;
    }
}

int main(int argc, char **argv) {
    gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
    FLAGS_colorlogtostderr = true;
    google::SetStderrLogging(google::INFO);
    google::InitGoogleLogging("agent-client");
    if (!CheckCommandLine()) {
        LOG(ERROR) << "Failed to start the client!" << std::endl;
        showUsage();
        return 1;
    }

    testDoPipelineStreamInfer();

    return 0;
}

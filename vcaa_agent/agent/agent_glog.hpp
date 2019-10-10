// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

//!
//! \file       agent_glog.hpp
//! \brief      agent glog helper
//!
#pragma once
#include <gflags/gflags.h>
#include <glog/logging.h>

namespace vcaa {
namespace agent {

// const std::string LOGDIR = "./logs";
// const std::string MKDIR = std::string("mkdir -p ") + LOGDIR;

class AgentGlogger {
  public:
    AgentGlogger(const char *name) {
        // system(MKDIR.c_str());
        // FLAGS_log_dir = LOGDIR;
        FLAGS_colorlogtostderr = true;
        FLAGS_stop_logging_if_full_disk = true;
        google::SetStderrLogging(google::INFO);
        google::InitGoogleLogging(name);
    }

    AgentGlogger(AgentGlogger &&) = default;
    AgentGlogger(const AgentGlogger &) = default;
    AgentGlogger &operator=(AgentGlogger &&) = default;
    AgentGlogger &operator=(const AgentGlogger &) = default;
    ~AgentGlogger() {
        google::ShutdownGoogleLogging();
    }

  private:
};

} // end of namespace agent
} // end of namespace vcaa

// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

//!
//! \file       agent_flags.hpp
//! \brief      agent flags defines
//!
#pragma once
#include <gflags/gflags.h>

#include <string>
#include <vector>
#include <iostream>

// @brief message for help argument
static const char help_message[] = "Print a usage message.";

// @brief message for grpc host
static const char grpc_host_message[] = "grpc service host, default 0.0.0.0";

// @brief message for grpc port
static const char grpc_port_message[] = "grpc service port, default 8080";

// @brief message for storage system
static const char storage_message[] = "File storage system.";

// @brief message for agent id information
static const char agent_id_message[] = "agent id.";

DECLARE_bool(h);

DECLARE_string(gh);

DECLARE_int32(gp);

DECLARE_string(s);

DECLARE_string(id);

inline void showUsage()
{
    std::cout << std::endl;
    std::cout << "VCAA agent grpc service [OPTION]" << std::endl;
    std::cout << "Options: " << std::endl;
    std::cout << "    -h                          " << help_message << std::endl;
    std::cout << "    -gh \"<host>\"              " << grpc_host_message << std::endl;
    std::cout << "    -gp \"<port>\"              " << grpc_port_message << std::endl;
    std::cout << "    -s  \"<E5 storage>\"        " << storage_message << std::endl;
    std::cout << "    -id \"<Agent id>\"          " << agent_id_message << std::endl;
}

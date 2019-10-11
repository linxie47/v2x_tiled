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

// @brief message for grpc url
static const char grpc_address_message[] = "grpc service url";

// @brief message for rtmp url
static const char rtmp_message[] = "rtmp url";

// @brief message for local image file path
static const char image_message[] = "image file path";

// @brief message for local config file path
static const char config_message[] = "config file path";

// @brief message for deadline time
static const char deadline_ms_message [] = "Deadline in milliseconds.";

// @brief message for unload time
static const char unload_ms_message[] = "Unload the pipeline in milliseconds.";

// @brief message for image number
static const char image_num_message[] = "Image test number.";

static const char image_format_message[] = "Image format: rgb, rgba, bgr, bgra, jpg, png, bmp";

DECLARE_bool(h);

DECLARE_string(gs);

DECLARE_string(rtmp);

DECLARE_string(image);

DECLARE_string(cf);

DECLARE_int32(deadline_ms);

DECLARE_int32(unload_ms);

DECLARE_int32(image_num);

DECLARE_string(image_format);
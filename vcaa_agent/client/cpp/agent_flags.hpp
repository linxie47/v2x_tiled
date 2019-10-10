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
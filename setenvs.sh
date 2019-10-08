#!/bin/bash
#
# Copyright (C) 2018-2019 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

workspace=`pwd`
libpath="$workspace/vcaa_agent/temp"

export LD_LIBRARY_PATH=$libpath/c-ares/lib:$libpath/gflags/lib:$libpath/glog/lib:$libpath/grpc/lib:$libpath/protobuf/lib:$libpath/zlib/lib:$LD_LIBRARY_PATH

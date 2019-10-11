#!/bin/bash
#
# Copyright (C) 2018-2019 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

workspace=`pwd`
cd $workspace/vcaa_agent/third_party/ffmpeg

./configure \
    --prefix=$workspace/vcaa_agent/temp/ffmpeg \
    --extra-libs="-lpthread -lm" \
    --enable-shared \
    --enable-gpl    \
    --enable-openssl \
    --enable-nonfree \
    --enable-libdrm  \
    --disable-doc --disable-htmlpages --disable-manpages --disable-podpages --disable-txtpages \
    --enable-libx264 \
    --enable-libx265 \
    --enable-librdkafka \
    --enable-libjson_c \
    --enable-libinference_engine_c_api

make -j16 && make install


#!/bin/bash

username=$1
workspace=`pwd`

if [[ ! $username ]]; then
    echo "usage: $0 <desktop_user_id>"
    exit
fi

sed -i 's/sys_mediaci/'$username'/g' .gitmodules

git submodule init
git submodule update --remote

cd $workspace/vcaa_agent/third_party/glog
git checkout v0.4.0
git pull origin v0.4.0

cd $workspace/vcaa_agent/third_party/grpc
git checkout v1.24.0
git submodule update --init

cd $workspace/vcaa_agent/third_party/grpc/third_party/protobuf
git checkout v3.9.2
git pull origin v3.9.2

rm -rf  $workspace/vcaa_agent/third_party/grpc/third_party/upb/.gitmodules

sed -i 's/'$username'/sys_mediaci/g' .gitmodules

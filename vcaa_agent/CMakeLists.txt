# Copyright (C) 2018-2019 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.8)

# Project
project(VCAA_AGENT)

include(ExternalProject)

set(INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/temp")
file(MAKE_DIRECTORY  ${INSTALL_DIR})

if(ENABLE_THIRDPARTY)
  # Builds c-ares project from the git submodule.
  # Note: For all external projects, instead of using checked-out code, one could
  # specify GIT_REPOSITORY and GIT_TAG to have cmake download the dependency directly,
  # without needing to add a submodule to your project.
  ExternalProject_Add(c-ares
    PREFIX c-ares
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/cares/cares"
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_CXX_FLAGS=-fPIC"
    CMAKE_CACHE_ARGS
	  -DCARES_SHARED:BOOL=ON
	  -DCARES_STATIC:BOOL=ON
	  -DCARES_STATIC_PIC:BOOL=ON
	  -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}/c-ares
  )

  # Builds protobuf project from the git submodule.
  ExternalProject_Add(protobuf
    PREFIX protobuf
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/protobuf/cmake"
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_CXX_FLAGS=-fPIC"
    CMAKE_CACHE_ARGS
	  -Dprotobuf_BUILD_TESTS:BOOL=OFF
	  -Dprotobuf_WITH_ZLIB:BOOL=OFF
	  -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
	  -DBUILD_SHARED_LIBS:BOOL=ON
	  -DCMAKE_SKIP_INSTALL_RPATH:BOOL=ON
	  -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}/protobuf
  )

  # Builds zlib project from the git submodule.
  ExternalProject_Add(zlib
    PREFIX zlib
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/zlib"
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_CXX_FLAGS=-fPIC"
    CMAKE_CACHE_ARGS
	  -DBUILD_SHARED_LIBS:BOOL=ON
	  -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}/zlib
  )

  # the location where protobuf-config.cmake will be installed varies by platform
  if (WIN32)
    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${INSTALL_DIR}/protobuf/cmake")
  else()
    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${INSTALL_DIR}/protobuf/lib/cmake/protobuf")
  endif()

  # if OPENSSL_ROOT_DIR is set, propagate that hint path to the external projects with OpenSSL dependency.
  set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "")
  if (OPENSSL_ROOT_DIR)
    set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "-DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}")
  endif()

  # Builds gRPC based on locally checked-out sources and set arguments so that all the dependencies
  # are correctly located.
  ExternalProject_Add(grpc
    PREFIX grpc
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc"
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_CXX_FLAGS=-fPIC"
    CMAKE_CACHE_ARGS
	  -DgRPC_INSTALL:BOOL=ON
	  -DBUILD_SHARED_LIBS:BOOL=ON
	  -DgRPC_BUILD_TESTS:BOOL=OFF
	  -DgRPC_PROTOBUF_PROVIDER:STRING=package
	  -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
	  -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
	  -DgRPC_ZLIB_PROVIDER:STRING=package
	  -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
	  -DgRPC_CARES_PROVIDER:STRING=package
	  -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
	  -DgRPC_SSL_PROVIDER:STRING=package
	  ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
	  -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}/grpc
    DEPENDS c-ares protobuf zlib
  )

  # Builds gflags project from the git submodule.
  ExternalProject_Add(gflags
    PREFIX gflags
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/gflags"
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_CXX_FLAGS=-fPIC"
    CMAKE_CACHE_ARGS
	  -DBUILD_SHARED_LIBS:BOOL=ON
	  -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}/gflags
  )

  # Builds glog project from the git submodule.
  ExternalProject_Add(glog
    PREFIX glog
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glog"
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_CXX_FLAGS=-fPIC"
    CMAKE_CACHE_ARGS
	  -DBUILD_SHARED_LIBS:BOOL=ON
	  -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}/glog
  )

endif()

if(ENABLE_AGENT)
  # Build the agent projects itself using a CMakeLists.txt that assumes all the dependencies
  # have already been installed.
  # Even though agent is not really an "external project" from perspective of this build,
  # we are still importing it using ExternalProject_Add because that allows us to use find_package()
  # to locate all the dependencies (if we were building agent directly in this build we,
  # we would have needed to manually import the libraries as opposed to reusing targets exported by
  # gRPC and protobuf).
  ExternalProject_Add(agent
    PREFIX agent
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/agent"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/agent"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
	  -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
	  -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
	  -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
	  ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
	  -DgRPC_DIR:PATH=${INSTALL_DIR}/grpc/lib/cmake/grpc
	  -Dgflags_DIR:PATH=${INSTALL_DIR}/gflags/lib/cmake/gflags
	  -Dglog_DIR:PATH=${INSTALL_DIR}/glog/lib/cmake/glog
    DEPENDS protobuf grpc gflags glog cppnetlib
  )
endif()

if(ENABLE_AGENT_CLIENT)
  # Build the agent client projects itself using a CMakeLists.txt that assumes all the dependencies
  # have already been installed.
  # Even though agent is not really an "external project" from perspective of this build,
  # we are still importing it using ExternalProject_Add because that allows us to use find_package()
  # to locate all the dependencies (if we were building agent directly in this build we,
  # we would have needed to manually import the libraries as opposed to reusing targets exported by
  # gRPC and protobuf).
  ExternalProject_Add(agent-client
    PREFIX agent
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/client/cpp"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/client/cpp"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
	  -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
	  -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
	  -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
	  ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
	  -DgRPC_DIR:PATH=${INSTALL_DIR}/grpc/lib/cmake/grpc
	  -Dgflags_DIR:PATH=${INSTALL_DIR}/gflags/lib/cmake/gflags
	  -Dglog_DIR:PATH=${INSTALL_DIR}/glog/lib/cmake/glog
    DEPENDS protobuf grpc gflags glog
  )
endif()

if(ENABLE_AGENT_TEST)
  ExternalProject_Add(agent-integration-gtest
    PREFIX agent
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/integration"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/test/integration"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
        -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
        -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DgRPC_DIR:PATH=${INSTALL_DIR}/grpc/lib/cmake/grpc
        -Dgflags_DIR:PATH=${INSTALL_DIR}/gflags/lib/cmake/gflags
        -Dglog_DIR:PATH=${INSTALL_DIR}/glog/lib/cmake/glog
    DEPENDS protobuf grpc gflags glog
  )
endif()

if(ENABLE_EXAMPLES)
  ExternalProject_Add(image-infer
    PREFIX examples
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples/image_infer"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/image_infer"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
        -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
        -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DgRPC_DIR:PATH=${INSTALL_DIR}/grpc/lib/cmake/grpc
        -Dgflags_DIR:PATH=${INSTALL_DIR}/gflags/lib/cmake/gflags
        -Dglog_DIR:PATH=${INSTALL_DIR}/glog/lib/cmake/glog
    DEPENDS protobuf grpc gflags glog
  )

  ExternalProject_Add(stream-infer
    PREFIX examples
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples/stream_infer"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/stream_infer"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
        -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
        -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DgRPC_DIR:PATH=${INSTALL_DIR}/grpc/lib/cmake/grpc
        -Dgflags_DIR:PATH=${INSTALL_DIR}/gflags/lib/cmake/gflags
        -Dglog_DIR:PATH=${INSTALL_DIR}/glog/lib/cmake/glog
    DEPENDS protobuf grpc gflags glog
  )

  ExternalProject_Add(image-infer-restful
    PREFIX examples
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples/restful_image_infer"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/restful_image_infer"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
        -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
        -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DgRPC_DIR:PATH=${INSTALL_DIR}/grpc/lib/cmake/grpc
        -Dgflags_DIR:PATH=${INSTALL_DIR}/gflags/lib/cmake/gflags
        -Dglog_DIR:PATH=${INSTALL_DIR}/glog/lib/cmake/glog
    DEPENDS protobuf grpc gflags glog
  )
  
  ExternalProject_Add(stream-infer-restful
    PREFIX examples
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples/restful_stream_infer"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/restful_stream_infer"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
        -Dc-ares_DIR:PATH=${INSTALL_DIR}/c-ares/lib/cmake/c-ares
        -DZLIB_ROOT:STRING=${INSTALL_DIR}/zlib
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DgRPC_DIR:PATH=${INSTALL_DIR}/grpc/lib/cmake/grpc
        -Dgflags_DIR:PATH=${INSTALL_DIR}/gflags/lib/cmake/gflags
        -Dglog_DIR:PATH=${INSTALL_DIR}/glog/lib/cmake/glog
    DEPENDS protobuf grpc gflags glog
  )
endif()

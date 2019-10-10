cmake_minimum_required(VERSION 3.8)

#project VCAA_CLIENT
project(VCAA_CLIENT)

####################################
## All next project will use C++11
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
####################################


set(TARGET_NAME "vcaa_client")

if (CMAKE_VERSION VERSION_LESS 2.8.12)
  add_definitions(-Wall)
  add_definitions(-Werror)
  add_definitions(-fstack-protector-strong)
  add_definitions(-Wformat -Wformat-security)
  add_definitions(-fPIE)
  add_definitions(-fPIC)
else()
  add_compile_options(-Wall)
  add_compile_options(-Werror)
  add_compile_options(-fstack-protector-strong)
  add_compile_options(-Wformat -Wformat-security)
  add_compile_options(-fPIE)
  add_compile_options(-fPIC)
endif()

set(CMAKE_SKIP_BUILD_RPATH  TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o0 -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z noexecstack -z relro -z now -pie ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wl,-S")

if(CMAKE_BUILD_TYPE MATCHES "Release")
  set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "-fvisibility=default")
endif()

set(AGENT_THIRD_PATRY_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../../temp")

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Find Gflags installation
find_package(gflags CONFIG REQUIRED)
message(STATUS "Using Gflags ${gflags_VERSION}")
set(_GFLAGS_LIBRARIES gflags)

# Find gLog installation
find_package(glog CONFIG REQUIRED)
message(STATUS "Using glog ${glog_VERSION}")
set(_GLOG_LIBRARIES glog)

set(LINK_LIB pthread dl)

# Proto file
get_filename_component(agent_proto "../../protos/vcaa_agent.proto" ABSOLUTE)
get_filename_component(agent_proto_path "${agent_proto}" PATH)

# Generated sources
set(agent_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/vcaa_agent.pb.cc")
set(agent_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/vcaa_agent.pb.h")
set(agent_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/vcaa_agent.grpc.pb.cc")
set(agent_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/vcaa_agent.grpc.pb.h")

add_custom_command(
      OUTPUT "${agent_proto_srcs}" "${agent_proto_hdrs}" "${agent_grpc_srcs}" "${agent_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${agent_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${agent_proto}"
      DEPENDS "${agent_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# glog can not export include dir and link dir, it is needed to manually add it
include_directories("${AGENT_THIRD_PATRY_FOLDER}/glog/include")
link_directories("${AGENT_THIRD_PATRY_FOLDER}/glog/lib")

message(STATUS "grpc ${_GRPC_GRPCPP_UNSECURE}")
add_executable(${TARGET_NAME} "main.cc" "agent_client.cc"
    ${agent_proto_srcs}
    ${agent_grpc_srcs})
target_link_libraries(${TARGET_NAME}
    ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF}
    ${_GFLAGS_LIBRARIES}
    ${_GLOG_LIBRARIES}
)

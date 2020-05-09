#!/bin/bash

protoc --cpp_out=demoweb/proto_cc \
       --grpc_out=demoweb/proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=demoweb/proto \
       demoweb/proto/*.proto


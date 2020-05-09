#!/bin/bash

protoc --cpp_out=deployment/proto_cc \
       --grpc_out=deployment/proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=deployment/proto \
       deployment/proto/*.proto


#!/bin/bash

protoc --cpp_out=deployment \
       --grpc_out=deployment \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=deployment/proto \
       deployment/proto/*.proto


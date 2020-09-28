#!/bin/bash

mkdir -p proto_cc
mkdir -p client/web_frontend/lib/src/proto_dart

protoc --cpp_out=./proto_cc \
       --grpc_out=./proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=proto \
       `find proto -name *.proto`
       
protoc --python_out=. \
       --grpc_out=. \
       --plugin=protoc-gen-grpc=`which grpc_python_plugin` \
       --proto_path=proto \
       proto/service_node_state.proto \
       proto/node.proto \
       proto/delta.proto

export PATH=$PATH:$HOME/.pub-cache/bin

protoc --dart_out=grpc:client/web_frontend/lib/src/proto_dart \
       --proto_path=proto \
       `find proto -name *.proto`



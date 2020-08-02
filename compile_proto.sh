#!/bin/bash

protoc --cpp_out=./proto_cc \
       --grpc_out=./proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=proto \
       `find proto -name *.proto`

export PATH=$PATH:$HOME/.pub-cache/bin

protoc --dart_out=grpc:client/web_frontend/lib/src/proto_dart \
       --proto_path=proto \
       `find proto -name *.proto`


#!/bin/bash

protoc --cpp_out=demoweb_service/demoweb/proto_cc \
       --grpc_out=demoweb_service/demoweb/proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=proto \
       `find proto -name *.proto`

export PATH=$PATH:$HOME/.pub-cache/bin

protoc --dart_out=client/web_frontend/proto_dart \
       --proto_path=proto \
       `find proto -name *.proto`


#!/bin/bash

protoc --cpp_out=demoweb_service/demoweb/proto_cc \
       --grpc_out=demoweb_service/demoweb/proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=proto \
       `find proto -name *.proto`

protoc --cpp_out=message_queue/message_queue/proto_cc \
       --grpc_out=message_queue/message_queue/proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=proto \
       `find proto -name *.proto`

protoc --cpp_out=message_queue/publisher/proto_cc \
       --grpc_out=message_queue/publisher/proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=proto \
       `find proto -name *.proto`

protoc --cpp_out=message_queue/subscriber/proto_cc \
       --grpc_out=message_queue/subscriber/proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=proto \
       `find proto -name *.proto`

export PATH=$PATH:$HOME/.pub-cache/bin

protoc --dart_out=grpc:client/web_frontend/lib/src/proto_dart \
       --proto_path=proto \
       `find proto -name *.proto`


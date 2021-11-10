#!/bin/bash

mkdir -p ../proto_cc
mkdir -p proto_py
mkdir -p ../client/web_frontend/lib/src/proto_dart

echo "Generating C++ protobuf sources..."
protoc --cpp_out=../proto_cc \
       --grpc_out=../proto_cc \
       --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
       --proto_path=../proto \
       `find ../proto -name *.proto`

echo "Generating Python protobuf sources..."
protoc --python_out=./proto_py \
       --grpc_out=./proto_py \
       --plugin=protoc-gen-grpc=`which grpc_python_plugin` \
       --proto_path=../proto \
       `find ../proto -name *.proto`

export PATH=$PATH:$HOME/.pub-cache/bin

if test -f "$HOME/.pub-cache/bin/protoc-gen-dart"
then
        echo "Generating dart sources..."
        protoc --dart_out=grpc:../client/web_frontend/lib/src/proto_dart \
               --proto_path=../proto \
               `find ../proto -name *.proto`
fi


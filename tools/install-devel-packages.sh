#!/bin/bash

apt install -y curl wget

# sh -c 'wget -qO- https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add -'
# sh -c 'wget -qO- https://storage.googleapis.com/download.dartlang.org/linux/debian/dart_stable.list > /etc/apt/sources.list.d/dart_stable.list'
curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash -

apt update
apt upgrade -y 

apt install -y \
    build-essential \
    clang-format \
    dart \
    git \
    golang-go \
    go-dep \
    libboost-log-dev \
    libboost-thread-dev \
    libcrypto++-dev \
    libcrypt-dev \
    libgrpc++-dev \
    libmlpack-dev \
    libpqxx-dev \
    libprotobuf-dev \
    libsqlite3-dev \
    nodejs \
    postgresql \
    protobuf-compiler \
    protobuf-compiler-grpc \
    python3 \
    python3-grpcio \
    python3-grpc-tools \
    python3-numpy \
    python3-pip \
    python3-protobuf \
    python3-psycopg2 \
    qt5-default

/usr/lib/dart/bin/pub get
/usr/lib/dart/bin/pub global activate webdev
/usr/lib/dart/bin/pub global activate stagehand
/usr/lib/dart/bin/pub global activate protoc_plugin

# GOPATH=~/go ; export GOPATH
# git clone https://github.com/improbable-eng/grpc-web.git $GOPATH/src/github.com/improbable-eng/grpc-web
# pushd $GOPATH/src/github.com/improbable-eng/grpc-web
# dep ensure # after installing dep
# go install ./go/grpcwebproxy # installs into $GOPATH/bin/grpcwebproxy
# popd

pip3 install --upgrade tensorflow

#!/bin/bash

echo "Installing build essentials..."
apt install -y build-essential

echo "Installing C++ gRPC and protobuf development packages..."
apt install -y libprotobuf-dev
apt install -y protobuf-compiler
apt install -y libgrpc++-dev

echo "Installing Qt5 development packages..."
apt install -y qt5-default

echo "Installing crypt development packages..."
apt install -y libcrypt-dev

echo "Installing crypto++ development packages..."
apt install -y libcrypto++-dev


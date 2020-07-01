#!/bin/bash

echo "Installing build essentials..."
sudo apt install -y build-essential

echo "Installing C++ gRPC and protobuf development packages..."
sudo apt install -y libprotobuf-dev
sudo apt install -y protobuf-compiler
sudo apt install -y libgrpc++-dev

echo "Installing Qt5 development packages..."
sudo apt install -y qt5-default

echo "Installing crypt development packages..."
sudo apt install -y libcrypt-dev

echo "Installing crypto++ development packages..."
sudo apt install -y libcrypto++-dev


#!/bin/bash

echo "Installing C++ gRPC and protobuf development packages..."
sudo apt install -y libprotobuf-dev
sudo apt install -y libgrpc++-dev

echo "Installing crypt development packages..."
sudo apt install -y libcrypt-dev

echo "Installing crypto++ development packages..."
sudo apt install -y libcrypto++-dev


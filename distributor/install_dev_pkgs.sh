#!/bin/bash

echo "Installing C++ gRPC and protobuf development packages..."
sudo apt install -y libprotobuf-dev
sudo apt install -y libgrpc++-dev

echo "Installing sqlite3 packages..."
sudo apt install -y libsqlite3-dev


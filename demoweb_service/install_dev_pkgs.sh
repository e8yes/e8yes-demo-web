#!/bin/bash

echo "Installing build essentials..."
sudo apt install -y build-essential

echo "Installing postgres client development packages..."
sudo apt install -y libpqxx-dev

echo "Installing C++ gRPC development packages..."
sudo apt install -y libgrpc++-dev

echo "Installing Qt5 development packages..."
sudo apt install -y qt5-default

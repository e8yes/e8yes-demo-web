#!/bin/bash

echo "Installing C++ protobuf development packages..."
sudo apt install -y libprotobuf-dev

echo "Installing python numpy packages..."
sudo apt install -y python3-numpy

echo "Installing python tensorflow packages..."
sudo pip3 install --upgrade tensorflow



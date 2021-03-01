#!/bin/bash

echo "Installing C++ protobuf development packages..."
sudo apt install -y libprotobuf-dev

echo "Installing mlpack development packages..."
sudo apt install -y libmlpack-dev

echo "Installing python numpy packages..."
sudo apt install -y python3-numpy

echo "Installing python psycopg2 packages..."
sudo apt install -y python3-psycopg2

echo "Installing python tensorflow packages..."
sudo pip3 install --upgrade tensorflow


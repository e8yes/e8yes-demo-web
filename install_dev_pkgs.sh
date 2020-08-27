#!/bin/bash

echo "Updating APT packages..."
sudo apt update
sudo apt upgrade -y 

echo "Installing build essentials..."
sudo apt install -y build-essential

echo "Installing Protobuf compiler..."
sudo apt install -y protobuf-compiler
sudo apt install -y protobuf-compiler-grpc

echo "Installing Qt5 development packages..."
sudo apt install -y qt5-default

cd common && ./install_dev_pkgs.sh && cd ..
cd script && ./install_dev_pkgs.sh && cd ..
cd postgres && ./install_dev_pkgs.sh && cd ..
cd demoweb_service && ./install_dev_pkgs.sh && cd ..
cd keygen && ./install_dev_pkgs.sh && cd ..
cd message_queue && ./install_dev_pkgs.sh && cd ..
cd distributor && ./install_dev_pkgs.sh && cd ..
cd client/web_frontend && ./install_dev_pkgs.sh && cd ../..
cd third_party/grpcwebproxy && ./install_dev_pkgs.sh && cd ../..
cd postgres && ./push_schema.sh && cd ..


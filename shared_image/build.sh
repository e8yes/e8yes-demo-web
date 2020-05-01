#!/bin/bash

sudo docker pull ubuntu:19.10

echo "Building shared image grpc_service..."
sudo docker build -t="grpc_service" grpc_service


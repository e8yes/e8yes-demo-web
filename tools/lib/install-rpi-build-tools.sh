#!/bin/bash

sudo apt update
sudo apt upgrade -y

sudo apt install -y python3 python3-grpcio python3-protobuf

if ! dpkg -s docker-ce-cli >/dev/null 2>&1
then
	echo "Installing relevant packages required by Docker..."
	sudo apt install -y \
	    apt-transport-https \
	    ca-certificates \
	    curl \
	    gnupg-agent \
	    software-properties-common

	echo "Installing Docker..."
	curl -sSL https://get.docker.com | sh
else
	echo "Docker has already been installed. Skip."
fi

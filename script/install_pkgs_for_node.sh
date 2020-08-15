#!/bin/bash

sudo apt update
sudo apt upgrade -y

sudo apt install -y git

if ! dpkg -s docker-ce-cli >/dev/null 2>&1
then
	echo "Installing relevant packages for docker..."
	sudo apt install -y \
	    apt-transport-https \
	    ca-certificates \
	    curl \
	    gnupg-agent \
	    software-properties-common

	echo "Installing docker..."
	curl -sSL https://get.docker.com | sh
else
	echo "Docker has already been installed. Skip."
fi


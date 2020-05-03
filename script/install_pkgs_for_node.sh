#!/bin/bash

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

echo "Installing kubernetes..."
curl -s https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add - && \
echo "deb http://apt.kubernetes.io/ kubernetes-xenial main" | sudo tee /etc/apt/sources.list.d/kubernetes.list && \
sudo apt-get update -q && sudo apt-get install -qy kubeadm


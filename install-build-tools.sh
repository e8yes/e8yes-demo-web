#!/bin/bash

if ! dpkg -s docker-ce-cli >/dev/null 2>&1
then
	echo "Installing relevant packages for docker..."
	sudo apt install -y \
	    apt-transport-https \
	    ca-certificates \
	    curl \
	    gnupg-agent \
	    software-properties-common
	
	echo "Add docker apt repository..."
	curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
	sudo add-apt-repository \
	   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
	   $(lsb_release -cs) \
	   stable"
	sudo apt update
	
	echo "Installing docker..."
	sudo apt install -y docker-ce docker-ce-cli containerd.io
else
	echo "Docker has already been installed. Skip."
fi




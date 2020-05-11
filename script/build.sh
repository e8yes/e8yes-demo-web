#!/bin/bash

host=""
if [ "$#" -eq 1 ]; then
	host="$1:5000/"
fi

sudo docker build -t="demoweb_dev_env" ..
sudo docker build -t=$host"demowebservice" ../demoweb_service


#!/bin/bash

sudo docker pull ubuntu:19.10
sudo docker build -t="demoweb_dev_env" ..
sudo docker build -t="demowebservice" ../demoweb_service


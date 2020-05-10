#!/bin/bash

sudo docker pull ubuntu:20.04
sudo docker build -t="demoweb_dev_env" .
sudo docker build -t="demowebservice" demoweb_service


#!/bin/bash

system_name=$1

sudo docker container rm --force \
    `sudo docker container ls -q --filter name=demoweb_system`
sudo docker run \
    --name=demoweb_system \
    --restart=always \
    --network=host \
    --hostname=`hostname` \
    --volume=`echo ~`/demoweb-data:/host/home \
    --detach \
    $system_name

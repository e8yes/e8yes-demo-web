#!/bin/bash

set -e

echo "Building shared images..."
cd ../shared_image
./build.sh
cd ../script

echo "Building demoweb service image..."
cd ../demoweb_service
./build.sh
cd ../script


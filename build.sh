#!/bin/bash

set -e

echo "Installing build tools..."
./install-build-tools.sh

echo "Building shared images..."
cd shared_image
./build.sh
cd ..

echo "Building demoweb service image..."
cd demoweb_service
./build.sh
cd ..


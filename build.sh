#!/bin/bash

set -e

echo "Installing build tools..."
./install-build-tools.sh

echo "Building demoweb service..."
cd service
./build.sh
cd ..


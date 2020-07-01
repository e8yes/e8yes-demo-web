#!/bin/bash

echo "Installing Dart SDK and compiler..."
sh -c 'wget -qO- https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add -'
sudo sh -c 'wget -qO- https://storage.googleapis.com/download.dartlang.org/linux/debian/dart_stable.list > /etc/apt/sources.list.d/dart_stable.list'
sudo apt update
sudo apt install -y dart

echo "Installing Dart packages..."
/usr/lib/dart/bin/pub get
/usr/lib/dart/bin/pub global activate webdev
/usr/lib/dart/bin/pub global activate stagehand
/usr/lib/dart/bin/pub global activate protoc_plugin

echo "Installing NodeJS..."
curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash -
sudo apt install -y nodejs

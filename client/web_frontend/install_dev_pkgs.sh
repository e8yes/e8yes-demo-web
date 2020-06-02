#!/bin/bash

echo "Installing Dart SDK and compiler..."
sh -c 'wget -qO- https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add -'
sh -c 'wget -qO- https://storage.googleapis.com/download.dartlang.org/linux/debian/dart_stable.list > /etc/apt/sources.list.d/dart_stable.list'
apt update
apt install -y dart

echo "Installing Dart packages..."
/usr/lib/dart/bin/pub get
/usr/lib/dart/bin/pub global activate webdev
/usr/lib/dart/bin/pub global activate stagehand


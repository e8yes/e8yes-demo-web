#!/bin/bash

echo "Installing postgres database engine..."
apt install -y postgresql

echo "Launching postgres cluster..."
systemctl restart postgresql

echo "Installing posgres C++ client library..."
apt install -y libpqxx-dev


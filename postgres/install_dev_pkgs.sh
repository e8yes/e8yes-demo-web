#!/bin/bash

echo "Installing postgres database engine..."
sudo apt install -y postgresql

echo "Launching postgres cluster..."
sudo service postgresql start

echo "Installing posgres C++ client library..."
sudo apt install -y libpqxx-dev


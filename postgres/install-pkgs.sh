#!/bin/bash

echo "Installing postgres server..."
sudo apt install -y postgresql

echo "Launching postgres cluster..."
sudo systemctl start postgresql

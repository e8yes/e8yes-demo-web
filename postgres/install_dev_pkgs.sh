#!/bin/bash

echo "Installing postgres server..."
apt install -y postgresql

echo "Launching postgres cluster..."
systemctl start postgresql


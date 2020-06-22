#!/bin/bash

echo "Installing postgres database engine..."
apt install -y postgresql

echo "Copying config files..."
cp ./conf/11/pg_hba.conf /etc/postgresql/11/main/pg_hba.conf
cp ./conf/11/postgresql.conf /etc/postgresql/11/main/postgresql.conf

echo "Launching postgres cluster..."
systemctl restart postgresql

echo "Installing posgres C++ client library..."
apt install -y libpqxx-dev


#!/bin/bash

sudo apt install postgresql

echo "ALTER USER postgres WITH PASSWORD 'password';" | sudo -u postgres psql demoweb

echo "CREATE DATABASE demoweb WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';" | sudo -u postgres psql postgres
cat 'schema.pgsql' | sudo -u postgres psql demoweb

echo "CREATE DATABASE demowebtest WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';" | sudo -u postgres psql postgres
cat 'schema.pgsql' | sudo -u postgres psql demowebtest

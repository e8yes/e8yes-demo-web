#!/bin/bash

echo "CREATE DATABASE demoweb WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';" | sudo -u postgres psql postgres
echo "CREATE DATABASE demowebtest WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';" | sudo -u postgres psql postgres

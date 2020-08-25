#!/bin/bash

# Ensure that the postgres db engine is started.
sudo service postgresql start

set +e # The database might have already existed, but that's fine.
echo "CREATE DATABASE demoweb WITH TEMPLATE = template0 ENCODING = 'UTF8';" | sudo -u postgres psql postgres
set -e

echo "ALTER DATABASE demoweb OWNER TO postgres;" | sudo -u postgres psql demoweb
echo "ALTER USER postgres WITH PASSWORD 'password';" | sudo -u postgres psql demoweb
cat 'schema.pgsql' | sudo -u postgres psql demoweb



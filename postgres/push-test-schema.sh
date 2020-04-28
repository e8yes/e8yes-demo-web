#!/bin/bash

POSTGRESQL_VERSION=11

# Kill existing connections and ensures that the database server is started.
sudo pg_ctlcluster $POSTGRESQL_VERSION main restart

set +e # The database might have already existed, but that's fine.
echo "CREATE DATABASE demowebtest WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_GB.UTF-8' LC_CTYPE = 'en_GB.UTF-8';" | sudo -u postgres psql postgres
set -e

echo "ALTER USER postgres WITH PASSWORD 'password';" | sudo -u postgres psql demowebtest

cat 'schema.pgsql' | sudo -u postgres psql demowebtest



#!/bin/bash

sudo pg_ctlcluster 11 main start
echo "ALTER USER postgres WITH PASSWORD 'password';" | sudo -u postgres psql demoweb
cat 'schema.pgsql' | sudo -u postgres psql demoweb
cat 'schema.pgsql' | sudo -u postgres psql demowebtest

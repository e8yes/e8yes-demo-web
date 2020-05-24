#!/bin/bash

echo "Installing postgres database engine and citus data extension..."
curl https://install.citusdata.com/community/deb.sh | bash
apt install -y postgresql-12-citus-9.3

echo "Launching postgres cluster..."
systemctl start postgresql


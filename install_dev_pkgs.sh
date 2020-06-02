#!/bin/bash

cd postgres && ./install_dev_pkgs.sh && cd ..
cd deployment_service && ./install_dev_pkgs.sh && cd ..
cd demoweb_service && ./install_dev_pkgs.sh && cd ..
cd keygen && ./install_dev_pkgs.sh && cd ..
cd client/web_frontend && ./install_dev_pkgs.sh && cd ../..


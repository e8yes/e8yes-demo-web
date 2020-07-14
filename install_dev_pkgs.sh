#!/bin/bash

cd postgres && ./install_dev_pkgs.sh && cd ..
cd demoweb_service && ./install_dev_pkgs.sh && cd ..
cd keygen && ./install_dev_pkgs.sh && cd ..
cd client/web_frontend && ./install_dev_pkgs.sh && cd ../..
cd third_party/grpcwebproxy && ./install_dev_pkgs.sh && cd ../..
cd postgres && ./push_schema.sh && cd ..
cd message_queue && ./install_dev_pkgs.sh && cd ../..


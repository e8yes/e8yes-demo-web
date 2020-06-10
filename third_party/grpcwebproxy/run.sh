#!/bin/bash

~/go/bin/grpcwebproxy --backend_addr=localhost:50051 --run_tls_server=false --server_http_debug_port=8000 --allow_all_origins


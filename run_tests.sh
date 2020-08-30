#!/bin/bash

set -e

echo "============================================"
echo "Starting a local postgres server for testing."
cd postgres
./push_schema.sh
cd ..

n_cores=$(nproc --all)

echo "============================================"
echo "Building binaries"
cd build && ./bootstrap.sh
make -j $n_cores
./collect_binaries.sh
cd ..

set +e

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:build/bin
test_execs=($(find build/bin -name test_* -type f -executable))

echo "============================================"
echo "Running the test suite"
num_tests=0
for test_exec in ${test_execs[@]}
do
	($test_exec)
	ret_val=$?
	if [ $ret_val -ne 0 ]
	then
		echo "============================================"
		echo "Test $test_exec failed."
		exit 1
	fi
	((num_tests = num_tests + 1))
done

echo "Test suite ($num_tests collections) has passed."


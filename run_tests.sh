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
cd ..


echo "============================================"
echo "Adding the following DLLs to LD_LIBRARY_PATH"
dlls=($(find . -regextype sed -regex ".*/build/\(demoweb_service/demoweb\|postgres/query_runner\|common/container\|keygen\|third_party/base64\)"))
for dll in ${dlls[@]}
do
	echo $dll
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$dll
done

echo "============================================"
echo "Creating test suite from the following tests"
test_execs=($(find build -regextype sed -regex ".*_test" -type f))
for test_exec in ${test_execs[@]}
do
	echo $test_exec
done

set +e

echo "============================================"
echo "Running the test suite"
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
done

echo "============================================"
echo "Test suite has passed."


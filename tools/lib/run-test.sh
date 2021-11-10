#!/bin/bash

if [ -z $1 ]; then
    exit -1
fi

pushd ..

test_binary_name="./build/bin/$1"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./build/bin

($test_binary_name)
ret_val=$?

popd

exit $ret_val

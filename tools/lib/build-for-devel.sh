#!/bin/bash

pushd ../build

./bootstrap.sh
make -j $n_cores

rm -rf bin
mkdir -p bin

find . -name *.so* -not -path "./bin/*" -exec cp -P {} bin/ \;
find . -type f -executable -not -path "./bin/*" -name *main -exec cp -f {} bin/ \;
find . -type f -executable -not -path "./bin/*" -name _test_* -exec cp -f {} bin/ \;

popd

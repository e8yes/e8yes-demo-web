#!/bin/bash

rm -rf bin
mkdir -p bin

find . -name *.so -not -path "./bin/*" -exec cp -f {} bin/ \;
find . -name *.1 -not -path "./bin/*" -exec cp -f {} bin/ \;
find . -type f -executable -not -path "./bin/*" -name *main -exec cp -f {} bin/ \;
find . -type f -executable -not -path "./bin/*" -name _test_* -exec cp -f {} bin/ \;


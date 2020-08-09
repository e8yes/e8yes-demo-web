rm -rf bin
mkdir -p bin

find . -name *.so -exec cp -f {} bin/ \;
find . -name *.1 -exec cp -f {} bin/ \;
find . -name *main -exec cp -f {} bin/ \;

rm bin/Makefile.*


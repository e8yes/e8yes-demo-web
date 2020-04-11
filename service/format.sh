#!/bin/bash

cd demoweb && find . -name *.cc -or -name *.h -exec \
../tools/iwyu -Xiwyu --no_comments -Xiwyu --no_fwd_decls -x c++ -std=c++17 {} \;

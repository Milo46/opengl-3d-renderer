#!/usr/bin/env bash

cd ../crenderr/source/Crenderr/
find . -name '*.cpp' | xargs wc -l
find . -name '*.hpp' | xargs wc -l

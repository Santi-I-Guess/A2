#!/usr/bin/env bash

cppcheck *.cpp include/*.h --check-level=exhaustive -I /usr/include --force

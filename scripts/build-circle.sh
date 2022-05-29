#!/bin/sh

cd sw/vendor/circle-stdlib
./configure -r 3 -o ARM_ALLOW_MULTI_CORE -o REALTIME -o NO_SDHOST
make

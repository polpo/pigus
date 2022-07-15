#!/bin/sh

cd sw/vendor/circle-stdlib
make mrproper
./configure -r 4 -o ARM_ALLOW_MULTI_CORE -o REALTIME -o NO_SDHOST -o NO_BUSY_WAIT
make

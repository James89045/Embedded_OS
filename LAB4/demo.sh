#!/bin/sh

set -x
# set -e

rmmod -f mydev
insmod mydev.ko

./writer yourname &
./reader 192.168.2.101 8000 /dev/mydev

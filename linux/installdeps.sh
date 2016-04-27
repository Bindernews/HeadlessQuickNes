#!/bin/bash
#
# Install dependancies for HQNes
#

if [ $UID -ne 0 ]; then
    echo "Must be run as root"
    exit
fi

if [ "$(which apt-get)" != "" ]; then
    apt-get install libsdl2-dev libsdl2-ttf-dev libluajit-5.1-dev
else
    echo "apt-get not available. Please install libraries manually."
fi

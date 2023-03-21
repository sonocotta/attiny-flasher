#!/bin/bash

DEVICES=/dev/ttyUSB*
PIOENV=bootloader
export PATH=$PATH:~/.platformio/penv/bin 

function bootloader() {
    platformio run --target bootloader --environment $PIOENV
}

while :
do
    if ls $DEVICES 1> /dev/null 2>&1; then
        DEVICE=$(ls $DEVICES | head -n 1)
        echo ""
        echo "Device $DEVICE found, flashing.."
        bootloader;
        echo -n "Flashed, starting again in 3"
        sleep 1
        echo -n "..2"
        sleep 1
        echo -n "..1"
        sleep 1
        echo -n "..0"
    else
        echo -n "Looking for device.."
        while :
        do
            if ls $DEVICE 1> /dev/null 2>&1; then
                break
            else
                echo -n "."
            fi
            sleep 1
        done
    fi
    # echo "bottom loop!"
    # sleep 1
done
#!/bin/bash

DEVICES=/dev/ttyUSB*
PIOENV=atmega328-tiny-rev-h-smaller
export PATH=$PATH:~/.platformio/penv/bin 

function flash() {
    platformio run --target upload --environment $PIOENV
}

while :
do
    if ls $DEVICES 1> /dev/null 2>&1; then
        DEVICE=$(ls $DEVICES | head -n 1)
        echo ""
        echo "Device $DEVICE found, flashing.."
        flash;
        echo -n "Flashed, waiting for disconnect.."
        while :
        do 
            if ls $DEVICE 1> /dev/null 2>&1; then
                echo -n "."
            else
                echo " disconnected!"
                break
            fi
            sleep 1
        done
    else
        echo -n "Looking for device.."
        while :
        do
            if ls $DEVICES 1> /dev/null 2>&1; then
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
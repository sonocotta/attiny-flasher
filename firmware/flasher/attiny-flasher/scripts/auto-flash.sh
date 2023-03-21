#!/bin/bash

DEVICES=/dev/ttyUSB*
PIOENV=atmega328-tiny-rev-h
PIOTESTENV=attiny85-no-serial
export PATH=$PATH:~/.platformio/penv/bin 

function flash() {
    while :
    do 
        if platformio run --target upload --environment $PIOENV; then
            echo " FLASHER firmware flashed!"
            break
        else
            echo "Failed, try again.."
        fi
        sleep 2
    done
}

function test() {
    pushd ../attiny-serial-demo

    while :
    do 
        if platformio run --target upload --environment $PIOTESTENV; then
            echo " TEST firmware flashed!"
            break
        else
            echo "Failed, try again.."
        fi
        sleep 2
    done

    popd
}

while :
do
    if ls $DEVICES 1> /dev/null 2>&1; then
        DEVICE=$(ls $DEVICES | head -n 1)
        echo ""
        echo "Device $DEVICE found, flashing.."
        flash;

        echo -n "Flashed, running flashing test (Programm switch to ON).."
        sleep 3
        test;

        echo -n "Tested, waiting for disconnect.."
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
        echo -n "Looking for device (Programm switch to OFF).."
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
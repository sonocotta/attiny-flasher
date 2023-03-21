#!/bin/bash

PIOENV=attiny85-rev-b

export PATH=$PATH:~/.platformio/penv/bin 

function flash() {
    pushd ./..
    
    while :
    do 
        if platformio run --target fuses --environment $PIOENV && platformio run --target upload --environment $PIOENV; then
            echo " firmware flashed!"
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
    echo -n "Press any key to continue..."
    read -n 1
    
    flash;

done
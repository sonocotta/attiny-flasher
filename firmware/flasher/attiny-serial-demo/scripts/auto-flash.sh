#!/bin/bash

DEVICES=/dev/ttyUSB*
PIOENVHV=attiny85-no-serial
PIOENVLV=attiny85-sw-serial
export PATH=$PATH:~/.platformio/penv/bin 

function flash_fuses_reset_enabled() {
    platformio run --target fuses --environment $PIOENVLV
}

function flash_fuses_reset_disabled() {
    platformio run --target fuses --environment $PIOENVHV
}

function flash_blinker_reset_enabled() {
    platformio run --target upload --environment $PIOENVLV
}

function flash_blinker_reset_disabled() {
    platformio run --target upload --environment $PIOENVHV
}

while :
do
    if ls $DEVICES 1> /dev/null 2>&1; then
        DEVICE=$(ls $DEVICES | head -n 1)
        echo ""
        echo "Device $DEVICE found, flashing blinker"
        flash_blinker_reset_disabled;
        echo "Flashing fuses (reset disabled).."
        flash_fuses_reset_disabled;
        
        read -p "Verify (1) LEDs, enable HV mode and press a key.." -n1 -s
     
        echo "Flashing fuses (reset re-enabled).."
        flash_fuses_reset_enabled;
        echo "Flashing blicker with serial (reset enabled).."
        flash_blinker_reset_enabled;
        echo "Verify (1) LEDs, (2) RESET buttons, (3) Serial on the screen and disconnect.."
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
        echo -n "Looking for device .."
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
    sleep 1
done
#include <Arduino.h>
#include <TinyWireM.h>
#include <radio.h>
#include <RDA5807M.h>

#ifdef _SW_SERIAL_
#include "ATtinySerialOut.h"

#define PIN_RX -1
#define PIN_TX 4
#define SERIAL_BAUD 115200
#endif

// ----- Fixed settings here. -----

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION  9920            ///< The station that will be tuned by this sketch is 89.30 MHz.
#define FIX_VOLUME   4               ///< The volume that will be set by this sketch is level 4.

RDA5807M radio;    // Create an instance of Class for RDA5807M Chip

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup() {
  
    #if defined(Serial)
    Serial.begin(SERIAL_BAUD);
    Serial.println(F("Radio..."));
    delay(200);
    #endif

  // Initialize the Radio 
  radio.init();

  // Enable information to the Serial port
  //radio.debugEnable();

  // Set all radio setting to the fixed values.
  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  radio.setVolume(FIX_VOLUME);
  radio.setMono(false);
  radio.setMute(false);
} // setup


/// show the current chip data every 3 seconds.
void loop() {
  char s[12];
  radio.formatFrequency(s, sizeof(s));
  Serial.print("Station:"); 
  Serial.println(s);
  
  Serial.print("Radio:"); 
  radio.debugRadioInfo();
  
  Serial.print("Audio:"); 
  radio.debugAudioInfo();

  delay(3000);
} // loop

// End.
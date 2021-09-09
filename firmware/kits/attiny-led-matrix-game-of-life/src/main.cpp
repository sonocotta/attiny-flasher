#include <Arduino.h>
#include <util/delay.h>
#include "main.h"

#if defined(HW_SERIAL) || defined(SW_SERIAL) || defined(LIGHT_SERIAL)
#define SERIAL_EN
#define SERIAL_BAUD 115200
#endif

#define TIME_PER_STEP_MS 100 

#define GRID_SIZE_X 16
#define GRID_SIZE_Y 8

uint8_t grid[GRID_SIZE_X][GRID_SIZE_Y] = {};

#ifdef SERIAL_EN
ConsoleVisualizer* console = new ConsoleVisualizer(GRID_SIZE_X, GRID_SIZE_Y, grid);
#endif
LedVisualizer* led         = new LedVisualizer(lc, GRID_SIZE_X, GRID_SIZE_Y, grid);
LifeGame* game             = new LifeGame(GRID_SIZE_X, GRID_SIZE_Y, grid);

void init_led() {
  int devices = lc->getDeviceCount();
  for(int address = 0; address < devices; address++) {
    lc->shutdown(address,false);
    lc->setIntensity(address, 8);
    lc->clearDisplay(address);
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Hello"));

  _delay_ms(1000);

  init_led();
  game->init();
}

void loop() { 
    #ifdef SERIAL_EN
    console->display();
    #endif
    led->display();
    
    _delay_ms(TIME_PER_STEP_MS);
    game->doStep();
}

void _loop() { 
  //read the number cascaded devices
  int devices=lc->getDeviceCount();
  
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    for(int row=0;row<8;row++) {
      for(int col=0;col<8;col++) {
        lc->setLed(address,row,col,true);
        delay(100);
        lc->setLed(address,row,col,false);
      }
    }
  }
}
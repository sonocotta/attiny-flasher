#include <stdint.h>
#include "LifeGame.h"
#ifdef LIGHT_SERIAL
#include <ATtinySerialOut.h>
#endif
#ifdef SW_SERIAL
#include <SoftwareSerial.h>
#endif

static uint8_t SizeX = 0;
static uint8_t SizeY = 0;
static void* Data;

LifeGame::LifeGame(uint8_t sizeX, uint8_t sizeY, void* data) {
  SizeX = sizeX;
  SizeY = sizeY;
  Data = data;
}

void LifeGame::fillRandom() {
  uint8_t (*p_arr)[SizeY][SizeX] = Data;

  for (uint8_t x = 0; x < SizeX; x++)
    for (uint8_t y = 0; y < SizeY; y++)
     {
        //if ((x == 0 && y == 0) || (x == 7 && y == 7) || (x == 8 && y == 0) /*|| (x == 15 && y == 7)*/)
        //if ((x == y) || (SizeX - x == y + 1))
        //if ((x == 2 && y == 2) || (x == SizeX - 3 && y == SizeY - 3))
        //  (*p_arr)[y][x] = 1;
        //else
        //  (*p_arr)[y][x] = 0;
        (*p_arr)[y][x] = random() % 2;
     }
}

void LifeGame::init() {
  fillRandom();
}

void copyGrid(void* from, void* to) {
  uint8_t (*p_arrs)[SizeY][SizeX] = from;
  uint8_t (*p_arrd)[SizeY][SizeX] = to;
  for (uint8_t x = 0; x < SizeX; x++)
    for (uint8_t y = 0; y < SizeY; y++)
      (*p_arrd)[y][x] = (*p_arrs)[y][x];
}

bool isGridEqual(void* from, void* to) {
  uint8_t (*p_arrs)[SizeY][SizeX] = from;
  uint8_t (*p_arrd)[SizeY][SizeX] = to;
  for (uint8_t x = 0; x < SizeX; x++)
    for (uint8_t y = 0; y < SizeY; y++)
      if ( (*p_arrd)[y][x] != (*p_arrs)[y][x] )
        return false;
  return true;
}

uint8_t gridPopulation(void* arr) {
  uint8_t result = 0;
  uint8_t (*p_arrs)[SizeY][SizeX] = arr;
  for (uint8_t x = 0; x < SizeX; x++)
    for (uint8_t y = 0; y < SizeY; y++)
      result += (*p_arrs)[y][x];
  return result;
}

void LifeGame::doStep() { 
  step++;

  uint8_t (*p_arr1)[SizeY][SizeX] = Data;
  void* p_arrx = new uint8_t[SizeX * SizeY]; 
  uint8_t (*p_arr2)[SizeY][SizeX] = p_arrx;
  copyGrid(p_arr1, p_arr2);
  
  uint8_t alive = 0;
  for (uint8_t x = 0; x < SizeX; x++)
  {
    for(uint8_t y = 0; y < SizeY; y++)
    {
      alive = 0;
      for(int8_t c = -1; c < 2; c++)
      {
        for(int8_t d = -1; d < 2; d++)
          if (
            !(c == 0 && d == 0) 
            && ((x + c) >= 0)
            && ((y + d) >= 0)
            && ((x + c) < SizeX)
            && ((y + d) < SizeY)
          )
          if((*p_arr2)[y + d][x + c] == 1) 
            ++alive;
          
      }

      if (alive < 2)
        (*p_arr1)[y][x] = 0;
      else if (alive == 3)
        (*p_arr1)[y][x] = 1;
      else if (alive > 3)
        (*p_arr1)[y][x] = 0;
    }
  }

  // check if we're looped
  if (isGridEqual(p_arr1, p_arr2))
    emptyMovesCount++;
  else 
    emptyMovesCount = 0;

  // uint8_t population = gridPopulation(p_arr1);
  // if (population == lastPopulation)
  //   loopMovesCount++;
  // else 
  //   loopMovesCount = 0;
  // lastPopulation = population;

  // check if empty moves count off limit
  if (emptyMovesCount >= emptyMovesThreshold) {
    emptyMovesCount = 0;
    fillRandom();
  }

  // if (loopMovesCount >= loopMovesThreshold) {
  //   loopMovesCount = 0;
  //   fillRandom();
  // }

  delete[] p_arrx;
}
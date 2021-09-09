#ifndef CONSOLEVISU_H
#define CONSOLEVISU_H

#include "LedControl.h"

// class ILifeVisualizer {
//    public:
//     virtual void display();
// };

class ConsoleVisualizer //: public ILifeVisualizer
{
    public:
        ConsoleVisualizer(uint8_t sizeX, uint8_t sizeY, void* data);
        void display();
};

#endif
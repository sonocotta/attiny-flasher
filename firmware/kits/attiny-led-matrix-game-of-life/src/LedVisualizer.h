#ifndef LEDVISU_H
#define LEDVISU_H

#include "LedControl.h"

// class ILifeVisualizer {
//    public:
//     virtual void display();
// };

class LedVisualizer //: public ILifeVisualizer
{
    public:
        LedVisualizer(LedControl* lc, uint8_t sizeX, uint8_t sizeY, void* data);
        void display();
};

#endif
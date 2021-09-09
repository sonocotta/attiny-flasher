#ifndef LIFEGAME_H
#define LIFEGAME_H

class ILifeGame {
   public:
    virtual void doStep();
};

class LifeGame : public ILifeGame
{
    public:
        LifeGame(uint8_t sizeX, uint8_t sizeY, void* data);
        void init();
        void doStep();
    protected:
        uint16_t step = 0;
    private:
        void fillRandom();
        uint8_t emptyMovesCount = 0;
        const uint8_t emptyMovesThreshold = 5;
        
        // uint8_t lastPopulation = 0;
        // uint8_t loopMovesCount = 0;
        // const uint8_t loopMovesThreshold = 10;
};

#endif
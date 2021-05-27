#ifndef _TPI_H_
#define _TPI_H_

// define the instruction set bytes
#define SLD 0x20
#define SLDp 0x24
#define SST 0x60
#define SSTp 0x64
#define SSTPRH 0x69
#define SSTPRL 0x68
#define SKEY 0xE0

#define NVM_PROGRAM_ENABLE 0x1289AB45CDD888FFULL // the ULL means unsigned long long
#define NVMCMD 0x33
#define NVMCSR 0x32
#define NVM_NOP 0x00
#define NVM_CHIP_ERASE 0x10
#define NVM_SECTION_ERASE 0x14
#define NVM_WORD_WRITE 0x1D

#define Tiny4_5 10
#define Tiny9 1
#define Tiny10 1
#define Tiny20 2
#define Tiny40 4

#define TimeOut 1
#define HexError 2
#define TooLarge 3

#define RESET_HIGH digitalWrite(SS, HIGH);
#define RESET_LOW digitalWrite(SS, LOW);

class TPI
{
public:
    static void start();
    static void setLockBits();
    static void dumpMemory();
    static void eraseChip();
    static void setConfig(boolean val);
    // static boolean writeProgram();
    static void checkID();

    static void finish();

protected:

private:
    static uint8_t readCSS(uint8_t address);
    static void writeCSS(uint8_t address, uint8_t value);
  
    static void send_byte(uint8_t data);
    static uint8_t receive_byte(void);  

    static void writeIO(uint8_t address, uint8_t value);
    static uint8_t readIO(uint8_t address);

    static void setPointer(unsigned short address);
    static void send_skey(uint64_t nvm_key);
    
    static void outHex(unsigned int n, char l);

};

#endif
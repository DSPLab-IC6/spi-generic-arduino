#include <SPI.h>

#define GET_REGISTER      0xDE
#define SET_REGISTER      0xDB 

static char registers[0xFF];
static int command;
static int reg_num; 

enum states_protocol {
    processing,
    cmd,
    reg,
    value
};
states_protocol state = processing;

void setup (void) {
    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);

    for (int i = 0; i<0xFF; i++) {
        registers[i] = 0;
    }

    SPI.attachInterrupt();
}


// SPI interrupt routine
ISR (SPI_STC_vect) {
    switch (state) {
        case processing:
            if (SPDR == 0xFA)
                state = cmd;
            break;
        case cmd:
            command = SPDR;
            state = reg;
            break;
        case reg:
            reg_num = SPDR;
            state = value; 
            switch (command) {
                case GET_REGISTER:
                    SPDR = registers[reg_num];
                    break;
            }
            break;
        case value:
            state = processing;
            switch (command) {
                case SET_REGISTER:
                    registers[reg_num] = SPDR;
                    break;
            }
            break;
    };
//exit:
//    __asm__ __volatile__("nop");
}

void loop (void)
{
    
}


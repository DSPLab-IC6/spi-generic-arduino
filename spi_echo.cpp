#include <SPI.h>

#define GET_REGISTER      0xDE
#define SET_REGISTER      0xDB 

volatile static char registers[0xFF];
volatile static int command;
volatile static int reg_num; 

enum states_protocol {
    processing,
    cmd,
    reg,
    value
};
volatile states_protocol state;

void setup (void) {
    state = processing;

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
            if (SPDR == 0xFA) {
                state = cmd;
                for (int i = 0; i<0x04; i++) {
                    registers[i] = 0x00;
                }
            }
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
}

void loop (void)
{
/*    switch (state) {
        case cmd:
            Serial.println("cmd");
            break;
        case reg:
            Serial.println("reg");
            break;
        case value:
            Serial.println("val");
            break;
    };
*/
}


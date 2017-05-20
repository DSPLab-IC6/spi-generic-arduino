#include <SPI.h>

char buf [4];
volatile byte pos;
volatile boolean process_it;

void setup (void) {
    pinMode(MISO, OUTPUT);  
    SPCR |= _BV(SPE);
    
    pos = 0;
    process_it = false;
    for (byte i = 0; i<4; i++) {
        buf[i] = 0;
    }

    SPI.attachInterrupt();
}


// SPI interrupt routine
ISR (SPI_STC_vect) {
    if (!process_it) {  
        buf[pos++] = SPDR;
    }
    if (pos == 2) {
        pos = 0;
        process_it = !process_it;
    }
    if (process_it) {
        SPDR = buf[pos++];
    }
}

void loop (void)
{
    
}


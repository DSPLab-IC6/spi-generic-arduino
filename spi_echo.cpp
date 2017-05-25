#include <SPI.h>

char buf [4];
volatile byte pos;
volatile boolean begin_procssing;
volatile boolean process_it;
volatile unsigned process_cnt;

void setup (void) {
    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);

    pos = 0;
    process_it = false;
    process_cnt = 0;
    for (byte i = 0; i<4; i++) {
        buf[i] = 0;
    }

    SPI.attachInterrupt();
}


// SPI interrupt routine
ISR (SPI_STC_vect) {
    if (SPDR == 0xFA && !begin_procssing) {
        begin_procssing = true;
        goto exit;
    }

    if (begin_procssing) {
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
        if (++process_cnt >= 4) {
            begin_procssing = false;
            process_cnt = 0;
        }
    }
exit:
    __asm__ __volatile__("nop");
}

void loop (void)
{
    
}


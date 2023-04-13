#ifndef PROTOTYPE_FOR_CLEO_H
#define PROTOTYPE_FOR_CLEO_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// SPI pin definitions
#define SPI_DDR     DDRB
#define SPI_PORT    PORTB
#define SPI_SS      PB2
#define SPI_MOSI    PB3
#define SPI_MISO    PB4
#define SPI_SCK     PB5

// SPI clock divider settings
#define SPI_CLOCK_DIV2   0x00
#define SPI_CLOCK_DIV4   0x01
#define SPI_CLOCK_DIV8   0x02
#define SPI_CLOCK_DIV16  0x03
#define SPI_CLOCK_DIV32  0x04
#define SPI_CLOCK_DIV64  0x05
#define SPI_CLOCK_DIV128 0x06

// SPI data mode 
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

// SPI Bit Order
#define MSBFIRST 0
#define LSBFIRST 1

class SPIClass {
public :
    // Function prototypes
    void begin(void);
    uint8_t transfer(uint8_t data);
    uint8_t Receive(void);
    void setClockDivider(uint8_t divider);
    void setDataMode(uint8_t mode);
    void setBitOrder(uint8_t order);
};



extern SPIClass SPI;

#endif // PROTOTYPE_FOR_CLEO_H
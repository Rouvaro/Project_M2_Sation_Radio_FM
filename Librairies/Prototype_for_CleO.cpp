#include "Prototype_for_CleO.h"

SPIClass SPI;

void SPIClass::begin(void)
{
    // Set MOSI, SCK, and SS as output pins
    SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
    
    // Set MISO as input pin
    SPI_DDR &= ~(1 << SPI_MISO);
    
    // Enable SPI, Master mode, and set clock divider to 4
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    
    // Clear SPI interrupt flag and enable interrupts
    SPSR |= (1 << SPIF);
    sei();
}

uint8_t SPIClass::transfer(uint8_t data)
{
    // Enable slave select pin
    SPI_PORT &= ~(1 << SPI_SS);
    
    // Load data into the SPI data register
    SPDR = data;
    
    // Wait for transmission to complete
    while(!(SPSR & (1 << SPIF)));
    
    // Disable slave select pin
    SPI_PORT |= (1 << SPI_SS);

    return SPDR;
}

uint8_t SPIClass::Receive(void)
{
    // Enable slave select pin
    SPI_PORT &= ~(1 << SPI_SS);
    
    // Send dummy byte to initiate SPI transfer
    SPDR = 0x00;
    
    // Wait for reception to complete
    while(!(SPSR & (1 << SPIF)));
    
    // Disable slave select pin
    SPI_PORT |= (1 << SPI_SS);
    
    // Return received data
    return SPDR;
}

void SPIClass::setClockDivider(uint8_t divider)
{
    // Clear the SPI2X bit in SPSR to ensure normal SPI clock rate
    SPSR &= ~(1 << SPI2X);
    
    // Set the clock divider in SPCR
    switch(divider)
    {
        case SPI_CLOCK_DIV2:
            SPCR = (SPCR & ~((1 << SPR0) | (1 << SPR1))) | (0 << SPR0) | (0 << SPR1);
            break;
        case SPI_CLOCK_DIV4:
            SPCR = (SPCR & ~((1 << SPR0) | (1 << SPR1))) | (0 << SPR0) | (1 << SPR1);
            break;
        case SPI_CLOCK_DIV8:
            SPCR = (SPCR & ~((1 << SPR0) | (1 << SPR1))) | (1 << SPR0) | (0 << SPR1);
            break;
        case SPI_CLOCK_DIV16:
            SPCR = (SPCR & ~((1 << SPR0) | (1 << SPR1))) | (1 << SPR0) | (1 << SPR1);
            break;
        case SPI_CLOCK_DIV32:
            SPCR |= (1 << SPI2X);
            SPCR = (SPCR & ~((1 << SPR0) | (1 << SPR1))) | (0 << SPR0) | (0 << SPR1);
            break;
        case SPI_CLOCK_DIV64:
            SPCR |= (1 << SPI2X);
            SPCR = (SPCR & ~((1 << SPR0) | (1 << SPR1))) | (0 << SPR0) | (1 << SPR1);
            break;
        case SPI_CLOCK_DIV128:
            SPCR |= (1 << SPI2X);
            SPCR = (SPCR & ~((1 << SPR0) | (1 << SPR1))) | (1 << SPR0) | (0 << SPR1);
            break;
        default:
            // Invalid clock divider value, do nothing
            break;
    }
}

void SPIClass::setDataMode(uint8_t mode)
{
    // Clear the CPOL and CPHA bits in SPCR
    SPCR &= ~((1 << CPOL) | (1 << CPHA));
    
    // Set the CPOL and CPHA bits based on the desired mode
    switch(mode)
    {
        case SPI_MODE0:
            // CPOL = 0, CPHA = 0
            SPCR &= ~((1 << CPOL) | (1 << CPHA));
            break;
        case SPI_MODE1:
            // CPOL = 0, CPHA = 1
            SPCR &= ~(1 << CPOL);
            SPCR |= (1 << CPHA);
            break;
        case SPI_MODE2:
            // CPOL = 1, CPHA = 0
            SPCR |= (1 << CPOL);
            SPCR &= ~(1 << CPHA);
            break;
        case SPI_MODE3:
            // CPOL = 1, CPHA = 1
            SPCR |= (1 << CPOL) | (1 << CPHA);
            break;
        default:
            // Invalid mode value, do nothing
            break;
    }
}

void SPIClass::setBitOrder(uint8_t order)
{
    if(order == LSBFIRST)
    {
        // Set the LSB first bit in SPCR
        SPCR |= (1 << DORD);
    }
    else
    {
        // Clear the LSB first bit in SPCR
        SPCR &= ~(1 << DORD);
    }
}
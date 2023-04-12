/**
 * @file SSD1306.hpp
 * @author Clément MOQUEL / Guillaume SANCHEZ / Paul FILLIETTE
 * @brief 
 * @version 0.1
 * @date 2023-04-12
 * @details Source file to implement SSD1306 screen'class
 * @copyright Copyright (c) 2023
 * 
 */

/* ---------- INCLUDES ---------- */
#include "SSD1306.hpp"

/* ---------- METHODES ---------- */
SSD1306::SSD1306()
{
    this->display_volume = 0;
    this->frequency_display = 0;
}

void SSD1306::ssd1306_init()
{
    i2c_start();
    start_ack;
    i2c_write(SSD1306_ADDR << 1);
    sla_ack;    
    i2c_write(SSD1306_COMMAND);
    data_ack;
    i2c_write(SSD1306_DISPLAY_OFF); 
    data_ack;
    i2c_write(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO); 
    data_ack;RadioSensor.sensor_frequency
    i2c_write(SSD1306_SUGG_RATIO); 
    data_ack;
    i2c_write(SSD1306_SET_MULTIPLEX_RATIO);
    data_ack;
    i2c_write(0x3F);    // height-1 = 63 = 0x3F and 31 = 0x3D for half size displays
    data_ack;
    i2c_write(SSD1306_SET_DISPLAY_OFFSET);
    data_ack;
    i2c_write(0x00);    // Offset OFF
    data_ack;
    i2c_write(SSD1306_DATA_CONTINUE);
    data_ack;
    i2c_write(SSD1306_CHARGE_PUMP); 
    data_ack;
    i2c_write(0x14);    // Enable charge pump
    data_ack;
    i2c_write(SSD1306_MEMORY_ADDR_MODE);
    data_ack;
    i2c_write(0x00);    // horizontal addressing mode
    data_ack;
    i2c_write(SSD1306_SET_SEGMENT_REMAP | 0x1);
    data_ack;
    i2c_write(SSD1306_COM_SCAN_DIR_DEC);
    data_ack;
    i2c_write(SSD1306_SET_COM_PINS);
    data_ack;
    i2c_write(0x02);    // alt com pins, disable left/right remap
    data_ack;
    i2c_write(SSD1306_SET_CONTRAST_CONTROL); 
    data_ack;
    i2c_write(SSD1306_CONTRAST_RECOMMANDED_VALUE); 
    data_ack;
    i2c_write(SSD1306_SET_PRECHARGE_PERIOD); 
    data_ack;
    i2c_write(SSD1306_PRECHARGE_PERIOD_RECOMMANDED_VALUE);
    data_ack;
    i2c_write(SSD1306_SET_VCOM_DESELECT); 
    data_ack;
    i2c_write(0x40);    // Default value
    data_ack;
    i2c_write(SSD1306_DISPLAY_ON);  // display on
    data_ack;
    i2c_stop();
}

void SSD1306::clear(void)
{
    for (int i = 0; i < 8; i++)
    {
        this->sendCommand(0xB0 + i);  // Set page address
        this->sendCommand(0x00);      // Set column address (lower 4 bits)
        this->sendCommand(0x10);      // Set column address (upper 4 bits)
    for (int j = 0; j < 128; j++)
    {
        this->sendData(0x00);         // Set pixel OFF
    }
    }
}

void SSD1306::fill(void)
{
    for (int i = 0; i < 8; i++)
    {
        this->sendCommand(0xB0 + i);  // Set page address
        this->sendCommand(0x00);      // Set column address (lower 4 bits)
        this->sendCommand(0x10);      // Set column address (upper 4 bits)
    for (int j = 0; j < 128; j++)
    {
        this->sendData(0xFF);         // Set pixel ON
    }
    }
}

void SSD1306::update(void)
{
    for (int i = 0; i < 8; i++)
    {
        this->sendCommand(0xB0 + i); // Set page address
        this->sendCommand(0x00);     // Set column address (lower 4 bits)
        this->sendCommand(0x10);     // Set column address (upper 4 bits)
    for (int j = 0; j < 128; j++)
    {
        this->sendData(this->buffer[i * 128 + j]);
    }
    }
}

void SSD1306::pixel_set(int x, int y, bool state)
{
    unsigned char tab[64] = {0, 32, 1, 33, 2, 34, 3, 35,
                             4, 36, 5, 37, 6, 38, 7, 39,
                             8, 40, 9, 41,10, 42,11, 43,
                             12,44,13, 45,14, 46,15, 47,
                             16,48,17, 49,18, 50,19, 51,
                             20,52,21, 53,22, 54,23, 55,
                             24,56,25, 57,26, 58,27, 59,
                             28,60,29,61,30,62,31,63  };
    y = tab[y];
    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) // Check if coordonates are in the display
    {
        return;
    }
    if (state)
    {
        this->buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y & 7)); // Set bit to 1 in buffer
    }
    else
    {
        this->buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y & 7)); // Set bit to 0 in buffer
    } 
}

void SSD1306::volume_meter(uint16_t vol)
{
    if( vol > this->display_volume)
    {
        for(int i = 64 ; i > (64 - (vol * 4)) ; i--)
        {
            for( int j = 0 ; j < (16 - floor(i / 4)) ; j++)
                this->pixel_set(j, i, true);
        }
    }
    else
    {
        for(int i = 0 ; i < (64 - (vol * 4)) ; i++)
        {
            for(int j = 0 ; j < (16 - floor(i / 4)) ; j++)
                this->pixel_set(j, i, false);
        }
    }
    this->display_volume = vol;
}

void SSD1306::digit_display(unsigned char digit, int x, int y)
{
    const unsigned char digits[10][8] = {
        {0x3E, 0x7F, 0x71, 0x79, 0x7F, 0x3E}, // 0
        {0x00, 0x21, 0x7F, 0x7F, 0x01, 0x00}, // 1
        {0x21, 0x63, 0x55, 0x49, 0x7F, 0x36}, // 2
        {0x22, 0x63, 0x49, 0x49, 0x7F, 0x36}, // 3
        {0x0F, 0x0C, 0x08, 0x08, 0x7F, 0x7F}, // 4
        {0x2F, 0x6F, 0x49, 0x49, 0x79, 0x31}, // 5
        {0x3E, 0x7F, 0x49, 0x49, 0x7B, 0x32}, // 6
        {0x03, 0x03, 0x71, 0x79, 0x0F, 0x07}, // 7
        {0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36}, // 8
        {0x26, 0x6F, 0x49, 0x49, 0x7F, 0x3E}  // 9
    };

    if (digit < 0 || digit > 9) 
    {
        return;
    }

    for (int i = 0; i < 6; i++) 
    {
        unsigned char row = digits[digit][i];
        for (int j = 0; j < 8; j++) 
        {
            bool state = (row >> j) & 1;
            this->pixel_set(x + i, y + j, state);
        }
    }
}

void SSD1306::display_frequency(int freq)
{
    if(freq < 8800 || freq > 10850)
    return;
    this->digit_display(floor(freq/10000),30,30);
    this->digit_display(floor(freq/1000)-floor(freq/10000)*10,40,30);
    this->digit_display(floor(freq/100)-floor(freq/1000)*10,50,30);
    this->pixel_set(58,36,true);
    this->digit_display(floor(freq/10)-floor(freq/100)*10,60,30);
    this->digit_display(freq-floor(freq/10)*10,70,30);   
}

void SSD1306::sendCommand(uint8_t command)
{
    i2c_start();
    i2c_send_byte(SSD1306_ADDR << 1);
    i2c_send_byte(SSD1306_COMMAND);
    i2c_send_byte(command);
    i2c_stop();
}

void SSD1306::sendData(uint8_t data)
{
    i2c_start();
    i2c_send_byte(SSD1306_ADDR << 1);
    i2c_send_byte(SSD1306_DATA_CONTINUE);
    i2c_send_byte(data);
    i2c_stop();
}

void SSD1306::starting_animation(void)
{
  for(int k = 0 ; k < 16 ; k++)
  {
    this->volume_meter(k);
    this->updatet();
  }
  for(int k = 16 ; k >= 0 ; k--)
  {
    this->volume_meter(k);
    this->update();
  }
  this->volume_meter(10);
  this->update();
}

void SSD1306::setFrequency(int frequency)
{
    this->frequency_display = frequency;
}

void SSD1306::setVolume(uint16_t volume)
{
    this->display_volume = volume;
}
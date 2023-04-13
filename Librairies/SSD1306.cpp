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

/**
 * @brief Construct a new SSD1306::SSD1306 object
 * @param none
 * 
 */
SSD1306::SSD1306()
{
    this->display_volume = 0;
    this->frequency_display = 0;
}

/**
 * @brief Initialisation of SSD1306 object
 * @param none
 * @retval none
 */
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
    data_ack;
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

/**
 * @brief Clear entire screen display
 * @param none
 * @retval none
 */
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

/**
 * @brief Set entire screen display
 * @param none
 * @retval none
 */
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

/**
 * @brief Update screen display
 * @param none
 * @retval none
 */
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

/**
 * @brief Set pixel(x, y) ON or OFF
 * @param Coordinates of the pixel to set, Integer x and y and the state (true, false)
 * @retval none
 */
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

/**
 * @brief display volume on the screen
 * @param 8 bits unsigned integer
 * @retval none
 */
void SSD1306::volume_meter(uint8_t vol)
{
    if( vol > volume)
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
    volume = vol;
}


/**
 * @brief Display the digit wanted at x and y place
 * @param unsigned char for digit to display and its coordonates
 * @retval none
 */
void SSD1306::digit_display(unsigned char digit, int x, int y)
{
    const unsigned char digits[10][8] = {
        {0x3E, 0x7F, 0x71, 0x79, 0x7F, 0x3E}, // 0
        {0x00, 0x84>>1, 0xFE>>1, 0xFE>>1, 0x80>>1, 0x00}, // 1
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

/**
 * @brief display frequency
 * @param integer frequency
 * @retval none
 */
void SSD1306::display_frequency(int freq)
{
  if(freq < 8800 || freq > 10850)
    return;
  this->digit_display(floor(freq/10000),30,30);
  this->digit_display(floor(freq/1000)-floor(freq/10000)*10,40,30);
  this->digit_display(floor(freq/100)-floor(freq/1000)*10,50,30);
  this->pixel_set(58,36,true);
  this->pixel_set(57,36,true);
  this->pixel_set(58,37,true);
  this->pixel_set(57,37,true);
  this->digit_display(floor(freq/10)-floor(freq/100)*10,60,30);
  this->digit_display(freq-floor(freq/10)*10,70,30);
  this->letter_display('M',80,30);
  this->letter_display('H',87,30);
  this->letter_display('Z',94,30);
}

/**
 * @brief Display the letter wanted
 * @param unsigned char for the letter to display and its coordonates
 * @retval none
 */
void SSD1306::letter_display(unsigned char letter, int x, int y) {
    const uint8_t FONT[][5] = {
      {0x7E, 0x11, 0x11, 0x11, 0x7E},  // A
      {0x7F, 0x49, 0x49, 0x49, 0x36},  // B
      {0x3E, 0x41, 0x41, 0x41, 0x22},  // C
      {0x7F, 0x41, 0x41, 0x22, 0x1C},  // D
      {0x7F, 0x49, 0x49, 0x49, 0x41},  // E
      {0x7F, 0x09, 0x09, 0x09, 0x01},  // F
      {0x3E, 0x41, 0x49, 0x49, 0x7A},  // G
      {0x7F, 0x08, 0x08, 0x08, 0x7F},  // H
      {0x00, 0x41, 0x7F, 0x41, 0x00},  // I
      {0x20, 0x40, 0x41, 0x3F, 0x01},  // J
      {0x7F, 0x08, 0x14, 0x22, 0x41},  // K
      {0x7F, 0x40, 0x40, 0x40, 0x40},  // L
      {0x7F, 0x02, 0x0C, 0x02, 0x7F},  // M
      {0x7F, 0x04, 0x08, 0x10, 0x7F},  // N
      {0x3E, 0x41, 0x41, 0x41, 0x3E},  // O
      {0x7F, 0x09, 0x09, 0x09, 0x06},  // P
      {0x3E, 0x41, 0x51, 0x21, 0x5E},  // Q
      {0x7F, 0x09, 0x19, 0x29, 0x46},  // R
      {0x46, 0x49, 0x49, 0x49, 0x31},  // S
      {0x01, 0x01, 0x7F, 0x01, 0x01},  // T
      {0x3F, 0x40, 0x40, 0x40, 0x3F},  // U
      {0x0F, 0x30, 0x40, 0x30, 0x0F},  // V
      {0x3F, 0x40, 0x30, 0x40, 0x3F},  // W
      {0x7F, 0x48, 0x48, 0x48, 0x30},  // X
      {0x07, 0x08, 0x78, 0x08, 0x07},  // Y
      {0x71, 0x49, 0x45, 0x43, 0x00},  // Z
      {0x00, 0x00, 0x00, 0x00, 0x00},
      {0x00, 0x00, 0x00, 0x00, 0x00},
      {0x00, 0x00, 0x00, 0x00, 0x00},
      {0x00, 0x00, 0x00, 0x00, 0x00},
      {0x00, 0x00, 0x00, 0x00, 0x00},
      {0x00, 0x00, 0x00, 0x00, 0x00},
      {0x3E, 0x45, 0x49, 0x51, 0x3E},  // a
      {0x7F, 0x49, 0x49, 0x49, 0x36},  // b
      {0x3E, 0x41, 0x41, 0x41, 0x22},  // c
      {0x3E, 0x41, 0x41, 0x22, 0x7F},  // d
      {0x3F, 0x49, 0x49, 0x49, 0x31},  // e
      {0x7F, 0x09, 0x09, 0x09, 0x01},  // f
      {0x31, 0x49, 0x49, 0x4A, 0x3C},  // g
      {0x7F, 0x08, 0x08, 0x08, 0x7F},  // h
      {0x00, 0x41, 0x7F, 0x41, 0x00},  // i
      {0x20, 0x40, 0x41, 0x3F, 0x01},  // j
      {0x7F, 0x08, 0x14, 0x22, 0x41},  // k
      {0x7F, 0x40, 0x40, 0x40, 0x40},  // l
      {0x7F, 0x02, 0x0C, 0x02, 0x7F},  // m
      {0x7F, 0x04, 0x08, 0x10, 0x7F},  // n
      {0x3E, 0x41, 0x41, 0x41, 0x3E},  // o
      {0x7F, 0x09, 0x09, 0x09, 0x06},  // p
      {0x3E, 0x41, 0x51, 0x21, 0x5E},  // q
      {0x7F, 0x09, 0x19, 0x29, 0x46},  // r
      {0x26, 0x49, 0x49, 0x49, 0x32},  // s
      {0x01, 0x01, 0x7F, 0x01, 0x01},  // t
      {0x3F, 0x40, 0x40, 0x40, 0x3F},  // u
      {0x0F, 0x30, 0x40, 0x30, 0x0F},  // v
      {0x77, 0x08, 0x08, 0x77, 0x00},  // w
      {0x7C, 0x08, 0x08, 0x7C, 0x00},  // x
      {0x7C, 0x08, 0x08, 0x7F, 0x00},  // y
      {0x63, 0x59, 0x49, 0x46, 0x00},  // z
      };
    
    if ((letter < 'A' || letter > 'Z') && (letter < 'a' || letter > 'z'))
      return;
    
    for (int i = 0; i < 5; i++) {
        unsigned char row = FONT[(letter-'A')][i];
        for (int j = 0; j < 25; j++) {
            bool state = (row >> j) & 1;
            this->pixel_set(x + i, y + j, state);
        }
    }
}

/**
 * @brief Send command to sensor
 * @param 8 bits unsigned integer command 
 * @retval none
 */
void SSD1306::sendCommand(uint8_t command)
{
    i2c_start();
    start_ack;
    i2c_send_byte(SSD1306_ADDR << 1);
    sla_ack;
    i2c_send_byte(SSD1306_COMMAND);
    data_ack;
    i2c_send_byte(command);
    data_ack;
    i2c_stop();
}

/**
 * @brief SSend data to sensor
 * @param 8 bits unsigned integer data 
 * @retval none
 */
void SSD1306::sendData(uint8_t data)
{
    i2c_start();
    start_ack;
    i2c_send_byte(SSD1306_ADDR << 1);
    sla_ack;
    i2c_send_byte(SSD1306_DATA_CONTINUE);
    data_ack;
    i2c_send_byte(data);
    data_ack;
    i2c_stop();
}

/**
 * @brief Display an animation to test Volume display and Frequency display
 * @param none
 * @retval none
 */
void SSD1306::starting_animation(void)
{
  for(int k = 0 ; k < 10 ; k++)
  {
    this->volume_meter(k+4);  
    this->digit_display(k,30,30);
    this->digit_display(k,40,30);
    this->digit_display(k,50,30);
    this->pixel_set(58,36,true);
    this->pixel_set(57,36,true);
    this->pixel_set(58,37,true);
    this->pixel_set(57,37,true);
    this->digit_display(k,60,30);
    this->digit_display(k,70,30);
    this->letter_display('Z',94,30);  
    this->letter_display('M',80,30);
    this->letter_display('H',87,30);
    this->update();
  }
  for(int k = 10 ; k >= 0 ; k--)
  {
    this->volume_meter(k+4);  
    digit_display(k,30,30);
    this->digit_display(k,40,30);
    this->digit_display(k,50,30);
    this->pixel_set(58,36,true);
    this->pixel_set(57,36,true);
    this->pixel_set(58,37,true);
    this->pixel_set(57,37,true);
    this->digit_display(k,60,30);
    this->digit_display(k,70,30);
    this->letter_display('M',80,30);
    this->letter_display('H',87,30);
    this->letter_display('Z',94,30);  
    this->update();
  }
  this->update();
} 

/**
 * @brief Setter for frequency value / Update frequency atribute
 * @param integer frequency to set
 * @retval none
 */
void SSD1306::setFrequency(int frequency)
{
    this->frequency_display = frequency;
}

/**
 * @brief Setter for volume level value / Update volume level atribute
 * @param 8 bits unsigned integer volume to set
 * @retval none
 */
void SSD1306::setVolume(uint8_t volume)
{
    this->display_volume = volume;
}
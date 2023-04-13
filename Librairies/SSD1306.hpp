/**
 * @file SSD1306.hpp
 * @author Clément MOQUEL / Guillaume SANCHEZ / Paul FILLIETTE
 * @brief 
 * @version 0.1
 * @date 2023-04-12
 * @details Header file to define SSD1306 screen'class
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SSD1306_HPP
#define SSD1306_HPP

/* ---------- INCLUDES ---------- */
#include "atmega328p_i2c.hpp"
#include <stdlib.h>


/* ---------- DEIFNES ----------- */
#define SSD1306_ADDR 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define ANALOG_PIN A0

// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL					0x81
#define SSD1306_CONTRAST_RECOMMANDED_VALUE              0xCF
#define SSD1306_DISPLAY_ALL_ON_RESUME					0xA4
#define SSD1306_DISPLAY_ALL_ON							0xA5
#define SSD1306_NORMAL_DISPLAY							0xA6
#define SSD1306_INVERT_DISPLAY							0xA7
#define SSD1306_DISPLAY_OFF								0xAE
#define SSD1306_DISPLAY_ON								0xAF
#define SSD1306_NOP										0xE3
// Scrolling Commands
#define SSD1306_HORIZONTAL_SCROLL_RIGHT					0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT					0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT	0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT		0x2A
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3
// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN						0x00
#define SSD1306_SET_HIGHER_COLUMN						0x10
#define SSD1306_MEMORY_ADDR_MODE						0x20
#define SSD1306_SET_COLUMN_ADDR							0x21
#define SSD1306_SET_PAGE_ADDR							0x22
// Hardware Configuration Commands
#define SSD1306_SET_START_LINE							0x40
#define SSD1306_SET_SEGMENT_REMAP						0xA0
#define SSD1306_SET_MULTIPLEX_RATIO						0xA8
#define SSD1306_COM_SCAN_DIR_INC						0xC0
#define SSD1306_COM_SCAN_DIR_DEC						0xC8
#define SSD1306_SET_DISPLAY_OFFSET						0xD3
#define SSD1306_SET_COM_PINS							0xDA
#define SSD1306_CHARGE_PUMP								0x8D
// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO				0xD5
#define SSD1306_SUGG_RATIO                              0x80
#define SSD1306_SET_PRECHARGE_PERIOD					0xD9
#define SSD1306_PRECHARGE_PERIOD_RECOMMANDED_VALUE      0xF1
#define SSD1306_SET_VCOM_DESELECT						0xDB

#define SSD1306_COMMAND			0x00
#define SSD1306_DATA			0xC0
#define SSD1306_DATA_CONTINUE	0x40
uint8_t volume; 

/**
 * @brief SSD1306 for I²C OLED's screen
 * @class SSD1306
 * @param none
 * @retval none
 */
class SSD1306
{
public :
    // Variables
    uint8_t display_volume;
    int frequency_display;
    uint8_t buffer [(OLED_WIDTH * OLED_HEIGHT) / 8];
    // Constructor
    SSD1306();
    // Methodes
    void ssd1306_init();
    void clear(void);
    void fill(void);
    void update(void);
    void pixel_set(int x, int y, bool state);
    void setFrequency(int frequency);
    void setVolume(uint8_t volume);
    void starting_animation(void);
    void volume_meter(uint8_t vol);
    void display_frequency(int freq);

private:
    // Methodes
    void sendCommand(uint8_t command);
    void sendData(uint8_t data);
    void digit_display(unsigned char digit, int x, int y);
    void letter_display(unsigned char letter, int x, int y);
};


#endif // SSD1306_HPP
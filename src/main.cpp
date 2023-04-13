/**
 * @file main.cpp
 * @author PAUL FILLIETTE / CLEMENT MOQUEL / GUILLAUME SANCHEZ
 * @brief 
 * @version 0.1
 * @date 2023-04-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* INCLUDES */
#include "SeeedGroveFMReceiver.cpp"
#include "SSD1306.cpp"

int main()
{
    SeeedGroveFMReceiver myFM;
    SSD1306 ssd1306Screen;

    i2c_init();
    ADC_init();

    uint8_t volume_read = 10;
    int frequency_read = RADIO_STATION_NRJ;

    ssd1306Screen.ssd1306_init();
    ssd1306Screen.clear();
    ssd1306Screen.starting_animation();

    // myFM.initSensor();
    // myFM.setVolumeLevel(volume_read);
    // myFM.setFrequency(frequency_read);

    while(1)
    {

        if( (adc_to_freq(read_adc(1)) >= frequency_read + 10) || (adc_to_freq(read_adc(1)) <= frequency_read - 20))
        {
            frequency_read = adc_to_freq(read_adc(1));
            // myFM.setFrequencyUpdate(frequency_read);
            // myFM.setFrequency(frequency_read);
            ssd1306Screen.setFrequency(frequency_read);
            ssd1306Screen.display_frequency(frequency_read);
            ssd1306Screen.update();

        }

        if( (adc_to_freq(read_adc(0)) >= volume_read + 1) || (adc_to_freq(read_adc(0)) <= volume_read - 1))
        {
            volume_read = adc_to_vol(read_adc(0));
            // myFM.setFrequencyUpdate(volume_read);
            // myFM.setVolumeLevel(volume_read);
            ssd1306Screen.setVolume(volume_read);
            ssd1306Screen.volume_meter(volume_read);
            ssd1306Screen.update();
        }
    }  
    return 0;
}
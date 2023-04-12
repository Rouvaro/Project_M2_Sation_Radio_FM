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
    i2c_init();
    SeeedGroveFMReceiver myFM;
    myFM.initSensor();
    myFM.setVolumeLevel(vol);
    myFM.setFMSeekDown();

    ADC_init();
    ssd1306_init();
    clear();
    starting_animation();

    while(1)
    {
        //Volume selection and display 
        myFM.setVolumeLevel(adc_to_vol(read_adc(0)));
        //volume_meter(adc_to_vol(read_adc(0)));
        volume_meter(myFM.getVolumeLevel());
 
        //Frequency selection and display 
        myFM.setFrequency(adc_to_freq(read_adc(1)));
        display_frequency(myFM.getFrequency());
        //display_frequency(adc_to_freq(read_adc(1)));

        update();
    }  
    return 0;
}
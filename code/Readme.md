In this section, the code. It was programmed using PIC C Compiler by CCS (Computer Custom Services) [link here](https://www.ccsinfo.com/compilers.php). 

I used libraries for drive:
1. i2c link with DS1307 RTC
![imagen](https://github.com/davemaster/School_Timer_2024/assets/1075807/a8283563-d08e-4143-9d7e-3c07bc8a2900)

2. 16X2 LCD with Backlight, in 4-bit operation mode
![imagen](https://github.com/davemaster/School_Timer_2024/assets/1075807/c0877c70-3854-4328-861b-b29901a5d933)

3. For relay, just:

    #define relay_timbre    PIN_A0
    
    Then;
    
    digitalWrite(relay_timbre, HIGHT);     // activate NC in the relay, and ring the BELL
    
    and digitalWrite(relay_timbre, LOW);     // deactivate NC in the relay, and stop ringing the BELL

4.   The Same for the backlight, and the POWER_LED
5.   A little more hard to get was the "blinking" in the data to be updated in Events, and Time Configuration.
6.   For Security, NO BLUETOOTH interfaces was implemented...

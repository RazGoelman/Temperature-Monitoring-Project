# Temperature-Monitoring-Project
# (VERSION 1.0)


**Project Goal:**

   Application of temperature monitoring sensor, the project is implemented by FreeRTOS using C++ 

**Interface for Project:**

   FreeRTOS using C++

**Hardware:**
 
 **link:** https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf

    STM32 Nucleo-64, L476RG board
    
    Core: ARM®32-bit Cortex®-M4 CPU

    Operating Supply Voltage: 3.3V,5V,7V to 12V
    
    VDD: 1.71V to 3.6V

**Components:**

    • Temperature Sensor – DHT11
    
    • Analog buzzer 
    
    • Data Logger shield
    
    • Red Led
    
    • Blue Led
    
    • Button Switch
    
    • SD Card
    
    • Rtc 
    
**Communication:**
    
    • UART Protocol 
    
    • SPI Protocol


**Use cases:**

**Temperature sensor:**

**link:** https://www.coursehero.com/file/52036667/DHT11-Technical-Data-Sheet-Translated-Version-1143054pdf/ 

    • Temperature sensor should constantly check (1s) the current temperature
    
    • Temperature sensor should save measured temperature every 1 minute in file
    
    • It should have two thresholds:
    
    • warning
    
    • critical temperature
    
    • below the threshold temperature
    
**1.1 When the temperature increases above the warning threshold it should:**

    1.1.1 switch on the red LED

    1.1.2 write the event into the events log file

**2.1 When the temperature increases above the critical threshold it should:** 

    2.1.1 Blink on the red LED 

    2.1.2 An alert by buzzer sound 

    2.1.3 write the event into the events log file

**3.1 When the temperature decreases the threshold, it should disable corresponding notifications:**

    3.1.1 Red LED

    3.1.2 Buzzer sound

    3.1.3 Events log file
 
**Analog Buzzer:**

    The user could disable the buzzer signal by pressing the switch button

**Data Logger Shield (Logger File):**

**link**: https://learn.adafruit.com/adafruit-data-logger-shield

    The log record should have the following data: temperature

    1. current date and time.

    2. log severity.

    3. message (temperature)

**Red Led:**

    will alert for temperature thresholds

**Blue Led:**

    1. will Switch when the system is ‘on’.

    2. when the temperature decreases the threshold

**Button Switch:**

    will give the user an option to disable the buzzer signal

**SD Card:**

    Files for event and temperature log shall be stored on SD Card

**FreeRTOS Tasks:**

**Temperature Task:**

     The temperature task will check every 1 secound the sensor output and will active according to the returen temperature.
      
     The sensor defined in 4 levels:
         
       * Critical 
         
       * Warning 
         
       * Normal 
         
       * Error
   
     The temperature task will chack evry 1 ms the sensor output and will active according to the returen temperature

   ![image](https://user-images.githubusercontent.com/66781442/203984336-d14be378-8a2d-4d25-9ec4-ae72628b2d2a.png)

**Communication Task:**

    The communication task will implement the Command line for a user to:

    led on/off/blink

    Rtc set / get time 
    
    Buzzer play on / play off

**Flash Task:**

    The flash task will save all the temperature thresholds (alerts) on file.


**Block diagram :**

   ![image](https://user-images.githubusercontent.com/66781442/203984996-b71b63df-40f6-4a01-8063-c9678e22ba80.png)

**System polymorphism:**

**FreeRTOS is connected to:**

     Temperature sensor - GPIO output
       
     Communication task - UART protocol
       
     Flash Task - SPI protocol
       
     Led Task  - GPIO output

**Led**  

    called by communication task and temperature tasks.

**Analog buzzer** 
    
    called by communication task and temperature task, active by real time clock (timer 3).

**Button**  
    
    called by analog buzzer

**SD Card** 
    
    called by flash task , temperature task and communication task.





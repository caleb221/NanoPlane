# NanoPlane
A fixed wing drone using only an Arduino nano!

This is a custom implementation of an arduino nano as a flight controller along with a 
custom built transmitter.

I wanted to built a fixed wing drone using some spare parts I had laying around 
and this is the result!
After a lot of testing and soldering, this is the result.

The hardware used is as follows:
 
 On the Drone (receiver)
 ----------------------------
    Arduino nano
    LoRa 433Mhz RA-02 transceiver
    Emax RS2205 2600Kv brushless motor
    UFO 30Amp Dshot1200 ESC
    MG90S 9 gram Servo motor (X2)
    LM2596S DC-DC buck converter
    AMS1117 3.3V voltage converter
    2200mah 3S 11.1V battery
 ----------------------------

 On the transmitter (remote control)
 ----------------------------
    Arduino nano
    LoRa RA-02 433Mhx transceiver
    Nokia 5110 LCD
    PS2 Joystick (X2)
    Slide potentiometer
    knob potentiometer
    LM2596S DC-DC buck converter
    AMS1117 3.3V voltage converter
 ----------------------------
  
  ToDo --> Add a camera for FPV capabilities
           (maybe using an ESP32Cam I have laying around)

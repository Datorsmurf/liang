# LiaNG

This is a software for the next generation of Liam, called LiaNG. 

Take care, this software is still in an early stage and is incomplete, buggy and... well... pre-alpha.

Fork it at you own risk, major refactoring is likely :)

The code is prepared for development in VSCode/PlatformIO, but will work in other IDE altough when you get the library depencies sorted out.

## Getting started:
1. Compile and upload the program

2. Upload the files in /data to the SPFISS area.

3. Connect to the temporary wifi accesspoint provided by LiaNG.

4. Surf to http://10.0.2.1/settings.html and enter your Wifi connection details.

5. After successful connection to the wifi LiaNG will report its IP in the serial and on the display if you have one connected.

## Modes
Liang is using modes to select the overall goal for the mower at any given time. A move implies the use of different behaviors to achieve that goal. The modes are *Idle*, *Charge*, *Mow* and *Mow Once*
### Idle
The mower is passive and only a few debug and testing behaviors are available
### Charge
The goal is to be charging and stay in charging until told otherwise.
### Mow
The mower aims to mow and charge in a ever repeating cycle. Mowing starts when the battery is full.
### Mow once
The mower mows until the battery is low and then the mode changes to *Charge*. **Mowing starts immediately when *Mow Once* is activated**


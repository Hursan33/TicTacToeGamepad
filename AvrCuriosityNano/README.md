# Avr Curiosity Nano version
This version of the software is written in C, compiled using avrgcc and flashed with avrdude. I wrote the driver for
the LEDs, which are sensitive to timing, so I decided to use `nop` instructions. They are used to create delay, which is more accurate than using delay functions from
`<util/delay.h>`. For the number of cycles, I used the LED datasheet as a reference. 
## Status
This version of the project was an assignment on a school cours and will *NOT* be updated anymore.
## Connection
In this version the MCU was connected via the 9 pins present on the PCB, 
but it can be made completely stand-alone.
## Images
![20250605_115015](https://github.com/user-attachments/assets/8691e97e-f627-422b-bc60-eb5335d71462)
![20250605_115028](https://github.com/user-attachments/assets/8f7977af-2a34-4b5b-aec4-e84e1c0bb2ae)

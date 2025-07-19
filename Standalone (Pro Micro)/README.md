# Standalone arduino pro micro version
This is a reworked version of the TicTacToe gamepad. It is has a battery and no need to be plugged into a computer.
## 1 Software
Sadly, I was unable to get the timing right for the LEDs. In the end, I decided to use Arduino code (instead of bare metal) and used the Neopixel library to control the LEDs.
## 2 Components
This variant of the projects uses additional components. 
* 3 x sliding switches like [this](https://www.aliexpress.com/item/1005005877582064.html?spm=a2g0o.order_list.order_list_main.116.5a0a1802KlUbMT) one
* 1 x charging circuit like [this](https://www.aliexpress.com/item/1005006734963929.html?spm=a2g0o.order_list.order_list_main.22.2a7718028qB14W)
* 1 x small lithium battery, I used a 180mAh battery because it was small and I had it already on hand, but a bigger battery wouldn't hurt.
## 3 Images and building
![a3](https://github.com/user-attachments/assets/b37088c3-84f8-4158-8cb2-939ac521687a)
![a2](https://github.com/user-attachments/assets/7139ae17-d90e-4051-b567-a9aadfca8007)
![a1](https://github.com/user-attachments/assets/c4f7b578-3d62-43e7-a51d-f30d4ac8b69c)
2 of the 3 switches are connected to pins A0 and A2, the position of these switches at startup controls whether the Bot or 3-mark rule functionalities are turned on, thus there is no need to edit the code. The last switch is the power switch and completes the circuit between the MCU and the charging circuit. I repurposed an old micro usb connecter between the MCU and charging circuit but you can hardwire it.
The pin connections can be found in the code [here](https://github.com/Hursan33/TicTacToeGamepad/blob/main/Standalone%20(Pro%20Micro)/firmware.ino).


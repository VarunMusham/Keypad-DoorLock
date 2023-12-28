# Keypad Based Door Lock Mechansim Using STM32
## Components Used
1. STM32F303RE
2. 20x4 LCD (no I2C)
3. 4x4 Keypad
4. Bolt IoT Wifi Module
5. Buzzer

## Introduction
This is a project for the Course Embedded Systems Hardware. This project aims on building a basic Keypad based Doorlock Mechanism using a STM32 development Board.

## How to Use it
1. Clone the repository to your local machine.
   ```bash
   git clone https://github.com/VarunMusham/Keypad-DoorLock.git
2. We used STM32F303RE development board in this project. If you are using a different model, then you are required to do some changes.
   * Go to Inc sub-folder in Core folder. You will find **LCD2004.h** and **Keypad4X4.h** files.
   * When you open these files you will find **#include "stm32f3xx_hal.h"**. This header file is for a STM32 board of F3 series. Incase if you are using a different model, then change this accordingly.
   * Do the same for **LCD2004.c** and **Keypad4X4.c** files in Src folder
3. Connect the Keypad and LCD. Make sure you note down the connected pin numbers correctly.
4. Make sure to connect the LCD in 4-bit Mode.
5. Go to **LCD2004.c** file and make changes for the pins as per your circuit/connections.
6. Configure the pins of LCD as *GPIO_OUTPUT*.
7. Configure the Rows of Keypad as *GPIO_INPUT* in *PULL_UP* mode and Columns as *GPIO_OUTPUT* and make sure you label the pins of Keypad as *KR0,KR1,....,KC2,KC3*.
8. Set the clock frequency to max.
9. Select timer1 and configure such that it can generate 1us delay. Our Configuration was:
   * Clock Frequency = 72 MHz
   * Pre-Scaler = 71
   * ARR = 0xffff - 1
10. Set up UART with baud rate 9600 bauds/sec
11.  You can find required documentations in the **Documentations** folder.

## Team Members
1. Varun Musham
2. Karthik Raj
3. Pavan Karthikeya
4. Revanth Kavuri
5. Bhavana K

## Email
In case of any suggestions or doubts, feel free to email me at <a href="mailto:mushamvarun130@gmail.com">mushamvarun130@gmail.com</a>.

## Useful Links
1. <a href="https://www.youtube.com/watch?v=sqf74jHEKn4">Configuring a 4X4 Keypad</a>
2. <a href="https://www.youtube.com/watch?v=ITTBWSQTi3c">Configuring a 16x2 LCD without I2C</a>

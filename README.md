Adaption of STM32CUBE's example FLASH_DualBoot for the B-L475-IOT "IoT Node" board.

Skeleton code generated for System Workbench 4 using STM32CubeMX with both project files included.

Instructions:
1. Clone repo
2. Define bank 2 in SW4 project properties > C/C++ General > Paths and Symbols > Symbols tab > Assembly > define "FLASH_BANK2"
3. Build project
4. Full chip erase using STLink utility
5. Program bin file to address 0x08080000 (2nd bank)
6. Define bank 1 in project ("FLASH_BANK1" as in step 2)
7. Build project
8. Program bin file to address 0x08000000 (1st bank)
9. Connect Serial port @ 115k and toggle blue user button to run demo

Blue user button toggles which bank of flash the device boots. When running firmware from bank 1, LED 2 flashes fast and UART1 serial outputs "BANK 1". When running from bank 2, serial port outputs "BANK 2" and LEDs 3 and 4 flash rapidly. If an error occurs, all LEDs flash with 1 second interval.
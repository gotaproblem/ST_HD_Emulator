# ATARI ACSI Hard Drive Controller Emulator
Use your ageing ATARI ST again!

KISS principal implemented throughout!

This has been done before. I don't pretend to have invented this. However, this design uses the Raspberry PI PICO, which, at time of designing, had not been done before.

This is in know way the finished article. I have uploaded to git simply because there is some interest, and who knows? maybe others will conribute to make a "complete" package.

### Initial Release:
Both PICO CPU cores are used. Core 2 is solely used for decoding command packets. Core 1, prints command data and live status. 

Note the shell... you can do things whilst the emulator is doing its stuff. At the moment, there isn't much. Just date, time reporting. But partition dumps could be performed, partitioning and formatting sd cards etc.
  
The SPI clock speed is dependent upon the chosen sd-card adapter/breakout board. My initial part would not excede 15 MHz. My replacement part attains 41 MHz.

The CPU is over-clocked at 150 MHz to get optimum performance. Higher CPU speeds have been tested, up to 300 MHz, but do not offer significant transfer speed increases.
The interface works happily at the default PICO CPU clock of 125 MHz.


### Latest Improvements:
* Incorporated local spi and sdcard files and optimised for this project.
* Removed the No-OS-FatFS dependancy.
* Two SD-CARDS are now functional.
* Tested with ICD driver, HDDriver driver, AHDI driver, PPutnik driver.
* Tested with EmuTOS and FreeMint.
Typical transfer speeds are > 900 KB/s and peak just under 1100 KB/s. Dependent upon sd-card used.

![IMG_20221123_075550](https://user-images.githubusercontent.com/37358158/203498492-91059b17-d28f-46f9-8b31-77cca58a99c8.jpg)
![IMG_20221123_075648](https://user-images.githubusercontent.com/37358158/203498219-669f4c92-ed25-4539-94dd-ad823408b29a.jpg)
![IMG_20221123_075717](https://user-images.githubusercontent.com/37358158/203498433-800c3950-8eab-4f6e-836a-1a9342bfb8cb.jpg)




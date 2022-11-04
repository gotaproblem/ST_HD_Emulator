# ATARI ACSI Hard Drive Controller Emulator
Use your ageing ATARI ST again!

KISS principal implemented throughout!

This has been done before. I don't pretend to have invented this. However, this design uses the Raspberry PI PICO, which, at time of designing, had not been done before.

This is in know way the finished article. I have uploaded to git simply because there is some interest, and who knows? maybe others will conribute to make a "complete" package.

### Initial Release:
This project uses both CPU cores. Core 2 is solely used for decoding command packets. Core 1, prints command data and returned status live. 

Note the shell... you can do things whilst the emulator is doing its stuff.

At the moment, there isn't much. Just date, time reporting. But partition dumps could be performed, partitioning and formatting sd cards as examples.
  
The SPI interface doesn't work above 15MHz. Probably due to the sd card adaptors I'm using.

The CPU is clocked at 125 MHz. This seems to be reliable. Any lower and you'll have problems. I have overclocked the PICO up to 300 MHz and got higher transfer rates.

Minor changes should be made to the fat FS config files ff.h anf ff_conf.h



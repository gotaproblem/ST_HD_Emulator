#include <string.h>
#include "emu.h"
#include "emuscsi.h"


void modeSense0 ( char *buf, DRIVES *pdrv, int lun )
{
    uint32_t blocks = pdrv->luns [lun].sectorCount;


    memset ( buf, 0, 16 );

    // Values got from the Hatari emulator
    buf [1] = 14;
    buf [3] = 8;

    // Send the number of blocks of the SD card
    buf [5] = (blocks >> 16) & 0xff;
    buf [6] = (blocks >> 8)  & 0xff;
    buf [7] = (blocks)       & 0xff;

    // Sector size middle byte
    buf [10] = 2;
}


void modeSense4 ( char *buf, DRIVES *pdrv, int lun )
{
    uint32_t blocks = pdrv->luns [lun].sectorCount;


    memset ( buf, 0, 16 );

    // Values got from the Hatari emulator
    buf [0] = 4;
    buf [1] = 22;

    // Send the number of blocks in CHS format
    buf [2] = (blocks >> 23) & 0xff;
    buf [3] = (blocks >> 15) & 0xff;
    buf [4] = (blocks >> 7)  & 0xff;

    // Hardcode 128 heads
    buf [5] = 128;
}
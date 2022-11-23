/*
 * ATARI ST HDC Emulator
 *
 * File:    emumount.c
 * Author:  Steve Bradford
 * Created: November 2022
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include "../sdcard/sd_card.h"
//#include "../fatfs/diskio.h"

/* project specific includes */
#include "../emuscsi.h"
#include "../emu.h"
#include "emushell.h"


extern DRIVES drv [];


void emumount ( void )
{
    //if ( ! mountFS () )
    //{
    //    printf ( "\nNo images found\nWill now try a raw mount\n\n" );

        if ( ! mountRAW () )
        {
            printf ( "\nERROR: no drives mounted\n\n" );
        }
    //}
}


void emuunmount ( int disk )
{
    if ( drv [disk].mounted )
    {
        sd_init_card ( drv [disk].pSD );        /* need to call this to reset status bits */
        
        drv [disk].pSD->sectors = 0;
        drv [disk].packetCount  = 0;
        drv [disk].mounted      = false;        /* mark as unmounted */
                                                /* card has been unmounted but is still inserted, so eject it */
        if ( gpio_get (drv [disk].pSD->card_detect_gpio) == false )
            drv [disk].ejected = true;

        printf ( "\n%s unmounted\n", drv [disk].pSD->pcName );
    }
}
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
#include "sd_card.h"
#include "diskio.h"

/* project specific includes */
#include "ff.h"
#include "../emu.h"
#include "emushell.h"


extern DRIVES drv[];


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

        drv [disk].mounted = false;             /* mark as unmounted */

        printf ( "\n%s unmounted\n", drv [disk].pSD->pcName );
    }
}
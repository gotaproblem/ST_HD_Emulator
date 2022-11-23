/* 
 * ATARI ST HDC Emulator
 *
 * File:    emucheckcards.c
 * Author:  Steve Bradford
 * Created: November 2022
 * 
 * 
 */

#include "emu.h"

#define INSERTED        false
#define NOT_INSERTED    true


extern DRIVES drv[];


extern void emumount ( void );
extern void emuunmount ( int );


void checkSDcards ( void )
{
    for ( int d = 0; d < MAX_DRIVES; d++ )
    {
        /* check to see if card has been removed then re-inserted */
        if ( drv [d].prevState != gpio_get (drv [d].pSD->card_detect_gpio) )
        {
            drv [d].prevState = gpio_get (drv [d].pSD->card_detect_gpio);

            if ( drv [d].ejected )
                drv [d].ejected = false;        /* reset ejected flag */
        }

        if ( gpio_get (drv [d].pSD->card_detect_gpio) == INSERTED )
        {
            if ( ! drv [d].mounted && ! drv [d].ejected )
                emumount ();
        }

        if ( gpio_get (drv [d].pSD->card_detect_gpio) == NOT_INSERTED )
        {
            if ( drv [d].mounted )
                emuunmount (d);
        }
    }
}
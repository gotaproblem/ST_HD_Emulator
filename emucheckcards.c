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


extern DRIVES drv[];


extern void emumount ( void );
extern void emuunmount ( int );

void checkSDcards ( void )
{
    for ( int d = 0; d < MAX_DRIVES; d++ )
    {
        if ( ! drv [d].mounted )
        {
            if ( gpio_get (drv [d].pSD->card_detect_gpio) == false ) /* inserted */
            {
                emumount ();
            }
        }

        else 
        {
            if ( gpio_get (drv [d].pSD->card_detect_gpio) == true ) /* not inserted */
            {
                emuunmount (d);
            }
        }
    }
}
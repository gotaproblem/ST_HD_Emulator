/* 
 * ATARI ST HDC Emulator
 *
 * File:    cpdisk.c
 * Author:  Steve Bradford
 * Created: November 2022
 * 
 * Shell command
 */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

/* project specific includes */
#include "../emu.h"
#include "emushell.h"

extern volatile bool VERBOSE;

/* Arg can be ON OFF, TRUE FLASE or 0 1 */
void debugVerbose ( char *arg )
{
    char tmp [10];


    for ( int i = 0; i < strlen (arg); i++ )
    {
        tmp [i] = toupper (arg [i] );
    }

    if ( strncmp      ( tmp, "YES", 3 ) == 0 )
    {
        VERBOSE = true;
    }
    
    else if ( strncmp ( tmp, "NO", 2 ) == 0 )
    {
        VERBOSE = false;
    }

    if ( strncmp      ( tmp, "ON", 2 ) == 0 )
    {
        VERBOSE = true;
    }
    
    else if ( strncmp ( tmp, "OFF", 3 ) == 0 )
    {
        VERBOSE = false;
    }

    else if ( strncmp ( tmp, "TRUE", 4 ) == 0 )
    {
        VERBOSE = true;
    }

    else if ( strncmp ( tmp, "FALSE", 5 ) == 0 )
    {
        VERBOSE = false;
    }

    else
    {
        if ( *arg == '1' )
        {
            VERBOSE = true;
        }

        else if ( *arg == '0' )
        {
            VERBOSE = false;
        }

        else
        {
            printf ( "%s: Invalid argument - [YES, NO, ON, OFF, TRUE, FALSE, 0, 1]\n", __func__ );
        }
    }
}
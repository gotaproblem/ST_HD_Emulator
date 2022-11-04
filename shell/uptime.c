/*
 * ATARI ST HDC Emulator
 *
 * File:    uptime.c
 * Author:  Steve Bradford
 * Created: November 2022
 *
 * Shell commands
 * syntax:  uptime
 *
 * returns true on success
 */

#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "emushell.h"

bool uptime ( void )
{
    uint64_t timestamp;
    uint32_t uptime;
    uint8_t  upsecs, upmins, uphrs;
    uint32_t updays;


    timestamp = time_us_64 () / 1000000;
    uptime = timestamp;

    if (uptime > 60)
        uptime += 30;

    updays = uptime / 86400;                    /* secs in a day */
    uptime %= 86400;
    uphrs = uptime / 3600;
    uptime %= 3600;
    upmins = uptime / 60;
    upsecs = uptime % 60;

    printf ( "We have been running for %d day%s ", updays, updays != 1 ? "s\0" : "" );
    printf ( "%02d:%02d:%02d\n", uphrs, upmins, upsecs );

    return GOOD;
}


/*
 * ATARI ST HDC Emulator Shell Comands
 *
 * cmd:
 *      uptime
 *
 * syntax:
 *      Emu> uptime
 *
 * returns true on success
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include <pico/util/datetime.h>
#include "hardware/rtc.h"
#include "emushell.h"


extern int dow ( datetime_t *dt );


static char days    [7][4] = { "Sun\0", "Mon\0", "Tue\0", "Wed\0", "Thu\0", "Fri\0", "Sat\0" };
static char months [12][4] = { "Jan\0", "Feb\0", "Mar\0", "Apr\0", "May\0", "Jun\0", "Jul\0", "Aug\0", "Sep\0", "Oct\0", "Nov\0", "Dec\0" };

bool emudate ( char *d )
{
    int         l, ret;
    char        dd[3], mm[3], yy[3];
    datetime_t  dt;
    long        day, mon, year;
    char        *daystr, *monstr;
    const char  endstr[4][2] = {"st", "nd", "rd", "th"};
    char        estr[3];

    
    ret = BAD;                                  /* assume fail */
    
    if ( *d )                                   /* have an argument, so set date */
    {
        l = strlen (d);
        
        if ( l == 4 || l == 6 )
        {
            rtc_get_datetime ( &dt );                                                /* without this, time component gets corrupted */
        
            strncpy( dd, &d[0], 2 );
          
            day = atoi (dd);

            if ( day == 0 || day > 31 )
                return ret;

            dt.day = day;
            
            strncpy( mm, &d[2], 2 );
      
            mon = atoi (mm);

            if ( mon == 0 || mon > 12 )
                return ret;

            dt.month = mon;
            
            if ( l == 6 )                                                       /* setting year as well */
            {
                strncpy( yy, &d[4], 2 );
               
                year = atoi (yy);

                if ( year < 0 || year > 99 )
                    return ret;

                dt.year = year + 2000;
            }

            rtc_set_datetime ( &dt );                                                /* set new date */
        }

        else
        {
            printf( "\tdate: Syntax error\n" );

            return ret;
        }
    }
                                                
    rtc_get_datetime ( &dt );                                                       /* read what we set */

    daystr = calloc ( 4, 1 );
    monstr = calloc ( 4, 1 );

    day = dow ( &dt );                          /* 0 - 6 (SUN - SAT) */
    dt.dotw = day;
    strcpy ( daystr, days [day] );
    
    strncpy ( monstr, months [dt.month - 1], 3 );

    switch ( dt.day )
    {
        case 1:                                 /* st */
        case 21:
        case 31:
            strncpy ( estr, endstr[0], 2 );
            break;
        case 2:                                 /* nd */
        case 22:
            strncpy ( estr, endstr[1], 2 );
            break;
        case 3:                                 /* rd */
        case 23:
            strncpy ( estr, endstr[2], 2 );
            break;
        default:                                /* th */
            strncpy ( estr, endstr[3], 2 );;
    }
    
    estr[2] = 0;                                /* terminate string */
    
    printf( "\t%s %02d%s %s %4d\n", daystr, dt.day, estr, monstr, dt.year );

    if (    ( dt.month == 12 && dt.day >= 25 )
                ||
            ( dt.month == 1 && dt.day <= 5 ) ) {
        printf( "\tMERRY CHRISTMAS" );

        if ( dt.day <= 5 && dt.month == 1 ) {
            printf( " and a HAPPY NEW YEAR\n" );
        }
        else {
            printf( "\n" );
        }
    }
    
    free( monstr );
    free( daystr );

    return GOOD;
}



bool emutime ( char *t )
{
    int         l, ret;
    char        hh[3], mm[3], ss[3];
    datetime_t  tm;
    long        hr, min, sec;

    ret = BAD;                                  /* assume fail */

    if ( *t )                                   /* have an argument, so set time */
    {
        l = strlen (t);

        if ( l == 4 || l == 6 )
        {
            rtc_get_datetime ( &tm );           /* without this, the date component gets corrupted */

            strncpy( hh, &t[0], 2 );
            hh[2] = 0;
            hr = atoi (hh);
            if ( hr < 0 || hr > 23 ) {
                printf ( "emutime() hr entry failed - hr = %d\n", hr );
                return ret;
            }
            tm.hour = hr;

            strncpy( mm, &t[2], 2 );
            mm[2] = 0;
            min = atoi (mm);
            if ( min < 0 || min > 59 ) {
                printf ( "emutime() min entry failed - min = %d\n", min );
                return ret;
            }
            tm.min = min;

            if ( l == 6 )                       /* setting seconds as well */
            {
                strncpy( ss, &t[4], 2 );
                ss[2] = 0;
                sec = atoi (ss);
                if ( sec < 0 || sec > 59 ) {
                    printf ( "emutime() sec entry failed - sec = %d\n", sec );
                    return ret;
                }
                tm.sec = sec;
            }

            rtc_set_datetime ( &tm );           /* set new time */
        }
        else
        {
            printf( "\ttime: Syntax error\n" );

            return ret;
        }
    }
                                                
    rtc_get_datetime ( &tm );                   /* read what we have set */

    printf( "\t%02d:%02d:%02d\n", tm.hour, tm.min, tm.sec );

    return GOOD;
}
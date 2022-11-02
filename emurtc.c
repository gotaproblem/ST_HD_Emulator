#include <string.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include "pico.h"
#include "hardware/rtc.h"


int dow ( datetime_t *dt )
{
    int month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d = dt->day;


    // correction for leap year
    if ( dt->year % 4 == 0 && (dt->year % 100 != 0 || dt->year % 400 == 0))
        month[1] = 29;

    for (int i = 1900; i < dt->year; i++)
        if (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0))
            d += 366;
        else
            d += 365;

    for (int i = 0; i < dt->month - 1; i++) 
        d += month[i];

    return (d % 7);
}


/* 
 * populate RTC datetime_t structure with programmes build date and time 
 */
void buildDateTime ( void )
{
    char dateStr [12];
    char timeStr [9];
    datetime_t   tm;
    char year    [5];
    char month   [4];
    char date    [3];
    char months  [12][3] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                             "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    //char days    [7][3]  = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    char hours   [3];
    char mins    [3];
    char secs    [3];
    int          i;


    sleep_ms (1);

    strncpy ( dateStr, __DATE__, 11 );
    strncpy ( timeStr, __TIME__, 9 );

    strncpy ( year,  &dateStr [7], 4 );
    strncpy ( date,  &dateStr [4], 2 );
    strncpy ( month, &dateStr [0], 3 );
    strncpy ( hours, &timeStr [0], 2 );
    strncpy ( mins,  &timeStr [3], 2 );
    strncpy ( secs,  &timeStr [6], 2 );

    for ( i = 0; i < 12; i++ )
    {
        if ( strncmp ( month, months [i], 3 ) == 0 )
        {
            break;
        }
    }

    tm.year  = atoi ( year );
    tm.month = i + 1;
    tm.day   = atoi ( date );
    tm.dotw  = dow  ( &tm );

    tm.hour  = atoi ( hours );
    tm.min   = atoi ( mins );
    tm.sec   = atoi ( secs );

    rtc_set_datetime (&tm);                      /* initialise rtc to build date/time */
}

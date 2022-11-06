/* 
 * ATARI ST HDC Emulator
 *
 * File:    emushell.c
 * Author:  Steve Bradford
 * Created: 1st Nov 2022
 * 
 * Shell 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include "pico.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "hardware/vreg.h"
#include "../emu.h"
#include "emushell.h"


int CPUCLK;


/* 
 * cpu clock
 * Minimum clock is 3 * 38 = 114 MHz MHz for this project
 * 
 * cpu clock speeds may seem odd, but they are calculated based on known good SPI
 * clock speeds
 * Gigastone    8GB max 41 MHz
 * Sandisk     16GB max 38 MHz
 * 
 * Need to implement automatic SPI clock setting
 */
bool cpuFreq ( char *input )
{	
    /*                  114       123            138            152       164       184       190            228       246            266       276            304      */
    int speeds [] = { (3 * 38), (3 * 41), 125, (3 * 46), 150, (4 * 38), (4 * 41), (4 * 46), (5 * 38), 200, (6 * 38), (6 * 41), 250, (7 * 38), (6 * 46), 300, (8 * 38) };
    int f;
    int currentClock = clock_get_hz ( clk_sys );

    
    /* just report speeds */
    if ( *input == 0 )
    {
        printf ( "CPU clock is running at %d MHz\n", currentClock / 1000 / 1000 );
        printf ( "SPI clock is running at %d MHz\n", spi_get_baudrate ( spi0 ) / 1000000 );

        return BAD;
    }

    f = atoi ( input );

    if ( f < speeds [0] || f > speeds [ (sizeof (speeds) / 4) - 1] )
    {
        printf ( "\tCPU clock is running at %d MHz\n", clock_get_hz ( clk_sys ) / 1000 / 1000 );

        return BAD;
    }
                                                
    for ( int i = 0; i < (sizeof (speeds) / 4); i++ )               /* set cpu clock to nearest (next highest) valid frequency */
    {
        if ( f <= speeds [i]  )
        {
            CPUCLK = speeds [i];
            break;
        }
    }

    if ( CPUCLK >= 300 )
        vreg_set_voltage ( VREG_VOLTAGE_1_25 ); /* overclock cpu voltage - norm is 1.10v */

    else if ( CPUCLK > 200 )
        vreg_set_voltage ( VREG_VOLTAGE_1_15 ); /* overclock cpu voltage - norm is 1.10v */

    else 
        vreg_set_voltage ( VREG_VOLTAGE_1_10 ); /* overclock cpu voltage - norm is 1.10v */

    sleep_ms (100);

    bool clock = set_sys_clock_khz ( CPUCLK * 1000, false ); /* set PICO clock speed */
    sleep_ms (100);

    if ( !clock ) 
    {
        printf ( "\nUnable to set cpu clock to %d\n", CPUCLK );

        CPUCLK = currentClock;
    }

    /* I am struggling to sort the peripheral bus clock speed */
    /* I have had to create a copy of the "spi_set_baudrate" to get the */
    /* fastest SPI, which is now 24 MHz */

    /* At power-on the SPI is half the cpu clock, which = 62 MHz */
    /* and that gives us an SPI clock of 31 MHz. As soon as I change */
    /* the cpu clock speed, the peripheral clock changes to 48 MHz */
    /* which means the best SPI clock is 24 MHz */
    //extern uint spi_set_baudrate_fast(spi_inst_t *spi, uint baudrate) ;
    spi_set_baudrate_fast( spi0, 31 * MHZ );

    printf ( "\tCPU now running at %d MHz\n", clock_get_hz ( clk_sys ) / 1000 / 1000 );
    printf ( "\tSPI clock is running at %d MHz\n", spi_get_baudrate ( spi0 ) / 1000000 );

    return GOOD;
}

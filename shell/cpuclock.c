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
 * Minimum clock is 125 MHz for this project
 * 
 */
bool cpuFreq ( char *input )
{	
    int speeds [7] = { 100, 125, 133, 150, 200, 250, 300 };
    int f;
    
    if ( *input == 0 )
    {
        printf ( "CPU clock is running at %d MHz\n", clock_get_hz ( clk_sys ) / 1000 / 1000 );
        printf ( "SPI clock is running at %d MHz\n", spi_get_baudrate ( spi0 ) / 1000000 );

        return BAD;
    }

    f = atoi ( input );

    
    if ( f < speeds [0] || f > speeds [6] )
    {
        printf ( "\tCPU clock is running at %d MHz\n", clock_get_hz ( clk_sys ) / 1000 / 1000 );

        return BAD;
    }
                                                
    for ( int i = 0; i < 7; i++ )               /* set cpu clock to nearest (next highest) valid frequency */
    {
        if ( f <= speeds [i]  )
        {
            CPUCLK = speeds [i];
            break;
        }
    }

    if ( CPUCLK == 300 )
        vreg_set_voltage ( VREG_VOLTAGE_1_25 ); /* overclock cpu voltage - norm is 1.10v */

    else if ( CPUCLK > 200 )
        vreg_set_voltage ( VREG_VOLTAGE_1_15 ); /* overclock cpu voltage - norm is 1.10v */

    else 
        vreg_set_voltage ( VREG_VOLTAGE_1_10 ); /* overclock cpu voltage - norm is 1.10v */

    sleep_ms (100);

    set_sys_clock_khz ( CPUCLK * 1000, false ); /* set PICO clock speed */
    sleep_ms (100);

    //clock_configure ( clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, clock_get_hz ( clk_sys ), clock_get_hz ( clk_sys ) );
    //sleep_ms (100);

    //stdio_init_all ();                          /* CMakeLists.txt assigns serial out device - USB / UART */
    //sleep_ms (10);
    printf ( "\tCPU now running at %d MHz\n", clock_get_hz ( clk_sys ) / 1000 / 1000 );
    printf ( "\tSPI clock is running at %d MHz\n", spi_get_baudrate ( spi0 ) / 1000000 );

    return GOOD;
}

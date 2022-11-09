/*
 * ATARI ST HDC Emulator
 * 
 * File:    emuinit.c
 * Author:  Steve Bradford
 * Created: September 2022
 *
 * PICO hardware initialisation
 */

#include <stdio.h>
#include <pico/stdlib.h>
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"
#include "emuinit.h"
#include "shell/emushell.h"




int EMUinit ( void )
{
    /* initialise cpu clock to 133 MHz. No need other than to get higher SPI clock speed (33 MHz) */
    set_sys_clock_khz ( 133 * 1000, false );
    
    clock_configure ( clk_peri, 
                      0, 
                      CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, 
                      clock_get_hz ( clk_sys ), 
                      clock_get_hz ( clk_sys ) );
    
    stdio_init_all ();                          /* CMakeLists.txt assigns serial out device - USB / UART */
    sleep_ms ( 3000 );                          /* give time to minicom (or similar) to establish comms */

    /* initialise GPIO pins */
    gpio_init_mask         ( GPIO_MASK );       /* initialise all GPIO pins for this project - initially all inputs */
    gpio_set_dir_masked    ( GPIO_MASK, GPIO_OUTPUTS_MASK );  
    gpio_set_dir_in_masked ( ACSI_DATA_MASK );

    gpio_set_pulls ( MICROSD_CARD_CS0, true, false );   /* pull-ups */
    gpio_set_pulls ( MICROSD_CARD_CS1, true, false );   /* pull-ups */
    gpio_set_pulls ( MICROSD_CARD_CD0, true, false );   /* pull-ups */
    gpio_set_pulls ( MICROSD_CARD_CD1, true, false );   /* pull-ups */ 

    /* OE on the bidirectional data-bus chip must be kept low on power-up / power-down */
    /* so a higher drive strength is needed to compensate - 3.3v / 1kR = 3mA */
    gpio_set_drive_strength ( DATA_BUS_CNTRL, GPIO_DRIVE_STRENGTH_4MA );  
    
    return 1;
}


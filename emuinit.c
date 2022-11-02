#include <pico/stdlib.h>
#include "emuinit.h"
#include "shell/emushell.h"




int EMUinit ( void )
{
    cpuFreq ( 'd' );                            /* set cpu clock etc. */

    sleep_ms ( 3000 );                          /* give time to minicom (or similar) to establish comms */

    /* initialise GPIO pins */
    gpio_init_mask         ( GPIO_MASK );       /* initialise all GPIO pins for this project - initially all inputs */
    gpio_set_dir_masked    ( GPIO_MASK, GPIO_OUTPUTS_MASK );  
    gpio_set_dir_in_masked ( ACSI_DATA_MASK );

    gpio_set_pulls ( MICROSD_CARD_CS0, true, false );   /* pull-ups */
    gpio_set_pulls ( MICROSD_CARD_CS1, true, false );   /* pull-ups */
    gpio_set_pulls ( MICROSD_CARD_CS2, true, false );   /* pull-ups */
    gpio_set_pulls ( MICROSD_CARD_CS3, true, false );   /* pull-ups */    
    
    return 1;
}


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"

/* project specific includes */
#include "try.pio.h"
#include "error.h"
#include "cmd.h"
#include "hdcemu.h"
#include "bootsectordump.h"

/* FAT filesystem includes */
#include "sd_card.h"
#include "ff.h"
#include "diskio.h"



#define enableDataBus()  gpio_put ( DATA_BUS_ENABLE, LO )
#define disableDataBus() gpio_put ( DATA_BUS_ENABLE, HI )
#define checkCS()        while ( gpio_get ( CS ) == LO )
#define waitCS()         while ( gpio_get ( CS ) == HI )


/* disk drive structures */
struct DU {

    bool     MEDIA_RDY;                 /* flag - uSD is inserted and FS is configured */
    uint32_t error;
    uint32_t lba;                       /* logical block address */
    uint32_t sectorCount;               /* Total sectors */

} driveUnit;

uint smONE;
uint smTWO;
PIO  pioONE;
PIO  pioTWO;
uint8_t DMAbuffer [512 * 32];           /* 16KB */

extern DSTATUS disk_initialize ( BYTE );

/* prototypes */
void doStatus ( uint8_t );
void HWinit ( void );
static int decodeCMD ( CommandDescriptorBlock* );


int main () {

    CommandDescriptorBlock CDB;
    bool notForUs;
    bool timedout;


    HWinit ();                          /* MUST do first */

    memset ( &CDB, 0, sizeof (CommandDescriptorBlock) );

    printf ( "ATARI ST Hard Drive Emulator - v%s\n", VERSION );
    printf ( "Steve Bradford, Sep 2022\n\n" );
    printf ( "Hardware initialised\n" );

    /*
     * STA_NOINIT  = 0x01 Drive not initialized
     * STA_NODISK  = 0x02 No medium in the drive
     * STA_PROTECT = 0x04 Write protected 
     */
    if ( ( driveUnit.MEDIA_RDY = disk_initialize ( 0 ) ) != 0 )
        printf ( "ERROR: Could not initialize SD card %d\n", driveUnit.MEDIA_RDY );

    //disk_ioctl ( 0, GET_SECTOR_COUNT, &driveUnit.sectorCount );
    //printf ( " Number of sectors on drive %u is %lu.\n", 0, driveUnit.sectorCount );

    printf ( "System clock is running at %d MHz\n", clock_get_hz (clk_sys) / 1000000 );
    printf ( "Ready\n\n" );


    gpio_put ( ONBOARD_LED, HI );
    gpio_put ( TRIGGER, LO );

    //pio_sm_set_enabled ( pioONE, smONE, true );
    //pio_sm_set_enabled ( pioONE, smTWO, true );
    //pio_sm_set_enabled ( pioONE, smTWO, true );

    uint32_t start = time_us_32 ();
    uint32_t end;
    gpio_put ( IRQ, HI );

    while (true) {
        
        /* ATARI holds RST low for 12us */
        if ( gpio_get ( RST ) == LO ) {

            //gpio_put ( TRIGGER, LO );
            //gpio_put ( DRQ, HI );
            //gpio_put ( IRQ, HI );
            //gpio_put ( DATA_BUS_ENABLE, HI );

            //gpio_set_function ( IRQ, GPIO_OVERRIDE_HIGH );
            //gpio_set_function ( DRQ, GPIO_OVERRIDE_HIGH );
            //gpio_set_function ( DATA_BUS_ENABLE, GPIO_OVERRIDE_HIGH );

            //pio_sm_clear_fifos ( pioONE, smONE );
            //pio_sm_clear_fifos ( pioONE, smTWO );
        }
        
        

        /* ATARI will timeout at 100ms without getting an acknowledgement ie. status byte */
        /* the peripheral (us), has complete control of bus until status byte is sent */
        else if ( gpio_get ( A1 ) == LO && gpio_get ( RW ) == LO ) {

            notForUs = false;

            enableDataBus ();

            waitCS ();
            CDB.b [0] = gpio_get_all () & ACSI_DATA_MASK;
            gpio_put ( IRQ, HI );
            start = time_us_32 ();

            while ( time_us_32 () < (start + 16) );
            
            gpio_put ( IRQ, LO );
            asm volatile ( 
                "nop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop\nnop\n"
            );
            gpio_put ( IRQ, HI );
            
            if ( (CDB.b [0] >> 5) == 0 ) {

                for ( int d = 1; d < 5; d++ ) {

                    waitCS ();
                    CDB.b [d] = gpio_get_all () & ACSI_DATA_MASK;
                    gpio_put ( IRQ, HI );
                    start = time_us_32 ();
        
                    while ( time_us_32 () < (start + 17) );
                    
                    gpio_put ( IRQ, LO );
                    asm volatile ( 
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"
                    );
                    gpio_put ( IRQ, HI );
                }

                waitCS ();
                CDB.b [5] = gpio_get_all () & ACSI_DATA_MASK;
                disableDataBus ();

                //if ( ! notForUs ) {
        
                    for ( int i = 0; i < 6; i++ ) {

                        if ( i == 0 )
                            printf ( "command = 0x%02x ", CDB.b [i] );
                        
                        else
                            printf ( "0x%02x ", CDB.b [i] );
                    }

                    printf ( "\n" );
        
                    /* ATARI waits for 80us after commands last byte is written out */
                    //decodeCMD ( &CDB );
                //}
            }

            else {
printf ( "command for device %d\n", CDB.b [0] >> 5 );
                disableDataBus ();
            }

            sleep_ms(4);
        }
    }
}






static inline int decodeCMD ( CommandDescriptorBlock *CDB )
{
    uint8_t  opCode;
    uint8_t  controllerNumber;
    uint8_t  length;
    uint8_t  statusLength;
    int      e, n, r;
    uint8_t  driveNumber;
    uint8_t  dataPatternFlags;
    uint8_t  dataPattern;
    uint8_t  status [16];               /* status bytes */
    //uint8_t  DMAbuf [40];               /* DMA buffer */
    uint32_t COMMAND_LENGTH = 6;
    uint32_t raw = 0;
    char     *ptr;


    /* ============= */
    /* command phase */
    /* ============= */

    opCode           = CDB->cmd & OPCODE_MASK;
    controllerNumber = CDB->cmd >> 5;
        
    /* last byte of command - ATARI doesn't do anything for 80us */
    busy_wait_us_32 (40);
    

    /* Handshaking 
     * CS and IRQ - Software handshake used ONLY for commands
     * ATARI asserts CS to transfer a byte. 
     * The peripheral asserts IRQ to notify ATARI it is ready to receive or send the byte.
     * The direction is determined by RW.
     * The handshake is always used for sending commands and receiving the single byte status after
     * command completion.
     * 
     * DRQ and ACK - Hardware handshake used only for data transfers
     * Peripheral asserts DRQ and ATARI asserts ACK.
     * ACK signifies ATARI has received a byte or has one ready to send.
     * DRQ is asserted by the peripheral when it is ready to send or receive a byte
     * 
     */


    /* ============= */
    /* data phase    */
    /* ============= */

    switch ( opCode ) {

        case TEST_UNIT_READY:

            printf ( "TEST_UNIT_READY\n" );

            statusLength = 1;       /* return 1 status byte */

            /* 0 if media installed and ready, 2 if no media or other error, set Check Condition bit*/
            status [0] = ( driveUnit.MEDIA_RDY == ERR_DRV_NO_ERROR ? driveUnit.MEDIA_RDY : CHECK_CONDITION );
            //printf ( "status[0] = 0x%02x\n", status[0] );
            //status [0] = 4; // device not ready
            //status [0] = 30; // controller self test failed

            break;

        case REQUEST_SENSE:
            /* returns a 4 byte status block */
            /* byte 0, bits 0-6 = error code */
            printf ( "REQUEST_SENSE\n" );
            
            driveNumber = CDB->msb >> 5;/* byte 1 = drive number (0-7) */
            length = CDB->len;                 /* return this number of bytes */
            
            /* never return Check Condition here (0x02) */
            DMAbuffer [0] = ERR_DRV_NO_ERROR; /* work TODO here - report last error */

            gpio_set_dir_out_masked (ACSI_DATA_MASK);
            gpio_set_function ( DATA_BUS_ENABLE, GPIO_OVERRIDE_LOW );

            /* DMA status */
            for ( uint8_t i = 0; i < length; i++ ) {
            
                //gpio_put_masked ( ACSI_DATA_MASK, DMAbuffer [i] ); 
                for ( int d = 0; d < 8; d++ ) {

                    if ( DMAbuffer [i] & (1 << d) )
                        gpio_set_function ( d, GPIO_OVERRIDE_HIGH );

                    else
                        gpio_set_function ( d, GPIO_OVERRIDE_LOW );
                }
                   
                gpio_set_function ( DRQ, GPIO_OVERRIDE_LOW );

                while ( gpio_get ( ACK ) == HI )
                        ;

                    asm volatile ( 
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"  
                    );

                    gpio_set_function ( DRQ, GPIO_OVERRIDE_HIGH );

                    while ( gpio_get ( ACK ) == LO )
                        ;
   
                    asm volatile ( 
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n" 
                    );
            }

            gpio_set_function ( DATA_BUS_ENABLE, GPIO_OVERRIDE_HIGH );
            gpio_set_dir_in_masked (ACSI_DATA_MASK);

            statusLength = 1;
            status [0] = ERR_DRV_NO_ERROR;

            printf ( "REQUEST_SENSE completed\n");

            break;

        case FORMAT_DRIVE:

            printf ( "FORMAT_DRIVE\n" );

            dataPatternFlags = (CDB->msb & 0x0f) >> 2;
            driveNumber = CDB->msb >> 5;    /* drive number */

            if ( dataPatternFlags )
                dataPattern = CDB->mid; /* byte to use to fill sectors */

            else
                dataPattern = 0x6c;

            /* NOTE ref. pg36 mentions Mode Select command - what is it? also Data Pattern Flag bits??? */

            /* Interleave Factor is not needed for an emulator */

            break;

        case CMD_READ:
            
            printf ( "CMD_READ\n" );

            driveNumber = CDB->msb >> 5;/* drive number */
            length = CDB->len;          /* sector count */
            
            if ( length > 32 ) {

                printf ( "CMD_READ length too big for 16K buffer %d\n", length );

                return 0;
            }
            
            driveUnit.lba = (uint32_t) ((uint32_t)(CDB->msb & 0x1f << 16) || (uint32_t)(CDB->mid << 8) || CDB->lsb);

            if ( driveNumber != 0 )
                printf ( "OOOPS! drive number %d requested for read does not match expected 0\n", driveNumber );

            if ( driveUnit.MEDIA_RDY == ERR_DRV_NO_ERROR ) {

                if ( driveUnit.lba == 0 )
                    ptr = bootSector;
                    //ptr = drvboot_tools_bin;

                else {

                    if ( ( e = disk_read ( 0, DMAbuffer, driveUnit.lba, length ) ) != FR_OK )
                        printf ( " disk read failed %d\n ", e );

                    ptr = DMAbuffer;
                }
                 

                /* DMA transfers */
                /* Peripheral asserts DRQ, ATARI responds with ACK */
                /* at end assert IRQ */
                gpio_set_dir_out_masked (ACSI_DATA_MASK);
                gpio_set_function ( DATA_BUS_ENABLE, GPIO_OVERRIDE_LOW );

                for ( int dataByte = 0; dataByte < (length << 9); dataByte++ ) {
#ifdef SECTOR_DUMP                
    if ( dataByte % 16 == 0 )
        printf ( "\n" );
    printf ( "0x%02x ", *ptr );
#endif
                    //gpio_put_masked ( ACSI_DATA_MASK, *ptr );
                    for ( int d = 0; d < 8; d++ ) {

                        if ( *ptr & (1 << d) )
                            gpio_set_function ( d, GPIO_OVERRIDE_HIGH );

                        else
                            gpio_set_function ( d, GPIO_OVERRIDE_LOW );
                    }
                    ptr++;
                    
                    gpio_set_function ( DRQ, GPIO_OVERRIDE_LOW );

                    while ( gpio_get ( ACK ) == HI )
                        ;

                    asm volatile ( 
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"  
                    );

                    gpio_set_function ( DRQ, GPIO_OVERRIDE_HIGH );

                    while ( gpio_get ( ACK ) == LO )
                        ;
   
                    asm volatile ( 
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n" 
                    );
                }
            }

            gpio_set_function ( DATA_BUS_ENABLE, GPIO_OVERRIDE_HIGH );
            gpio_set_dir_in_masked (ACSI_DATA_MASK);


            status[0] = 0;
            statusLength = 1;

            printf ( "CMD_READ completed - sent %d bytes, from LBA %d\n", length << 9, driveUnit.lba );

            break;

        case CMD_WRITE:

            printf ( "CMD_WRITE\n" );

            driveNumber = CDB->msb >> 5;    /* drive number */
            length = CDB->len;              /* block count */
            driveUnit.lba = (uint32_t)((CDB->msb & 0x1f) << 16) || (uint32_t)(CDB->mid << 8) || CDB->lsb;

            break;

        case CMD_SEEK:

            printf ( "CMD_SEEK\n" );

            driveNumber = CDB->msb >> 5;    /* drive number */
            length = CDB->len;              /* block count */
            driveUnit.lba = (uint32_t)((CDB->msb & 0x1f) << 16) || (uint32_t)(CDB->mid << 8) || CDB->lsb;
            
            /* don't need this command */

            break;

        
        case CMD_INQUIRY:

            printf ( "CMD_INQUIRY\n" );

            /* respond with DMA 16 bytes */
            /* NOTE ref. pg31 is ambiguous about the status byte count */

            //memset ( DMAbuffer, 0, sizeof (DMAbuf) );

            length = CDB->len;

            //if ( length == 0 )
            //    length = 4;

            /* check for valid drive number */
            /* TODO */

            DMAbuffer [1] = 0x80;          /* removable flag */
            DMAbuffer [2] = 0x01;          /* ACSI version */

            DMAbuffer [4] = 0x1f;          /* data length */

            sprintf ( &DMAbuffer [8], "%s%s", INQUIRY_STRING, VERSION ); /* max of 29 chars */

            gpio_set_dir_out_masked (ACSI_DATA_MASK);
            gpio_set_function ( DATA_BUS_ENABLE, GPIO_OVERRIDE_LOW );

            for ( uint8_t i = 0; i < length; i++ ) {
                
                for ( int d = 0; d < 8; d++ ) {

                    if ( DMAbuffer [i] & (1 << d) )
                        gpio_set_function ( d, GPIO_OVERRIDE_HIGH );

                    else
                        gpio_set_function ( d, GPIO_OVERRIDE_LOW );
                }

                gpio_set_function ( DRQ, GPIO_OVERRIDE_LOW );

                while ( gpio_get ( ACK ) == HI )
                    ;

                asm volatile ( 
                        "nop\nnop\nnop\nnop\nnop\n"
                        "nop\nnop\nnop\nnop\nnop\n"  
                    );

                gpio_set_function ( DRQ, GPIO_OVERRIDE_HIGH );

                while ( gpio_get ( ACK ) == LO )
                    ;

                // 200ns delay
                asm volatile ( 
                    "nop\nnop\nnop\nnop\nnop\n"
                    "nop\nnop\nnop\nnop\nnop\n"
                );
            }

            gpio_set_function ( DATA_BUS_ENABLE, GPIO_OVERRIDE_HIGH );
            gpio_set_dir_in_masked (ACSI_DATA_MASK);

            /* TODO */
            statusLength = 1;
            status [0] = ERR_DRV_NO_ERROR;

            break;

        default:
            printf ( "Unknown command 0x%0x\n", opCode );

            sleep_us ( 100 );
            
            return 0;
    }


    /* ============= */
    /* status phase  */
    /* ============= */

    busy_wait_us_32 (80);

    doStatus ( status [0] );

    /* ATARI doesn't want anything else for upto 40us */
    busy_wait_us_32 (39);

    return 1;
}



void doStatus ( uint8_t status ) {

    /* send status byte to state machine and wait for it to complete */
    pio_sm_put ( pioONE, smTWO, (uint32_t)status ); /* write one word to TXFIFO */
    
    gpio_put ( TRIGGER, HI );
    busy_wait_us_32 (1);
    gpio_put ( TRIGGER, LO );

    printf ( "Status - sent %d\n", status );

}



void HWinit ( void ) {

    int  CPUCLK;
    bool clk;
    uint baudrate = 400000;             /* 400KHz */


    CPUCLK = 200000;					/* default clock speed = 100 MHz (10ns clock) */
	clk = set_sys_clock_khz ( CPUCLK, true ); 	/* set PICO clock speed */

    stdio_init_all (); 

    sleep_ms ( 2000 );

    pioONE = pio0;
    pioTWO = pio1;


     /* initialise GPIO pins */
    gpio_init_mask ( GPIO_MASK );       /* initialise all GPIO pins for this project - initially all inputs */
    gpio_set_dir_masked ( GPIO_MASK, GPIO_OUTPUTS_MASK ); 

    gpio_put ( DATA_BUS, HI );
    gpio_put ( AIRQ, HI );
    gpio_put ( ADRQ, HI );
    gpio_put ( MICROSD_CARD_CS0, HI );
    gpio_put ( MICROSD_CARD_CS1, HI );
    gpio_put ( TRIGGER, LO );
    gpio_put ( ONBOARD_LED, LO );

    
    // PIO to control 3 GPIOs (as output)
    //pio_gpio_init ( pioONE, AIRQ );
    //pio_gpio_init ( pioTWO, AIRQ );
    //pio_gpio_init ( pioONE, ADRQ );
    //pio_gpio_init ( pioONE, DATA_BUS );
    for ( int i = 0; i < 8; i++ ) {

        gpio_pull_up ( i );
    //    pio_gpio_init ( pioONE, i );
    }


    /* initialise SPI - using GPIO 16, 17, 18, 19 */
    /* https://github.com/raspberrypi/pico-examples/blob/master/spi/bme280_spi/bme280_spi.c */
    spi_init ( spi_default, baudrate );
    
    gpio_set_function ( PICO_DEFAULT_SPI_RX_PIN,  GPIO_FUNC_SPI );
    gpio_set_function ( PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI );
    gpio_set_function ( PICO_DEFAULT_SPI_TX_PIN,  GPIO_FUNC_SPI );



    /* configure PIO */
    //smONE = pio_claim_unused_sm ( pioONE, true );
    //smTWO = smONE + 1; //pio_claim_unused_sm ( pioTWO, true );

   // uint tryOffset         = pio_add_program ( pioONE, &try_program );
    //uint statusPhaseOffset = pio_add_program ( pioONE, &statusPhase_program );
    //uint irqPulseOffset    = pio_add_program ( pioONE, &irqPulse_program );

    //try_program_init         ( pioONE, smONE, tryOffset );
    //statusPhase_program_init ( pioONE, smTWO, statusPhaseOffset );
    //irqPulse_program_init    ( pioONE, smTWO, irqPulseOffset );

    //pio_sm_clear_fifos ( pioONE, smONE );
    //pio_sm_clear_fifos ( pioONE, smTWO );
}
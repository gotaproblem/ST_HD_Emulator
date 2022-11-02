/*
 * ATARI ST Hard Drive Emulator
 *
 * Steve Bradford
 * September 2022
 * 
 * Version 1.0
 * 
 * Using Raspberry PI PICO RP2040
 * GPIO 00 - 29 are available to the programmer
 * GPIO 16, 17, 18, 19 reserved for SPI0
 * GPIO 23, 25 are not to be used because of board design
 * 
 * SPI0 assignments
 * 16   SPI0_Rx   MISO (Master In Slave Out)
 * 17   SPI0_CS
 * 18   SPI0_CLK
 * 19   SPI0_Tx   MOSI (Master Out Slave In)
 * 
 * GPIO assignments
 * 00   Data bit 0              BIDIRECTIONAL
 * 01   Data bit 1              BIDIRECTIONAL
 * 02   Data bit 2              BIDIRECTIONAL
 * 03   Data bit 3              BIDIRECTIONAL
 * 04   Data bit 4              BIDIRECTIONAL
 * 05   Data bit 5              BIDIRECTIONAL
 * 06   Data bit 6              BIDIRECTIONAL
 * 07   Data bit 7              BIDIRECTIONAL
 * 08   /CS                     INPUT
 * 09   /IRQ                    OUTPUT
 * 10   /RST                    INPUT
 * 11   /ACK                    INPUT
 * 12   A1                      INPUT
 * 13   R/W                     INPUT
 * 14   /DRQ                    OUTPUT
 * 15   /Interface Enable       OUTPUT          control tri-state octal driver chip on the data bus - low to enable
 *                                      
 *                                              NOTE - Hardware design - tie the DIR pin on the tri-state buffer to the RW signal
 *                                                     meaning GPIO 20 will not be needed
 * 16
 * 17   microSD card 1 CS       OUTPUT
 * 18
 * 19
 * 20   microSD card 2 CS       OUTPUT
 * 
 * The ACSI bus can address 8 controllers (0-7). Each controller can talk to a maximum of 8 drives.
 * This Emulator is designed to be controller 0.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pico/stdlib.h>
#include <pico/util/datetime.h>
#include "pico.h"
#include "hardware/rtc.h"
#include "pico/multicore.h"
//#include "hardware/gpio.h"
//#include "hardware/spi.h"
#include "hardware/clocks.h"

/* project specific includes */
#include "ff.h"
#include "emu.h"
#include "emuerror.h"
#include "emucmd.h"
#include "emurtc.h"
#include "shell/emushell.h"
#include "emuscsi.h"
#include "emuinit.h"
#include "emufileio.h"




/* prototypes */
int     getCMD   ( CommandDescriptorBlock * );
void    doStatus ( uint8_t );
uint8_t rdDMA    ( uint8_t *, uint32_t );
uint8_t wrDMA    ( uint8_t *, uint32_t );



/* 56 bytes */
uint8_t INQUIRY_DATA [] =
{
	0,                                          /* device type 0 = direct access device */
	0x80,                                       /* device type qualifier (removable) */
	2,                                          /* ACSI/SCSI version */
	2,                                          /* reserved */
	51,                                         /* length of the following data */
	0, 0, 0,                                    /* Vendor specific data */
	'B','B','a','N',' ',' ',' ',' ',            /* T10 Vendor ID    8 chars */
	'A','C','S','I',' ','H','D','C',            /* Product ID       8 chars */
	'S','D','0',' ',' ',' ',' ',' ',            /* Product Revision 8 chars */
	'S','D','0',' ',' ',' ',' ',' ',            /* Drive Serial Number  */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0             /* Vendor Unique - Empty */
};


uint8_t  DMAbuffer [512 * 256] __not_in_flash();/* max transfer size 128 KB */
DRIVES   drv [4];                               /* one controller can have 4 drives (uSD cards) */
int      lastSeek;
volatile uint32_t intState;                     /* interrupt state save/restore */
volatile bool     gotRST;
volatile bool     doPrint;
uint32_t gStatus;                               /* used for printing - copy of CDB */
CommandDescriptorBlock gCDB;                    /* used for printing - copy of CDB */



/*
 * test - run on second core
 * does this improve performance?
 */

void __not_in_flash_func (core1Entry) (void)
{
    int offset = 0;
    CommandDescriptorBlock sCMD;


    memset ( &sCMD, 0, sizeof (CommandDescriptorBlock) );

    gpio_set_dir_in_masked (ACSI_DATA_MASK);
    
    IRQ_HI ();
    DRQ_HI ();
    dataBus (DISABLE);
    disableInterrupts ();


    while ( 1 ) {

        /* 
         * ATARI holds RST low for 12us 
         *
         */
        
        if ( gpio_get ( RST ) == LO )
        {
            dataBus (DISABLE);
            IRQ_HI ();
            DRQ_HI ();
            
            gpio_set_dir_in_masked (ACSI_DATA_MASK); 
#if DEBUG
            printf ( "RESET\n" );
#endif            
            waitRST ();                         /* wait for RST to go high */
        }
        
        /* ============= */
        /* command phase */
        /* ============= */

        /* 
         * check GPIO for new command packet - A1 and RW = low
         * 
         * time critical, code MUST be kept to the minimum, otherwise, can miss bytes
         * interrupts should be disabled for the duration of the CDB building at least
         * 
         */
        
        if ( newCMD () ) 
        { 
            dataBus (ENABLE);

            waitCS ();                          /* wait for CS low */
            sCMD.b [0] = rdDataBus ();          /* data is available to read now - byte 0 */
            waitRW (LO);                        /* wait for RW to go high before continuing */

            /* check this command is for this controller 
             * better to do in hardware with a switch selection */
            if ( sCMD.DEVICE.target == CONTROLLER_ADDRESS || sCMD.DEVICE.target > 5 )
            {
                sCMD.cmdLength = 6;

                IRQ_LO ();                      /* take IRQ low to signal ATARI receipt */

                while ( gpio_get (A1) == LO );  /* subsequent command bytes are read whilst A1 is high */

                waitCS (); 
                IRQ_HI ();  
                sCMD.b [1] = rdDataBus ();      /* byte 2 - check for special/extended */
                waitRW (LO);
                                                /* we have a maximum of 12us to assert IRQ */
                if ( sCMD.b [0] == 0x1f )
                {
                    sCMD.cmdLength = 7;

                    if ( sCMD.b [1] >= 0xa0 )
                    {
                        sCMD.cmdLength = 13;
                    }

                    else if ( sCMD.b [1] >= 0x80 )
                    {
                        sCMD.cmdLength = 17;
                    }

                    else if ( sCMD.b [1] >= 0x20 )
                    {
                        sCMD.cmdLength = 11;
                    }
                }

                IRQ_LO ();
                for ( int d = 2; d < sCMD.cmdLength; d++ )
                {       
                    waitCS ();                  /* ATARI tells us another byte is ready */
                    IRQ_HI ();  
                    sCMD.b [d] = rdDataBus ();  /* bytes 1-5 (6) */
                    waitRW (LO);         
                                                /* the last command byte does NOT assert IRQ */
                    if ( d == (sCMD.cmdLength - 1) )
                        break;
                                      
                    IRQ_LO ();                  /* signal ATARI we are ready for next byte */
                }        

                getCMD ( &sCMD );               /* process ACSI/SCSI command */
            }

            else                                /* ignore command, it wasn't for us */
            {                                   /* ATARI will timeout */
                IRQ_HI ();
                DRQ_HI ();
            }

            /* end of command */
            dataBus (DISABLE);                  /* data-bus trisate */
        }
    }
    
    return;
}






/* 
 * use cpu core 0 to print command status
 * optimise to reduce overheads 
 */

static inline void printCmd ( CommandDescriptorBlock *CDB, int status ) 
{    
    char cmdString [16];
    char debugLine [80];
    
    switch ( CDB->DEVICE.opcode )
    {
        case TEST_UNIT_READY:
            sprintf ( cmdString, "TEST_UNIT_READY" );
            break;
        case REQUEST_SENSE:
            sprintf ( cmdString, "REQUEST_SENSE  " );
            break;
        case FORMAT_UNIT:
            sprintf ( cmdString, "FORMAT_UNIT    " );
            break;
        case CMD_READ:
            sprintf ( cmdString, "CMD_READ       " );
            break;
        case CMD_WRITE:
            sprintf ( cmdString, "CMD_WRITE      " );
            break;
        case CMD_SEEK:
            sprintf ( cmdString, "CMD_SEEK       " );
            break;
        case CMD_INQUIRY:
            sprintf ( cmdString, "CMD_INQUIRY    " );
            break;
        case MODE_SELECT:
            sprintf ( cmdString, "MODE_SELECT    " );
            break;
        case MODE_SENSE:
            sprintf ( cmdString, "MODE_SENSE     " );
            break;
        case MEDIA_REMOVE:
            sprintf ( cmdString, "MEDIA_REMOVE   " );
            break;
        case EXTENDED_CMD:
            sprintf ( cmdString, "EXTENDED_CMD   " );
            break;
        default:
            sprintf ( cmdString, "UNKNOWN_COMMAND" );
            break;
    }
   
    for ( int i = 0; i < CDB->cmdLength; i++ )
    {
        if ( i == 2 && CDB->cmdLength == 6 )
            sprintf ( debugLine, "%s tgt=%d lun=%d (0x%02x:%02x:%02x:", 
                    cmdString, 
                    CDB->DEVICE.target, 
                    CDB->msb >> 5,
                    CDB->DEVICE.opcode,
                    CDB->b [1],
                    CDB->b [2]
                    );
        
        else if ( i == 2 && CDB->cmdLength > 6 )
            sprintf ( debugLine, "%s tgt=%d lun=%d (0x%02x:%02x:%02x:", 
                    cmdString, 
                    CDB->b [2] & 0x1f, 
                    CDB->b [2] >> 5,
                    CDB->DEVICE.opcode,
                    CDB->b [1],
                    CDB->b [2]
                    );
        
        if ( i < 3 )
            continue;

        sprintf ( debugLine + strlen (debugLine), "%02x:", CDB->b [i] );
    }

    sprintf ( debugLine + strlen (debugLine), "\b \b) (%02x)\n", status );
    
    printf ( debugLine );                       /* one printf to stdio */
}



int main ( void )
{
    datetime_t dt;
    char datetime_buf [80];


    EMUinit ();                                 /* MUST do first */
    rtc_init ();
    buildDateTime ();                           /* use compilation date/time to initialise RTC */

    printf ( "\n\n\nATARI ACSI Hard Drive Controller Emulator - v%s\n", VERSION );
    printf ( "Steve Bradford, Sep 2022\n\n" );
    printf ( "Hardware initialised\n\n" );
    printf ( "CPU clock is running at %d MHz\n\n", clock_get_hz (clk_sys) / 1000000 );

    if ( ! mountFS () )
    {
        printf ( "\nERROR: no drives mounted\nWill now try a raw mount\n\n" );

        if ( ! mountRAW () )
        {
            printf ( "\nERROR: no drives mounted\n\n" );
        }
    }

    /* spi speed can't be got until fs is mounted */
    printf ( "\nSPI clock is running at %d MHz\n", spi_get_baudrate ( spi0 ) / 1000000 );
    printf ( "RTC is %s\n", rtc_running ? "enabled" : "disabled" );

    if ( rtc_running )
    {
        rtc_get_datetime (&dt);
        datetime_to_str  ( datetime_buf, sizeof (datetime_buf), &dt );
        
        printf ( "RTC = %s\n", datetime_buf );
        printf ( "Set date/time if required via shell\n" );
    }

    printf ( "\nACSI HDC Emulator Running ... press ESC for shell\n\n" );

    doPrint = false;                            /* flag to show if debug print is needed */

    multicore_launch_core1 ( core1Entry );      /* one whole cpu to read commands in */

    gpio_put ( ONBOARD_LED, HI );               /* show we are initialised and running */

    /* 
     * do something in core 0 :) 
     */
    while ( true ) 
    {
#if !DEBUG
        rtc_get_datetime (&dt);
        datetime_to_str  ( datetime_buf, sizeof (datetime_buf), &dt );
        printf ( "\033[2J\r%s      ", datetime_buf );

        sleep_ms (1000);
#else
        /* do the debug print on this CPU to reduce overheads on other CPU */
        if ( doPrint == true )
        {
            doPrint = false;

            printCmd ( &gCDB, gStatus );
        }
#endif

        /* enter shell on ESC key */
        if ( getchar_timeout_us (0) == 0x1b )
        {
            doShell ();
            //sleep_ms (1000);
        }
    }

    return 0;
}



int __not_in_flash_func (getCMD) ( CommandDescriptorBlock *CDB )
{  
    uint8_t  LUN;
    uint8_t  dataPatternFlags;
    uint8_t  dataPattern;
    DRIVES   *pdrv = &drv [0]; /* for now, hard code to use one sd card */
   

    /* ============= */
    /* data phase    */
    /* ============= */

    switch ( CDB->DEVICE.cmd ) {

        case TEST_UNIT_READY:        
            /* 
                * 0x00 No error 
                * 0x04 Device not ready 
                * 0x30 Controller self test failed
                * 
                * The check condition bit (1) should be set if error
                * 
                * NOTE status byte bits 5-7 = controller number
                */
            LUN = CDB->b [1] >> 5;

            /* ICD RTC */
            if ( LUN == 6 )
            {
                if ( ! drv [0].luns [6].mounted )
                    drv [0].luns [6].mounted = true;
            }

            pdrv->status = ( pdrv->luns [LUN].mounted ? ERR_DRV_NO_ERROR : CHECK_CONDITION );

            /* do I need SCSI error here ?? */

            lastSeek = false;

            break;

        /* SCSI SPC-5 */
        case REQUEST_SENSE:
            pdrv->status = ERR_DRV_NO_ERROR;

            LUN = CDB->msb >> 5;

            if ( pdrv->luns [LUN].mounted == false )
            //if ( pdrv->mounted == false ) 
            {
                pdrv->status = ERR_CMD_INVALID_ADD; //ERR_CMD_INVALID_DRV;
                pdrv->lastError = SCSI_ERR_INV_LUN;
            }

            memset ( DMAbuffer, 0, CDB->len );

            /* never return Check Condition here (0x02) */
            if ( CDB->len <= 4 ) 
            {
                CDB->len = 4;
                
            //    DMAbuffer [0] = 0x80 | pdrv->lastStatus; 
            //    DMAbuffer [1] = pdrv->lba >> 16;
            //    DMAbuffer [2] = pdrv->lba >> 8;
            //    DMAbuffer [3] = pdrv->lba;
            }
//#ifdef TODO
            //else {
                // Build long response in buf
                //CDB->len = 22;

                DMAbuffer [0] = 0x70;       /* Response Code (byte 0) = Current Information */

                // if (lastSeek) {

                    //DMAbuffer [0] |= 0x80;
                //  DMAbuffer [4] = pdrv->lba >> 16;
                //  DMAbuffer [5] = pdrv->lba >> 8;
                //  DMAbuffer [6] = pdrv->lba;
                //}
                DMAbuffer [1] = //(pdrv->lastError >> 16) & 0xff;
                DMAbuffer [2] = (pdrv->lastError >> 16) & 0xff; //(pdrv->lastError >> 8) & 0xff;
                DMAbuffer [3] = //pdrv->lastError & 0xff;
                DMAbuffer [7] = 14; // n-7

                /* byte 8 onwards - sense data descriptor list */
                /* sense data */
                /* 0 - descriptor type
                    *     0x00 information sense, 
                    *     0x01 command specific, 
                    *     0x02 sense key, 
                    *     0x03 field replacable unit, 
                    *     0x0a another progress indication, 
                    *     0x0c foreward sense, 
                    *     0x0e device designation, 
                    *     0x0f microcode activation, 
                    *     0x80-0xff vendor specific
                    * 1 - additional length n - 1
                    * 2 ... n sense data descriptor specific
                    */
                DMAbuffer [8] =  0;     /* byte 0 descriptor type */
                DMAbuffer [9] =  0x0a;  /* byte 1 additional length */
                DMAbuffer [10] = 0x80;  /* byte 2 reserved with valid bit set */
                DMAbuffer [11] = 0;     /* byte 3 reserved */
                DMAbuffer [12] = (pdrv->lastError >> 8) & 0xff;  /* byte 4 information MSB */
                DMAbuffer [13] = pdrv->lastError & 0xff;
                DMAbuffer [14] = 0;
                DMAbuffer [15] = 0;
                DMAbuffer [16] = 0;
                DMAbuffer [17] = 0;
                DMAbuffer [18] = 0;
                DMAbuffer [19] = 0;      /* byte 11 information LSB */
                
                switch ( pdrv->lastStatus )
                {
                    case ERR_DRV_NO_ERROR: 
                        DMAbuffer [2] = SCSI_SK_NO_SENSE; 
                        break;
                    case ERR_CMD_INVALID_CMD:
                    case ERR_CMD_INVALID_ADD:
                    case ERR_CMD_INVALID_ARG:
                    case ERR_CMD_INVALID_DRV:
                        DMAbuffer [2] = SCSI_SK_ILLEGAL_REQUEST; 
                        break;

                    default: 
                        DMAbuffer [2] = SCSI_SK_HARDWARE_ERROR; 
                        break;
                }
                //DMAbuffer [7]  = 14;
                //DMAbuffer [12] = pdrv->lastStatus;
                //DMAbuffer [19] = pdrv->lba >> 16;
                //DMAbuffer [20] = pdrv->lba >> 8;
                //DMAbuffer [21] = pdrv->lba;
            //}
//#endif
            wrDMA ( DMAbuffer, CDB->len );

            lastSeek = false;

            break;

        case FORMAT_UNIT:
            LUN = CDB->msb >> 5;

            dataPatternFlags = (CDB->msb & 0x0f) >> 2;

            if ( dataPatternFlags )
                dataPattern = CDB->mid;     /* byte to use to fill sectors */

            else
                dataPattern = 0x6c;

            /* NOTE ref. pg36 mentions Mode Select command - what is it? also Data Pattern Flag bits??? */

            enableInterrupts ();
            disableInterrupts ();
        
            /* Interleave Factor is not needed for an emulator */
            

            
            pdrv->status = ERR_DRV_NO_ERROR;

            //printf ( "FORMAT_DRIVE completed\n");

            break;

        case CMD_READ:
            LUN = CDB->msb >> 5;

            pdrv->lastError = SCSI_ERR_OK;       /* SCSI error code */
            pdrv->status    = ERR_DRV_NO_ERROR;
            pdrv->lba       =  ((uint32_t)CDB->msb & 0x1f) << 16; 
            pdrv->lba       |= (uint32_t)CDB->mid << 8;
            pdrv->lba       |= CDB->lsb;
            lastSeek        = true;

            if ( ! pdrv->luns [LUN].mounted )
            {
                pdrv->lastError = SCSI_ERR_INV_LUN;
                pdrv->status    = ERR_CMD_INVALID_DRV;
            }

            else 
            {
                fileIO ( FREAD, CDB, pdrv );
            }
        
            break;

        case CMD_WRITE:
            LUN = CDB->msb >> 5;

            pdrv->lastError = SCSI_ERR_OK;
            pdrv->status    = ERR_DRV_NO_ERROR;
            pdrv->lba       =  ((uint32_t)CDB->msb & 0x1f) << 16; 
            pdrv->lba       |= (uint32_t)CDB->mid << 8;
            pdrv->lba       |= CDB->lsb;
            lastSeek        = true;
            
            if ( ! pdrv->luns [LUN].mounted ) 
            {
                pdrv->lastError = SCSI_ERR_INV_LUN;
                pdrv->status    = ERR_CMD_INVALID_DRV;
            }

            else 
            {
                fileIO ( FWRITE, CDB, pdrv );
            }

            break;

        case CMD_SEEK:
            LUN = CDB->msb >> 5;

            pdrv->lba = (((uint32_t)CDB->msb & 0x1f) << 16) | 
                        ((uint32_t)CDB->mid << 8) 
                        | CDB->lsb;
            lastSeek = true;

            /* don't need this command */

            enableInterrupts ();
            disableInterrupts ();

            break;


        case CMD_INQUIRY:
            LUN = CDB->msb >> 5;

            if ( CDB->len <= 4 )
                CDB->len = 4; //16; /* fill ATARI FIFO */ 
#if DEBUG 
            if ( CDB->len != sizeof (INQUIRY_DATA) )
            {
            //    printf ( "INQUIRY: cmd expecting %d bytes, but we have %d\n", 
                //           CDB->len, sizeof (INQUIRY_DATA) );
            }
#endif                
            memcpy ( DMAbuffer, INQUIRY_DATA, sizeof (INQUIRY_DATA) );

            if ( LUN == 0 )
                DMAbuffer [0] = 0x00;

            else
                DMAbuffer [0] = 0x7f;

            wrDMA ( DMAbuffer, CDB->len );

            pdrv->status = ERR_DRV_NO_ERROR;
            lastSeek = false;
            
            break;


        /* SCSI commands */
        // 0 OPERATION CODE (1Ah)
        // 1 DBD (disable block descriptors) bit: bit 7 to 4 RES, bit3 DBD, bit 2 to 0 RES. --> DBD (disable block descriptors) bit.
        // 2 PC (Page Control) field: First two bit (PC, Page Control) field, bit 5 to 0 PAGE CODE.
        // 3 SUBPAGE CODE: The PAGE CODE and SUBPAGE CODE fields specify which mode pages and subpages to return (see table 349)
        // 4 ALLOCATION LENGTH: The ALLOCATION LENGTH field is defined in 2.2.6.
        // 5 CONTROL:
        //
        // Error
        // send Status 2
        // Success
        case MODE_SENSE:
            LUN = CDB->msb >> 5;

            pdrv->status = ERR_DRV_NO_ERROR;
            lastSeek     = false;

            /* get sub-command */
            switch ( CDB->b [2] ) {

                /* MODE SENSE 0: */
                case 0x00:
                    modeSense0 ( DMAbuffer, pdrv, LUN );

                    wrDMA ( DMAbuffer, 16 );

                    break;

                /* MODE SENSE 4: */
                case 0x04:
                    modeSense4 ( DMAbuffer, pdrv, LUN );

                    wrDMA ( DMAbuffer, 24 );

                    break;

                case 0x3f:
                    memset ( DMAbuffer, 0, 44 );

                    DMAbuffer [0] = 44;
                    DMAbuffer [1] = 0;
                    DMAbuffer [2] = 0;
                    DMAbuffer [3] = 0;

                    modeSense4 ( DMAbuffer + 4, pdrv, LUN );
                    modeSense0 ( DMAbuffer + 28, pdrv, LUN );

                    wrDMA ( DMAbuffer, 44 );

                    break;

                default:

                    printf ( "MODE_SENSE: unsupported mode sense 0x%02x\n", CDB->b [2] );
                    pdrv->status = ERR_CMD_INVALID_ARG;
                    break;
            }

            break;


        /* 
            * non-ACSI, 
            * so look for extended commands, SCSI commands or vendor specials ... 
            */
        default:

            switch ( CDB->b [0] )
            {
                /* extended commands */
                case 0x1f:

                    pdrv->status = ERR_DRV_NO_ERROR;

                    switch ( CDB->b [1] )
                    {
                        /* A2STM image */
                        case 0x0c:
                            //a2stmPtr = a2setup_boot_bin;
                            //a2stmLen = a2setup_boot_bin_len;

                            //wrDMA ( a2stmPtr, a2stmLen );

                            break;

                        /* A2STM image */
                        case 0x0d:
                            //a2stmPtr = a2stdrv_boot_bin;
                            //a2stmLen = a2stdrv_boot_bin_len;

                            //wrDMA ( a2stmPtr, a2stmLen );

                            break;

                        /* 0x12 SCSI Inquiry */
                        case 0x12:
                            memcpy ( DMAbuffer, INQUIRY_DATA, sizeof (INQUIRY_DATA) );
                            
                            DMAbuffer [4] = CDB->b [5];

                            wrDMA ( DMAbuffer, CDB->b [5] );

                            lastSeek = false;

                            break;

                        /* Ultrasatan RTC */
                        case 0x20:
                            printf ( "Ultrasatan RTC request\n" );
                            break;

                        /* 0x25 SCSI Read Capacity */
                        case SCSI_OP_READ_CAPACITY:
                            pdrv->lastError = SCSI_ERR_OK;       /* SCSI error code */
                            pdrv->status    = ERR_DRV_NO_ERROR;

                            memset ( DMAbuffer, 0, 16 );

                            LUN = CDB->b [2] >> 5;

                            { 
                                uint32_t capacity = pdrv->luns [LUN].sectorCount;

                                DMAbuffer [0] = (capacity >> 24) & 0xff;
                                DMAbuffer [1] = (capacity >> 16) & 0xff;
                                DMAbuffer [2] = (capacity >> 8)  & 0xff;
                                DMAbuffer [3] = capacity & 0xff;
                            }

                            DMAbuffer [4] = 0;
                            DMAbuffer [5] = 0;
                            DMAbuffer [6] = 0x02;  /* always 512 bytes */
                            DMAbuffer [7] = 0x00;

                            wrDMA ( DMAbuffer, 16 );

                            lastSeek = false;

                            break;

                        /* 0x28 SCSI Read */
                        case SCSI_OP_READ:
                        /* 0x3c SCSI Read Buffer */
                        case SCSI_OP_READ_BUFFER:
                            pdrv->lastError = SCSI_ERR_OK;       /* SCSI error code */
                            pdrv->status    = ERR_DRV_NO_ERROR;

                            pdrv->lba = (( (uint32_t)CDB->b [3]) << 24) | 
                                            (((uint32_t)CDB->b [4]) << 16) | 
                                            (((uint32_t)CDB->b [5]) << 8) | 
                                            (uint32_t)(CDB->b [6]);

                            pdrv->len =  (((uint32_t)CDB->b [8]) << 8) | 
                                                        CDB->b [9];

                            CDB->len = 0;
                            lastSeek = true;

                            fileIO ( FREAD, CDB, pdrv );

                            break;

                        /* 0x2a SCSI WRITE */
                        case SCSI_OP_WRITE:
                        /* 0x3b SCSI Write Buffer */
                        case SCSI_OP_WRITE_BUFFER:
                            pdrv->lastError = SCSI_ERR_OK;       /* SCSI error code */
                            pdrv->status    = ERR_DRV_NO_ERROR;

                            pdrv->lba = (( (uint32_t)CDB->b [3]) << 24) | 
                                            (((uint32_t)CDB->b [4]) << 16) | 
                                            (((uint32_t)CDB->b [5]) << 8) | 
                                            (uint32_t)(CDB->b [6]);

                            pdrv->len =  (((uint32_t)CDB->b [8]) << 8) | 
                                                        CDB->b [9];

                            CDB->len = 0;
                            lastSeek = true;

                            fileIO ( FWRITE, CDB, pdrv );

                            break;

                        /* unknown opcode */
                        default:
                            lastSeek = false;
                            pdrv->status = ERR_CMD_INVALID_CMD; 

                            break;
                    }

                    break;

                default:
                    lastSeek = false;
                    pdrv->status = ERR_CMD_INVALID_CMD; 

                    break;
            }

            break;
    }
    
    /* ============= */
    /* status phase  */
    /* ============= */

#if DEBUG 
    memcpy ( &gCDB, CDB, sizeof (CommandDescriptorBlock) );
    gStatus = pdrv->status;
    doPrint = true;
#endif 

    doStatus ( pdrv->status );
    
    pdrv->lastStatus = pdrv->status;

    return 1;
}



/* optimise function by telling compiler to put function in RAM */
void __not_in_flash_func (doStatus) ( uint8_t status )
{ 
    IRQ_LO ();                                  /* completes DMA (if any) */
    waitRW (LO);                                /* make sure RW is high = READ */
    
    gpio_set_dir_out_masked (ACSI_DATA_MASK);

    waitCS ();
    IRQ_HI ();
    wrDataBus ( status );
    waitRW (HI);
    
    gpio_set_dir_in_masked (ACSI_DATA_MASK);

    //pio_sm_put ( pioONE, smTWO, status );
    //pio_sm_restart ( pioONE, smTWO );
}



/*
 * from perspective of peripheral
 * read DMA means the ATARI is doing a DMA write to us
 */
inline uint8_t __not_in_flash_func (rdDMA) ( uint8_t *ptr, uint32_t length ) 
{
    register int n;

    /* DMA transfers - read */
    /* Peripheral asserts DRQ, ATARI responds with ACK */
    /* RW stays low (WRITE) for duration */
    /* at end assert IRQ to complete transfer - done in doStatus () */
    
    waitWR ();                                  /* make sure RW is low (WRITE) */
    
    gpio_set_dir_in_masked (ACSI_DATA_MASK);
    
    for ( uint32_t d = 0; d < length; d++ ) {

        DRQ_LO ();
        waitACK (HI);
        *ptr++ = rdDataBus ();
        DRQ_HI ();
        waitACK (LO);

        /* apparently have to wait a minimum of 240ns before next DRQ LO */
        for ( n = 12; n; n-- )
            Nop ();
    }
    
    return ERR_DRV_NO_ERROR;
}


/*
 * from perspective of peripheral
 * write DMA means the ATARI is doing a DMA read
 */
inline uint8_t __not_in_flash_func (wrDMA) ( uint8_t *ptr, uint32_t length ) 
{
    register int n;

    /* DMA transfers - write */
    /* Peripheral asserts DRQ, ATARI responds with ACK */
    /* RW stays high (READ) for duration */
    /* at end assert IRQ */
    
    waitRD ();
    
    gpio_set_dir_out_masked (ACSI_DATA_MASK);
    
    for ( uint32_t d = 0; d < length; d++ ) {

        DRQ_LO ();
        waitACK (HI);
        wrDataBus ( *ptr++ );
        DRQ_HI ();
        waitACK (LO);

        /* hold data for at least 50ns */
        for ( n = 5; n; n-- )
            Nop ();
    }
    
    gpio_set_dir_in_masked (ACSI_DATA_MASK);

    return ERR_DRV_NO_ERROR;
}




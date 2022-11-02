/*
 *
 * ATARI ST HARD DRIVE EMULATOR
 * Steve Bradford
 * September 2022
 *
 * ACSI-AHDI Command Set
 * ref: Atari ACSI/DMA Integration Guide - June 28, 1991
 * 
 */




/*  Atari ACSI DMA pinout, looking at the male connector at the ATARI computer:
             19 pin D-SUB
    ---------------------------------
    \ 01 02 03 04 05 06 07 08 09 10 /
     \ 11 12 13 14 15 16 17 18 19  /
       ---------------------------
Pin	Name	Description
1	D0	Data 0
2	D1	Data 1
3	D2	Data 2
4	D3	Data 3
5	D4	Data 4
6	D5	Data 5
7	D6	Data 6
8	D7	Data 7
9	_CS	Chip Select
10	IRQ	Interrupt Request
11	GND	Ground
12	/RST	Reset
13	GND	Ground
14	ACK	Acknowledge           
15	GND	Ground
16	A1	Address bit
17	GND	Ground
18	R/W	Read/Write
19	_DRQ    Data Request
*/
// ATARI Document Archive https://docs.dev-docs.org/
// in category: "Hard Drive" Application Notes on the Atari Computer System Interface (ACSI) [Sep 27 1985]
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ----- Command Phase ---------------------------------------------------
// DATA direction: From Atari to Target Device                          
// A1    ¯¯¯¯¯¯¯¯\___________________________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ LOW indicates 1st byte of new command.
// IRQ   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\___________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_____ Active low driven by target device to indicate (A) readness to accept another command byte (B) the availability of a byte to be read.
// _CS   ¯¯¯¯¯¯¯¯¯¯¯¯¯\__________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\__________/¯¯¯¯¯¯¯¯¯¯
//                     |        |                  |        |           
// R/_W  ¯¯¯¯¯¯\______________________/¯¯¯¯¯¯\____________________/¯¯¯¯¯ LOW from ATARI to Device (Write), HIGH from Device to ATARI (Read)
//               |     |        |     |      |     |        |     |     
// DATA  =======><-------VALID--------><====><-------VALID--------><==== 8bit DATA Bus
//               |     |        |     |      |     |        |     |     
//               |<-a->|<--b--->|<-c->|      |<-a->|<--b--->|<-c->|     
//                Byte 0                      Byte 1                     Byte n
// Timing
// a)  60 ns  (max)
// b)  250 ns (max)
// c)  20 ns  (max)
// IRQ Active LOW (open-collector) 1K Pullup on ATARI.
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ----- Status Phase ----------------------------------------------------
// DATA direction: From Target Device to Atari     
// A1    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ HIGH indicates inactive.
// IRQ   ______________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ Active LOW driven by target device to indicate (A) readness to accept another command byte (B) the availability of a byte to be read.
// _CS   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_______________/¯¯¯¯¯¯¯¯¯¯¯
//                     |              |            
// R/_W  _______/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_____ LOW from ATARI to Device (Write), HIGH from Device to ATARI (Read)
//              |      |              |      |     
// DATA  =====================><----VALID----><==== 8bit DATA Bus
//              |      |              |      |     
//              |<-a-->|<--b-->|<-c-->|<-d-->|     
//                              Byte 0             
// Timing
// a)  50 ns  (max)
// b)  150 ns (max)
// c)  100 ns (max)
// d)  80 ns  (max)
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ----- DMA - Data Out Phase --------------------------------------------
// DATA direction: From Atari to Target Device      
// A1   : ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ HIGH indicates inactive.
// DRQ  : ¯¯¯¯¯¯¯\_______________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯\___ Active LOW, DMA Device Data request, Used by the peripheral to request a DMA data transfer.
//                 |                                
// _ACK : ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\______________/¯¯¯¯¯¯¯¯¯¯¯
// R/_W :          |     |      ?       |     |      LOW from ATARI to Device (Write), HIGH from Device to ATARI (Read)
//                 |     |              |     |     
// DATA   ========><----------VALID-----------><==== 8bit DATA Bus
//                 |     |              |     |     
//                 |<-a->|<-----b------>|<-a->|     
//               |<--c-->|              |<--d-->|   
// Timing
// a)  60 ns  (max)
// b)  250 ns (max)
// c)  240 ns (max)
// d)  240 ns (min)
// DRQ Active LOW (open-collector) 1K Pullup on ATARI.
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ----- DMA - Data In Phase ---------------------------------------------
// DATA direction: From Target Device to Atari      
// A1   : ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ HIGH indicates inactive.
// DRQ  : ¯¯¯¯¯¯\________________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ Active LOW, DMA Device Data request, Used by the peripheral to request a DMA data transfer.
//                |                                 
// _ACK : ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\______________/¯¯¯¯¯¯¯¯¯¯¯
//                |     ||              |           
// DATA   =============><--------VALID--------><==== 8bit DATA Bus
//                |     ||              |     |     
//                |<-a->||<-----b------>|<-c->|     
// Timing
// a)  60 ns  (max)
// b)  250 ns (max)
// c)  50 ns  (min)
// DRQ Active LOW (open-collector) 1K Pullup on ATARI.



#ifndef _EMU_H
#define _EMU_H

#include "pico.h"
#include <stdbool.h>
#include "hardware/spi.h"
#include "hardware/sync.h"
#include "sd_card.h"



/* GPIO assignments by GPIO number */
#define D0                  0                   /* GPIO 00-07 bi-directional */
#define CS                  8                   /* GPIO 08 input  */
#define IRQ                 9                   /* GPIO 09 output */
#define RST                 10                  /* GPIO 10 input  */
#define ACK                 11                  /* GPIO 11 input  */
#define A1                  12                  /* GPIO 12 input  */
#define RW                  13                  /* GPIO 13 input  - read active-high, write active-low */
#define DRQ                 14                  /* GPIO 14 output */
#define DATA_BUS_CNTRL      15                  /* GPIO 15 output - data bus control - high goes tristate */
#define SPI_SDO             16                  /* GPIO 16 output - MISO */
#define MICROSD_CARD_CS0    17                  /* GPIO 17 output - microSD card adapter 1 CS, active-low */
#define SPI_CLK             18                  /* GPIO 18 output - microSD SPI clock */
#define SPI_SDI             19                  /* GPIO 19 input  - MOSI */
#define MICROSD_CARD_CS1    20                  /* GPIO 20 output - microSD card adapter 2 CS, active-low */
#define MICROSD_CARD_CS2    21                  /* GPIO 21 output - microSD card adapter 3 CS, active-low */
#define MICROSD_CARD_CS3    22                  /* GPIO 20 output - microSD card adapter 4 CS, active-low */
#define ONBOARD_LED         25                  /* GPIO 25 output - PI PICO onboard LED */
#define CONTROLLER_SELECT   26                  /* GPIO 26 input  - hardware controller ID match - high = selected */

/* ACSI BUS signals */
#define DATA_MASK           (1 << D0)           /* 0x00000000 */
#define CS_MASK             (1 << CS)           /* 0x00000100 */
#define IRQ_MASK            (1 << IRQ)          /* 0x00000200 */
#define RST_MASK            (1 << RST)          /* 0x00000400 */ 
#define ACK_MASK            (1 << ACK)          /* 0x00000800 */ 
#define A1_MASK             (1 << A1)           /* 0x00001000 */ 
#define RW_MASK             (1 << RW)           /* 0x00002000 */ 
#define DRQ_MASK            (1 << DRQ)          /* 0x00004000 */ 

/* GPIO output masks - 32 bit values */
#define SPI_CS0_MASK        (1 << MICROSD_CARD_CS0) /* 0x00020000 */
#define SPI_CS1_MASK        (1 << MICROSD_CARD_CS1) /* 0x00100000 */
#define SPI_CS2_MASK        (1 << MICROSD_CARD_CS2) /* 0x00100000 */
#define SPI_CS3_MASK        (1 << MICROSD_CARD_CS3) /* 0x00100000 */
#define SPI_CS_MASK         SPI_CS0_MASK | SPI_CS1_MASK | SPI_CS2_MASK | SPI_CS3_MASK
#define LED_MASK            (1 << ONBOARD_LED)      /* 0x02000000 */
#define DATA_BUS_CNTRL_MASK (1 << DATA_BUS_CNTRL)   /* 0x00008000 */
#define ACSI_DATA_MASK      0x000000ff          /* bits 0 - 7  ACSI bus data bits */
#define ACSI_CNTRL_MASK     0x00007f00          /* bits 8 - 14 ACSI bus control bits */
#define GPIO_MASK (uint32_t)(0x027fffff)

/* set 1 for each GPIO as an output */  
#define GPIO_OUTPUTS_MASK   ((uint32_t)(DRQ_MASK | SPI_CS_MASK | IRQ_MASK | LED_MASK | DATA_BUS_CNTRL_MASK ))



/* Emulator configuration */
#define CONTROLLER_ADDRESS  0           /* Hard Drive Emulator Controller Address ( 0 - 6, do not use 7 ) */
#define MAX_DRIVES          4           /* possible to have 7 drives (SD cards) per controller */
#define MAX_LUNS            24          /* max of 23 logical units per drive C; thru Z: */

#define LO                  0           /* signal level, 0v */
#define HI                  1           /* signal level, vcc */

#define ENABLE              0
#define DISABLE             1

#define VERSION             "1.0\0"     /* max length 6 */

#define IRQ_LO()            gpio_put (IRQ, LO);
#define IRQ_HI()            gpio_put (IRQ, HI);
#define DRQ_LO()            gpio_put (DRQ, LO);
#define DRQ_HI()            gpio_put (DRQ, HI);

extern volatile uint32_t intState;
#define enableInterrupts()  ; //(restore_interrupts (intState))
#define disableInterrupts() ; //(intState = save_and_disable_interrupts ())

#define dataBus(s)          ; //gpio_put (DATA_BUS_CNTRL, s)
#define newCMD()            gpio_get (A1) == LO && gpio_get (RW) == LO // future will be gpio_get (CONTROLLER_SELECT) == HI
#define rdDataBus()         (gpio_get_all () & ACSI_DATA_MASK)
#define wrDataBus(d)        gpio_put_masked ( ACSI_DATA_MASK, (d) )
#define checkRST()          {                               \
                                if ( gpio_get (RST) == LO ) \
                                    gotRST = true;          \
                            }
#define checkCS()           while ( gpio_get ( CS ) == LO )
#define waitCS()            while ( gpio_get ( CS ) == HI )
#define waitRW(s)           while ( gpio_get ( RW ) == s )
#define waitACK(s)          while ( gpio_get ( ACK ) == s )
#define waitRD()            while ( gpio_get ( RW ) == LO )
#define waitWR()            while ( gpio_get ( RW ) == HI )
#define waitA1()            while ( gpio_get ( A1 ) == LO )
#define waitRST()           while ( gpio_get ( RST ) == LO )

#define usDelay(x)          {                                           \
                                uint64_t tick = time_us_64 () + (x);    \
                                while ( time_us_64 () < tick );         \
                            }

#define Nop()               asm volatile ("nop\n")

#define FREAD               true
#define FWRITE              false


/* build options */
#define WR_ENABLE           1           /* enable ACSI writes */
#define DEBUG               1           /* enable debug stuff */




/* 
 * disk drive structures 
 *
 * drives are the sd-cards
 * luns are the partitions
 * e.g. 
 * drive 0, partition 3 = sdcard 0, lun 2
 * drive 1, partition 1 = sdcard 1, lun 0
 * 
 */

typedef struct LUN_INFO {
    
    bool     mounted;                   /* true = lun (partition) mounted */
    uint32_t sectorSize;                /* how big is a sector (bytes) */
    uint32_t startSector;               /* starting sector number for this logical drive (partition) */
    uint32_t endSector;                 /* ending sector number for this logical drive (partition) */
    uint32_t sectorCount;               /* Total sectors for this logical drive (partition) */
    uint8_t  lun;
    
} LUNS;

typedef struct DRIVE_INFO {

    uint8_t  status;                    /* drive current status */
    uint8_t  lastStatus;
    bool     mounted;                   /* true = drive mounted */
    bool     raw;                       /* true = a partioned micro-sd card, false = FAT32 *.img files */
    bool     writable;                  /* false = read-only */
    uint32_t lastError;                 /* SCSI 24 bit error code */
    uint32_t lba;                       /* logical block address passed in by the command */
    uint32_t len;                       /* data length for extended SCSI commands */
    LUNS     luns      [MAX_LUNS];      /* Logical Unit Number (max 24 partitions C: - Z:) */

    FATFS    fs;                        /* FAT file system work space */
    FIL      fp;                        /* FAT file pointer work space */
    //DIR      dp;                        /* FAT directory pointer work space */

    char     volume    [10];            /* drive volume name - eg. "sd0, sd1" */
    char     volLabel  [12];            /* volume label - don't care what this is */
    uint32_t volSerial;                 /* volume serial number - unique for each micro-sd card */
    uint8_t  partTotal;

    sd_card_t *pSD;

} DRIVES;


extern int      mountRAW        ( void );
extern int      mountFS         ( void );
extern bool     sd_init_driver  ();
extern int      sd_init_card    ( sd_card_t *);
extern int      sd_write_blocks ( sd_card_t *, const uint8_t *, uint64_t, uint32_t);
extern int      sd_read_blocks  ( sd_card_t *, uint8_t *, uint64_t, uint32_t);
extern bool     sd_card_detect  ( sd_card_t *);
extern uint64_t sd_sectors      ( sd_card_t *);
extern void     modeSense0      ( char *, DRIVES *, int );
extern void     modeSense4      ( char *, DRIVES *, int );

#endif
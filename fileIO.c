/*
 * ATARI ST HDC Emulator
 * 
 * File:    fileio.c
 * Author:  Steve Bradford
 * Created: September 2022
 *
 * PICO hardware initialisation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include "ff.h"
#include "diskio.h"
#include "emu.h"
#include "emuerror.h"
#include "emucmd.h"


extern uint8_t DMAbuffer[];

extern uint8_t rdDMA ( uint8_t *, uint32_t );
extern uint8_t wrDMA ( uint8_t *, uint32_t );



static inline int __not_in_flash_func (rawRD) ( CommandDescriptorBlock *cdb, DRIVES *drv )
{
#ifdef TODO
    if ( drv->lba == 0 && cdb->len == 1 ) {

        a2stmPtr = nosdcard_boot_bin; //drvboot_tools_bin; //bootSector
        a2stmLen = nosdcard_boot_bin_len; //512; //nosdcard_boot_bin_len;
        bool doCheckSum = true;
        // ORG of acsi2stm_boot_bin relative to the buffer
        int bootOrg = 0x60;

        memcpy ( DMAbuffer, a2stmPtr, a2stmLen );

        // Overlay bra.b instruction to boot org
        //DMAbuffer[0x00] = 0x60;
        //DMAbuffer[0x01] = bootOrg - 2;
        // Overlay alloc sizes
        //memcpy(&DMAbuffer[bootOrg - 4], &a2stdrv_boot_bin[4], 4);

        // Overlay boot loader
        //memcpy(&DMAbuffer[bootOrg], overlay_boot_bin, overlay_boot_bin_len);



        if ( doCheckSum ) {

            DMAbuffer [bootOrg - 6] = DMAbuffer [(bootOrg - 6) + 1] = 0;

            uint16_t checksum = 0;
            for ( int i = 0; i < 512; i += 2) {
                checksum += ((int) DMAbuffer [i] << 8) + (DMAbuffer [i+1] );
            }

            checksum = 0x1234 - checksum;

            DMAbuffer [bootOrg - 6] = (checksum >> 8) & 0xff;
            DMAbuffer [(bootOrg - 6) + 1] = checksum & 0xff;
        }

        wrDMA ( DMAbuffer, 512 );
    }
#endif
    
    int e;
    int length = cdb->len;


    if ( length == 0 )                          /* is this a ACSI read or SCSI read ? */
        length = drv->len;

    enableInterrupts ();

    if ( ( e = sd_read_blocks ( drv->pSD, DMAbuffer, drv->lba + drv->offset, length )) != SD_BLOCK_DEVICE_ERROR_NONE )
    {
        drv->lastError = SCSI_ERR_READ;
        drv->status    = ERR_CNTRL_DATA_NOT_FOUND;

        printf ( "READ: read failed %d, len = %d\n", e, length );
    }

    else 
    {
        disableInterrupts ();

        wrDMA ( DMAbuffer, length << 9 );
    }

    disableInterrupts ();
}


static inline int __not_in_flash_func (rawWR) ( CommandDescriptorBlock *cdb, DRIVES *drv )
{
    int e;
    int length = cdb->len;


    if ( length == 0 )                          /* is this a ACSI write or SCSI write ? */
        length = drv->len;
    
    disableInterrupts ();
                
    rdDMA ( DMAbuffer, length * 512 );
    
    enableInterrupts ();
    
#if WR_ENABLE
    if ( ( e = sd_write_blocks ( drv->pSD, DMAbuffer, drv->lba + drv->offset, length )) != SD_BLOCK_DEVICE_ERROR_NONE )
    {
        drv->lastError = SCSI_ERR_WRITE;
        drv->status    = ERR_CNTRL_DATA_NOT_FOUND;

        printf ( "WRITE: write failed %d, len = %d\n", e, length );
    }
#endif
    disableInterrupts ();
}


static inline void __not_in_flash_func (fileRD) ( CommandDescriptorBlock *cdb, DRIVES *drv )
{
    int e;
    UINT n;
    uint32_t tLength = cdb->len << 9;
    
    
    enableInterrupts ();
   
    if ( ( e = f_lseek ( &drv->fp, drv->lba * 512 ) ) != FR_OK )
    {
        drv->lastError = SCSI_ERR_READ;
        drv->status    = ERR_CNTRL_SEEK_ERROR;

        printf ( "READ: seek failed 0x%08x, error %d\n", drv->lba * 512, e );
    }

    else
    {
        if ( ( e = f_read ( &drv->fp, DMAbuffer, tLength, &n )) != FR_OK )
        {
            drv->lastError = SCSI_ERR_READ;
            drv->status    = ERR_CNTRL_DATA_NOT_FOUND;

            printf ( "READ: read failed %d\n ", e );
        }

        else 
        {
            disableInterrupts ();

            wrDMA ( DMAbuffer, tLength );
        }
    }
    
    disableInterrupts ();
}


static inline void __not_in_flash_func (fileWR) ( CommandDescriptorBlock *cdb, DRIVES *drv )
{
    int e;
    UINT n;
    uint32_t tLength = cdb->len << 9;
    
    
    disableInterrupts ();
                   
    rdDMA ( DMAbuffer, tLength );
    
    enableInterrupts ();
    
    if ( ( e = f_lseek ( &drv->fp, drv->lba * 512 ) ) != FR_OK )
    {
        drv->lastError = SCSI_ERR_WRITE;
        drv->status    = ERR_CNTRL_SEEK_ERROR;

        printf ( "WRITE: seek failed 0x%08x, error %d\n", drv->lba * 512, e );
    }
#if WR_ENABLE
    else
    {
        if ( ( e = f_write ( &drv->fp, DMAbuffer, tLength, &n )) != FR_OK )
        {
            drv->lastError = SCSI_ERR_WRITE;
            drv->status    = ERR_CNTRL_DATA_NOT_FOUND;

            printf ( "WRITE: write failed %d\n ", e );
        }
    }
#endif
    disableInterrupts ();
}



void __not_in_flash_func (fileIO) ( bool IOdirection, CommandDescriptorBlock *cdb, DRIVES *drv )
{
    
    switch ( IOdirection )
    {
        case FREAD:
            if ( drv->raw )
                rawRD ( cdb, drv );
           
            else
                fileRD ( cdb, drv );
      
            break;
        
        case FWRITE:
            if ( drv->raw )
                rawWR ( cdb, drv );
          
            else
                fileWR ( cdb, drv );
        
            break;
    }
    
}
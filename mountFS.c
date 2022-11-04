/*
 * ATARI ST HDC Emulator
 * 
 * File:    mountfs.c
 * Author:  Steve Bradford
 * Created: September 2022
 *
 * PICO hardware initialisation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ff.h"
#include "emu.h"



#define HDC_ReadInt32(a, i) (((unsigned) a[i] << 24) | ((unsigned) a[i + 1] << 16) | ((unsigned) a[i + 2] << 8) | a[i + 3])

extern DRIVES drv[];
extern sd_card_t *sd_get_by_num (size_t);



int HDC_PartitionCount ( DRIVES *pdrv )
{
	unsigned char *pinfo, bootsector[512];
	uint32_t start, sectors, total = 0;
	int i, parts = 0;

    FIL *fp = &pdrv->fp;
    UINT n;
    
    
	if (!fp)
		return 0;

	if ( (i = f_lseek ( fp, 0 ) ) != FR_OK ) 
    {
        printf( "HDC_PartitionCount: seek error %d\n", i );
        
		return 0;
	}
    
	if ( ( i = f_read ( fp, bootsector, sizeof (bootsector), &n ) ) != FR_OK )
	{
		printf( "HDC_PartitionCount: read error %d\n", i );
    
		return 0;
	}
    
    /* read MBR partition table
     * this is in the first sector (512 bytes) of the volume
     * if byte 0 is not equal to 0 then a boot programme resides
     * partition info starts at the following offsets, each is 16 bytes long
     * partition 1 at byte 446 (0x01be)
     * partition 2 at byte 462
     * partition 3 at byte 478
     * partition 4 at byte 494
     * byte offset 510 (0x01fe) should read 0xaa55 = valid MBR
     * 
     */
	if ( bootsector [0x01fe] == 0x55 && bootsector [0x01ff] == 0xaa )
	{
		int ptype, boot;
        //uint8_t *pbpb = bootsector + 0x0b;            /* pointer to bios parameter block */


        pdrv->luns [parts].sectorSize = (uint32_t)(bootsector [0x0b] << 8) | bootsector [0x0c];
        //printf ( "sector size is %d\n", pdrv->luns [parts].sectorSize );
        //printf ( "sectors per cluster is %d\n", bootsector [0x0d] );

		printf ( "DOS MBR:\n" );
        
		/* first partition table entry */
		pinfo = bootsector + 0x1be;
      
		for ( i = 0; i < MAX_DRIVES; i++, pinfo += 16 )
		{
			boot    = pinfo [0];                                                /* uint8_t - 0x00 = not bootable, 0x80 = bootable */
			ptype   = pinfo [4];                                                /* uint8_t - partition type
                                                                                 *          0x00: Blank entry. Any other field must be zero.
                                                                                 *          0x01: FAT12 (CHS/LBA, < 65536 sectors)
                                                                                 *          0x04: FAT16 (CHS/LBA, < 65536 sectors)
                                                                                 *          0x05: Extended partition (CHS/LBA)
                                                                                 *          0x06: FAT12/16 (CHS/LBA, >= 65536 sectors)
                                                                                 *          0x07: HPFS/NTFS/exFAT (CHS/LBA)
                                                                                 *          0x0B: FAT32 (CHS/LBA)
                                                                                 *          0x0C: FAT32 (LBA)
                                                                                 *          0x0E: FAT12/16 (LBA)
                                                                                 *          0x0F: Extended partition (LBA)
                                                                                 */
            
            start = ((uint32_t)pinfo [11] << 24) | ((uint32_t)pinfo [10] << 16) | ((uint32_t)pinfo [9] << 8) | pinfo [8]; //*(uint32_t*)(pinfo+8);                /* uint32_t - partition start sector in 32bit LBA format */
			sectors = ((uint32_t)pinfo [15] << 24) | ((uint32_t)pinfo [14] << 16) | ((uint32_t)pinfo [13] << 8) | pinfo [12]; //*(uint32_t*)(pinfo+12);             /* uint32_t - partition size in sectors */
			
            total += sectors;
			
            if ( ptype && (ptype != 0x05 && ptype != 0x0f) )
            {
                printf ( "- Partition %d: type=0x%02x, start=0x%08x, size=%.1f MB %s%s\n",
                    parts, ptype, start, sectors/2048.0, boot ? "(boot)" : "", sectors ? "" : "(invalid)");
                
                pdrv->luns [parts].startSector = start;
                pdrv->luns [parts].sectorCount = sectors;
                pdrv->luns [parts].lun         = parts;
                pdrv->luns [parts].mounted     = true;
                
				parts++;
            }
            
            /* check extended boot records (if any) */
            if ( ptype == 0x0f )
            {
                uint8_t extpart [512];
                char    *ep = (extpart + 0x1be);
                uint8_t epboot, eptype;
                uint32_t epstart, epsectors;
                
                if ( (i = f_lseek ( fp, start * 512 ) ) != FR_OK )
                {
                    printf ( "2nd seek failed %d\n", i );
                    return 0;
                }
                if ( ( i = f_read ( fp, extpart, sizeof (extpart), &n ) ) != FR_OK )
                {
                    printf ( "2nd read failed %d\n", i );
                    return 0;
                }
                
                if ( extpart [0x01fe] == 0x55 && extpart [0x01ff] == 0xaa )
                {
                    printf ( "DOS EBR:\n");
                  
                    for ( int p = 0; p < 4; p++, ep += 16 )
                    {
                        epboot    = ep [0];
                        eptype    = ep [4];
                        epstart = (((uint32_t)ep [11] << 24) | ((uint32_t)ep [10] << 16) | ((uint32_t)ep [9] << 8) | ep [8]) + start; //*(uint32_t*)(pinfo+8);                /* uint32_t - partition start sector in 32bit LBA format */
                        epsectors = ((uint32_t)ep [15] << 24) | ((uint32_t)ep [14] << 16) | ((uint32_t)ep [13] << 8) | ep [12];

                        if ( eptype )
                        {
                            printf ( "- Extended Partition %d: type=0x%02x, start=0x%08x, size=%.1f MB %s%s\n",
                                parts, eptype, epstart, epsectors/2048.0, epboot ? "(boot)" : "", epsectors ? "" : "(invalid)");

                            pdrv->luns [parts].startSector = epstart;
                            pdrv->luns [parts].sectorCount = epsectors;
                            pdrv->luns [parts].lun         = parts;
                            pdrv->luns [parts].mounted     = true;
                            
                            parts++;
                        }
                    }
                }
            }
		}
        
		printf( "Total size: %.1f MB in %d partitions\n", total/2048.0, parts);
	}
//#ifdef DOINEED   
	else
	{
		/* Partition table contains hd size + 4 partition entries
		 * (composed of flag byte, 3 char ID, start offset
		 * and size), this is followed by bad sector list +
		 * count and the root sector checksum. Before this
		 * there's the boot code.
		 */
		char c, pid[4];
		int j, flags;
		bool extended;

		printf( "ATARI MBR:\n");
		pinfo = bootsector + 0x1C6;
        
		for (i = 0; i < 4; i++, pinfo += 12)
		{
			flags = pinfo[0];
			
			if (flags & 0x1)
            {
                for (j = 0; j < 3; j++)
                {
                    c = pinfo[j+1];
                    if (c < 32 || c >= 127)
                        c = '.';
                    pid[j] = c;
                }
                
                pid[3] = '\0';
                extended = strcmp("XGM", pid) == 0;
                pdrv->luns [parts].startSector = HDC_ReadInt32(pinfo, 4);
                pdrv->luns [parts].sectorCount = HDC_ReadInt32(pinfo, 8);
                pdrv->luns [parts].lun         = parts;
                pdrv->luns [parts].mounted     = true;
                    
                printf(
                    "- Partition %d: ID=%s, start=0x%08x, size=%.1f MB, flags=0x%x %s%s\n",
                    i, pid, pdrv->luns [parts].startSector, pdrv->luns [parts].sectorCount/2048.0, flags,
                    (flags & 0x80) ? "(boot)": "",
                    extended ? "(extended)" : "");
                
				parts++;
            }
		}
        
		total = HDC_ReadInt32(bootsector, 0x1C2);
		printf( "- Total size: %.1f MB in %d partitions\n", total/2048.0, parts);
	}
//#endif
    
    drv [0].raw     = false;
    
	return parts;
}


/* 
 * volume is the micro-sd card which we will refer to as the drive
 * the drive can be a formatted, multi-partioned micro-sd card or 
 * a image file
 * lun is a partition on the volume (drive) 
 * a max of 4 luns is allowed (0-3)
 * 
 */

int mountFS ( void )
{
    int n;
    char imageName [16];
    char *volume   = "sd";
    char *filename [] = {"hd0.img", "hd1.img", "hd2.img", "hd3.img"};
    
    
    /* initialise all online micro-sd cards */
    for ( int d = 0; d < MAX_DRIVES; d++ ) 
    {
        sprintf ( drv [d].volume, "%s%d:", volume, d );

        drv [d].pSD = sd_get_by_num(d);
       
        if ( drv [d].pSD->card_detect_gpio )    /* check micro-sd card is inserted */
        {
            /* NOTE this will return error 13 if a partitioned SD card is installed 
             * because there isn't a FAT filesystem to read */
            if ( ( n = f_mount ( &drv [d].pSD->fatfs, drv [d].pSD->pcName, 1 ) ) == FR_OK ) 
            {   
                /* should be "sd0:/hd0.img" */
                sprintf ( imageName, "%s/%s", drv [d].volume, filename [d] );  
                
                if ( ( n = f_open ( &drv [d].fp, imageName, FA_READ | FA_WRITE ) ) == FR_OK )
                {
                    drv [d].raw     = false;
                    drv [d].mounted = true;

                    f_getlabel ( drv [d].volume, drv [d].volLabel, &drv [d].volSerial );

                    printf ( "\nVolume mounted: %s - label = %s - serial = 0x%08x\n",
                        drv [d].volume, drv [d].volLabel, drv [d].volSerial );
                    
                    drv [d].partTotal = HDC_PartitionCount ( &drv [d]);                    
                }
#if DEBUG                
                else
                {
                    printf ( "\nmount %s open failed, error %d\n", imageName, n );
                }
#endif
            } 
#if DEBUG
            else
            {
                printf ( "\nmount %s failed, error %d\n", drv [d].pSD->pcName, n );
            }
#endif
        }
    }
    
    return drv [0].partTotal + drv [1].partTotal;
}
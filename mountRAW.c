
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include "ff.h"
#include "sd_card.h"
#include "emu.h"


#define HDC_ReadInt32(a, i) (((unsigned) a[i] << 24) | ((unsigned) a[i + 1] << 16) | ((unsigned) a[i + 2] << 8) | a[i + 3])



extern DRIVES drv[];


int rawPartitionCount ( void )
{
	unsigned char *pinfo, bootsector[512];
	uint32_t start, sectors, total = 0;
	int i, parts = 0;
    int lba = 0;
    
	//if ( ( i = mydisk_read ( 0, bootsector, lba, 1 ) ) != RES_OK )
    if ( ( i = sd_read_blocks ( drv [0].pSD, bootsector, lba, 1 ) ) != SD_BLOCK_DEVICE_ERROR_NONE )
	{
		printf( "rawPartitionCount: read error %d\n", i );
        
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
                    i, ptype, start, sectors/2048.0, boot ? "(boot)" : "", sectors ? "" : "(invalid)");
                
                drv [0].luns [parts].startSector = start;
                drv [0].luns [parts].sectorCount = sectors;
                drv [0].luns [parts].lun         = parts;
                drv [0].luns [parts].mounted     = true;
                
				parts++;
            }
            
            /* check extended boot records (if any) */
            if ( ptype == 0x0f || ptype == 0x05 )
            {
                uint8_t extpart [512];
                char    *ep = (extpart + 0x1be);
                uint8_t epboot, eptype;
                uint32_t epstart, epsectors;
                
                
                //if ( ( i = mydisk_read ( 0, extpart, start, 1 ) ) != RES_OK )
                if ( ( i = sd_read_blocks ( drv [0].pSD, extpart, start, 1 ) ) != SD_BLOCK_DEVICE_ERROR_NONE )
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

                            drv [0].luns [parts].startSector = epstart;
                            drv [0].luns [parts].sectorCount = epsectors;
                            drv [0].luns [parts].lun         = parts;
                            drv [0].luns [parts].mounted     = true;
                            
                            parts++;
                        }
                    }
                }
            }
		}
        
		printf( "Total size: %.1f MB in %d partitions\n", total/2048.0, parts);
	}

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
                drv [0].luns [parts].startSector = HDC_ReadInt32(pinfo, 4);
                drv [0].luns [parts].sectorCount = HDC_ReadInt32(pinfo, 8);
                drv [0].luns [parts].lun         = parts;
                drv [0].luns [parts].mounted     = true;
                    
                printf(
                    "- Partition %d: ID=%s, start=0x%08x, size=%.1f MB, flags=0x%x %s%s\n",
                    parts, pid, drv [0].luns [parts].startSector, drv [0].luns [parts].sectorCount/2048.0, flags,
                    (flags & 0x80) ? "(boot)": "",
                    extended ? "(extended)" : "");
                
				parts++;
            }
		}
        
		total = HDC_ReadInt32(bootsector, 0x1C2);
		printf( "- Total size: %.1f MB in %d partitions\n", total/2048.0, parts);
	}
}
    

extern sd_card_t sd_cards[];

int mountRAW ( void )
{
    int e;

    drv [0].raw     = false;
    drv [0].mounted = false;
        
    if ( sd_init_driver () )
    {
        drv [0].pSD = &sd_cards [0];
        if ( sd_init_card ( drv [0].pSD ) == 0 )
        //e = mydisk_initialize ( 0 );
        //printf ( "disk_init = 0x%02x\n", e );
        //if ( e == 0 )
        {
    //if ( SDCARD_CD )                            /* check micro-sd card is inserted */
    //{
        //if ( disk_initialize ( 0 ) & STA_NOINIT != STA_NOINIT )
       // {
            //DSTATUS stat = disk_initialize ( 0 );
            //printf ( "mountRAW: stat = 0x%0x\n", stat );
            drv [0].raw     = true; /* raw  partioned */
            drv [0].mounted = true; /* sd card is mounted */

            return rawPartitionCount ();
        }        
    }
    
    return 0;
}


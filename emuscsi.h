/*
 * ATARI ST HDC Emulator
 * 
 * File:    emuscsi.h
 * Author:  Steve Bradford
 * Created: September 2022
 *
 * 
 */

#ifndef _EMU_SCSI_H
#define _EMU_SCSI_H

//#include "emu.h"
//extern DRIVES []

//extern void modeSense0 ( char *, DRIVES *, int );
//extern void modeSense4 ( char *, DRIVES *, int );

typedef union
{
    struct
    {
        uint8_t dummy;
        uint8_t SK;
        uint8_t ASC;
        uint8_t ASCQ;
    };
    uint32_t status;

} scsiStatus_t;



/* define some generic error codes using above */
/* 24 bits (3 bytes) comprising SK, ASC, ASCQ */
  #define SCSI_ERR_OK               0x000000
  #define SCSI_ERR_READ             ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_UNRECOVERED_READ_ERROR << 8)  | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )
  #define SCSI_ERR_WRITE            ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_WRITE_ERROR << 8)             | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )      
  #define SCSI_ERR_WRITE_PROT       ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_WRITE_PROTECTED << 8)         | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )      
  #define SCSI_ERR_OPCODE           ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_INVALID_COMMAND_OP_CODE << 8) | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )
  #define SCSI_ERR_INV_ADDR         ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_LBA_OUT_OF_RANGE << 8)        | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )    
  #define SCSI_ERR_INV_ARG          ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_INVALID_COMMAND_OP_CODE << 8) | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )
  #define SCSI_ERR_INV_LUN          ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_LUN_NOT_SUPPORTED << 8)       | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )     
  #define SCSI_ERR_MEDIUM_CHANGE    ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_NOT_READY_MEDIUM_CHANGE << 8) | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )
  #define SCSI_ERR_NO_MEDIUM        ( ((uint32_t)SCSI_SK_UNIT_ATTENTION << 16) | ((uint32_t)SCSI_ASC_MEDIUM_NOT_PRESENT << 8)      | SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION )     
      

#endif

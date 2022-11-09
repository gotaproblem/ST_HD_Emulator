/*
 * ATARI ST HDC Emulator
 * 
 * File:    emucmd.h
 * Author:  Steve Bradford
 * Created: September 2022
 *
 * ACSI-AHDI Command Set
 * ref: Atari ACSI/DMA Integration Guide - June 28, 1991
 */

#ifndef _ACSI_CMD_H
#define _ACSI_CMD_H

/* ref. SPC-5 table F.2 Operation Codes */
/* ATARI ACSI commands with some others - 6 bytes */
enum ACSI_COMMANDS {
    
    TEST_UNIT_READY = 0x00,
    REQUEST_SENSE   = 0x03,
    FORMAT_UNIT     = 0x04,
    CMD_READ        = 0x08,
    CMD_WRITE       = 0x0a,
    CMD_SEEK        = 0x0b,

    VENDOR_SPECIFIC = 0x0d,                     /* see this occasionally from the PPutnik driver - don't know what it does */

    CMD_INQUIRY     = 0x12,
    MODE_SELECT     = 0x15,
    MODE_SENSE      = 0x1a,

    SEND_DIAGNOSTIC = 0x1d,                     /* listed as mandatory, but is it ever used? */

    MEDIA_REMOVE    = 0x1e,
    EXTENDED_CMD    = 0x1f
            
};

/* SCSI commands - typically 10 bytes */
enum SCSI_OP_CODES {

    SCSI_OP_READ_CAPACITY   = 0x25,             /* 10 bytes */
    SCSI_OP_READ            = 0x28,
    SCSI_OP_WRITE           = 0x2a,
    SCSI_OP_WRITE_BUFFER    = 0x3b,
    SCSI_OP_READ_BUFFER     = 0x3c,
    SCSI_OP_MODE_SELECT     = 0x55,             /* 10 bytes */
    SCSI_OP_MODE_SENSE      = 0x5a,             /* 10 bytes */
    SCSI_OP_EXTENDED_CDB    = 0x7e,
    SCSI_OP_VARIABLE_CDB    = 0x7f,
    SCSI_OP_READ16          = 0x88,             /* 16 bytes */
    SCSI_OP_WRITE16         = 0x8a,
    SCSI_OP_REPORT_LUNS     = 0xa0,
    SCSI_OP_READ12          = 0xa8,             /* 12 bytes */
    SCSI_OP_WRITE12         = 0xaa

};

/* Additional operation codes */
enum SCSI_ADDITIONAL_OP_CODES {

    SCSI_ADD_OP_RECEIVE_DIAGNOSTIC_RESULTS  = 0x1c,
    SCSI_ADD_OP_SEND_DIAGNOSTIC
    
};


#define CONTROLLER_MASK     0xe0
#define OPCODE_MASK         0x1f


typedef union {

    struct {
        
        union {
            
            struct {
                
                uint8_t opcode : 5;             /* command byte - opcode 0-0x1f */
                uint8_t target : 3;             /* command byte - controller ID */
                
            };
            uint8_t cmd;                        /* opcode and controller ID */
            
        } DEVICE;                        
        
        uint8_t msb;                            /* LBA - most-significant */
        uint8_t mid;
        uint8_t lsb;                            /* LBA - least significant */
        uint8_t len;                            /* operation length (usually sector count in 512 byte increments) */
        uint8_t mod;                            /* operation modifiers in bits 6 and 7*/
        uint8_t extra [11];                     /* extra command bytes for extended commands */
        uint8_t cmdLength;                      /* length of the command packet */

    };
    uint8_t b [18];

} CommandDescriptorBlock;

#endif
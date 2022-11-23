/*
 * ATARI ST HDC Emulator
 * 
 * File:    emucmd.h
 * Author:  Steve Bradford
 * Created: September 2022
 *
 * Error Codes and Descriptions
 * ref: Atari ACSI/DMA Integration Guide - June 28, 1991
 * 
 */

#ifndef _ACSI_ERRORS_H
#define _ACSI_ERRORS_H

#define CHECK_CONDITION             0x02    // Check Condition Bit

/* Drive Errors */
#define ERR_DRV_NO_ERROR            0x00    // No error
#define ERR_DRV_NO_INDEX            0x01    // No index
#define ERR_DRV_WR_FAULT            0x03    // Write fault
#define ERR_DRV_DRV_NOT_READY       0x04    // Drive not ready
#define ERR_DRV_NO_TRACK            0x06    // No track 00

/* Controller Errors */
#define ERR_CNTRL_IDC_ERROR         0x10    // ID ECC error
#define ERR_CNTRL_DATA_ERROR        0x11    // Uncorrectable data error
#define ERR_CNTRL_ID_NOT_FOUND      0x12    // ID address mark not found
#define ERR_CNTRL_DATA_NOT_FOUND    0x13    // Data address mark not found
#define ERR_CNTRL_REC_NOT_FOUND     0x14    // Record not found
#define ERR_CNTRL_SEEK_ERROR        0x15    // Seek error
#define ERR_CNTRL_DATA_CHECK        0x18    // Data check in no abort mode
#define ERR_CNTRL_ECC_ERROR         0x19    // ECC error during verify
#define ERR_CNTRL_BAD_BLOCK         0x1a    // Bad block
#define ERR_CNTRL_BAD_FORMAT        0x1c    // Unformatted or bad format

/* Command Errors */
#define ERR_CMD_INVALID_CMD         0x20    // Invalid command
#define ERR_CMD_INVALID_ADD         0x21    // Invalid address
#define ERR_CMD_VOL_OVERFLOW        0x23    // Volume overflow
#define ERR_CMD_INVALID_ARG         0x24    // Invalid argument
#define ERR_CMD_INVALID_DRV         0x25    // Invalid drive number
#define ERR_CMD_ZERO_PARITY         0x26    // Byte zero parity check

/* Miscellaneous Errors */
#define ERR_MISC_CONTROLLER_FAIL    0x30    // Controller self test failed

/* ------------------------------------------------------------------------- */
/* ref. SCSI Primary Commands - 5 (SPC-5) */
/* https://standards.incits.org/apps/group_public/download.php/109877/eb-2019-00306-Public-Review-Register-INCITS-502-201x-Comments-Due-08-19-2019.pdf */

/* SCSI Sense Key */
enum SCSI_SK {

  SCSI_SK_NO_SENSE                  = 0x00,
  SCSI_SK_RECOVERED_ERROR,
  SCSI_SK_NOT_READY,
  SCSI_SK_MEDIUM_ERROR,
  SCSI_SK_HARDWARE_ERROR,
  SCSI_SK_ILLEGAL_REQUEST,
  SCSI_SK_UNIT_ATTENTION,
  SCSI_SK_DATA_PROTECT,
  SCSI_SK_BLANK_CHECK,
  SCSI_SK_VENDOR_SPECIFIC,
  SCSI_SK_COPY_ABORTED,
  SCSI_SK_ABORTED_COMMAND,
  SCSI_SK_VOLUME_OVERFLOW           = 0x0d,
  SCSI_SK_MISCOMPARE,
  SCSI_SK_COMPLETED

};

/* SCSI Additional Sense Code - ref. SPI-5 F.2 */
/* the following are for Direct Access Block Devices only */
enum SCSI_ASC {

  SCSI_ASC_NO_ADDITIONAL_SENSE      = 0x00,
  SCSI_ASC_PERIPHERAL_WRITE_FAULT   = 0x03,
  SCSI_ASC_LUN_NOT_READY            = 0x04,
  SCSI_ASC_LUN_NOT_RESPONDING       = 0x05,
  SCSI_ASC_LUN_COMMS_FAILURE        = 0x08,
  SCSI_ASC_WARNING                  = 0x0b,
  SCSI_ASC_WRITE_ERROR              = 0x0c,
  SCSI_ASC_UNRECOVERED_READ_ERROR   = 0x11,
  SCSI_ASC_INVALID_COMMAND_OP_CODE  = 0x20,
  SCSI_ASC_LBA_OUT_OF_RANGE         = 0x21,
  SCSI_ASC_INVALID_FIELD_IN_CDB     = 0x24,
  SCSI_ASC_LUN_NOT_SUPPORTED        = 0x25,
  SCSI_ASC_WRITE_PROTECTED          = 0x27,
  SCSI_ASC_NOT_READY_MEDIUM_CHANGE  = 0x28,
  SCSI_ASC_POWER_ON_RESET           = 0x29,
  SCSI_ASC_OPERATION_TIMEOUT        = 0x2e,
  SCSI_ASC_MEDIUM_NOT_PRESENT       = 0x3a,
  SCSI_ASC_SELF_TEST_FAILURE        = 0x40

  /* > 0x7f Vendor Specific */

};

/* SCSI Additional Sense Code Qualifier */
/* so mamy combinations with _ASC, just use 0x00 I think */
enum SCSI_ASCQ {

  SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION = 0x00,
  SCSI_ASCQ_IO_PROCESS_TERMINATED           = 0x06,
  SCSI_ASCQ_OPERATION_IN_PROGRESS           = 0x16

};
    
#endif
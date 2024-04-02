#ifndef _PARSER_HEX_H_
#define _PARSER_HEX_H_

/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "File_handle.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
#define MAX_BUFFER_LENGTH    50   /* maximum buffer length */

#define DATA_OFFSET          4    /* data field offset in buffer */

/* @ref record line structure */
typedef struct
{
    uint8_t recordLength;  /* 1 byte record (contain amount of data bytes) */
    uint8_t address[2];    /* 2 bytes address */
    uint8_t recordType;    /* 1 byte record type */
    uint8_t data[30];      /* data field */
    uint8_t checksum;      /* 1 byte checksum */
}hexRecordTypedef;

/* @ref State of record line (only use for state machine) */
typedef enum
{
    SIZE       = 0, /* check size */
    START_CODE = 1, /* check start code */
    CHECK_SUM  = 2  /* check checksum */
}STATE;

/* @ref State of record file (only use for state machine) */
typedef enum
{
    PARSER_ERROR  = 0, /* check error */
    PARSER_EOF    = 1, /* check EOF */
    PARSER_VALID  = 2  /* Valid */
}RECORD_FILE_STATE;

/* @ref Check state */
typedef enum
{
    CHECKING_ERROR  = 0, /* error */
    CHECKING_VALID  = 1, /* valid */
}CHECKING_STATE;

/* @ref Record line state */
typedef enum
{
    LINE_START_ERROR     = 0, /* start code error */
    LINE_SIZE_ERROR      = 1, /* wrong size */
    LINE_CHECKSUM_ERROR  = 2, /* checksum error */
    LINE_VALID           = 3  /* line valid */
}RECORDLINE_STATE;

/* @ref Check file state */
typedef enum
{
    FILE_START_ERROR     = 0, /* start code error occur in the file */
    FILE_SIZE_ERROR      = 1, /* wrong size error occur in the file */
    FILE_CHECKSUM_ERROR  = 2, /* checksum error occur in the file */
    FILE_EOF_ERROR       = 3, /* end of file error occur in the file */
    FILE_VALID           = 4, /* all of record is valid */
    FILE_RF_ERROR        = 5  /* read file error */
}FILE_CHECKING_STATE;

/* @ref record type definition */
typedef enum
{
    DATA       = 0x00, /* data record type */
    EX_SEGMENT = 0x02, /* extended segment address record type */
    EX_LINEAR  = 0x04  /* extended linear address record type */
}RECORDTYPE_STATE;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
  * @brief  Check record file is valid or no
  * @param  filename: name of file
  * @retval File checking state (Can be a value of @ref FILE_CHECKING_STATE)
  */
extern FILE_CHECKING_STATE checkRecordFile(char *filename);

/**
  * @brief  Print parser file with 2 fields absolute address and corresponding data
  * @param  filename: name of file that user want to print
  * @retval None
  */
extern void printfHexRecord(char *filename);

#endif /* _PARSER_HEX_H_ */


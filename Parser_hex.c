#include "Parser_hex.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
 
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 
/*******************************************************************************
 * Variables
 ******************************************************************************/
static hexRecordTypedef hexRecordStructure;
static char buffer[MAX_BUFFER_LENGTH];

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief  Get record file and save it in struct
  * @param  *file: string buffer of record line
  * @param  length: length of buffer
  * @retval None
  */
void getLineRecord(char *file, uint8_t length)
{
    static uint8_t value  = 0;   /* Variable contain converted value */
    uint8_t index         = 0;
    int8_t buf_index      = 0;
    int8_t buf_length     = 0;
    uint8_t c_buffer[50]  = {0};
    char c_temp[2]        = {0}; /* Contain two character use for conversion */

    /* calculate length of buffer to save data field */
    buf_length = (length - 2) / 2;

    /* convert from char to hex each byte (including 2 characters)*/
    for(index = 1; index < length; index += 2)
    {
        c_temp[0] = file[index];
        c_temp[1] = file[index+1];
        
        /* Convert char to hex */
        sscanf(c_temp, "%x", &value);
        
        /* Save to buffer */
        if(buf_index < buf_length)
        {
            c_buffer[buf_index] = value;
            buf_index++;
        }
    }

    /* get data from buffer to corresponding field in struct */
    hexRecordStructure.recordLength = c_buffer[0];

    hexRecordStructure.address[0] = c_buffer[1];
    hexRecordStructure.address[1] = c_buffer[2];

    hexRecordStructure.recordType = c_buffer[3];

    for(index = DATA_OFFSET; index < buf_length-1; index++)
    {
        hexRecordStructure.data[index - DATA_OFFSET] = c_buffer[index];
    }

    hexRecordStructure.checksum = c_buffer[buf_length-1];
}

/**
  * @brief  Print hex record struct
  * @param  *buffer: string buffer of record line
  * @param  option: print option according to record type (DATA, EX_SEGMENT or EX_LINEEAR) )
  * @param  MSB_Addr: value of data field of record type Ex_segment and Ex_linear to calculate absolute address
  * @retval None
  */
void printHexRecordStruct(char *buffer, RECORDTYPE_STATE option, uint32_t MSB_Addr)
{
    /*size of data field = record length field */
    uint8_t size_data     = hexRecordStructure.recordLength;
    uint8_t index         = 0;
    uint32_t Abs_address  = 0; /* absolute address */

    printf("\n");
    switch(option)
    {
        case DATA:
            Abs_address = (hexRecordStructure.address[0] << 8) | hexRecordStructure.address[1];
            printf("0x%08x                ", Abs_address);
            for(index = 0; index < size_data; index++)
            {
                printf("%02x", hexRecordStructure.data[index]);
            }
            break;
        case EX_SEGMENT:
            Abs_address = ((hexRecordStructure.address[0] << 8) | hexRecordStructure.address[1]) + (MSB_Addr<<4);
            printf("0x%08x                ", Abs_address);
            for(index = 0; index < size_data; index++)
            {
                printf("%02x", hexRecordStructure.data[index]);
            }
            break;
        case EX_LINEAR:
            Abs_address = (MSB_Addr<<16) | ((hexRecordStructure.address[0] << 8) | hexRecordStructure.address[1]);
            printf("0x%08x                ", Abs_address);
            for(index = 0; index < size_data; index++)
            {
                printf("%02x", hexRecordStructure.data[index]);
            }
            break;
    }
}

/**
  * @brief  Print parser file with 2 fields absolute address and corresponding data
  * @param  filename: name of file that user want to print
  * @retval None
  */
void printfHexRecord(char *filename)
{
    R_STATE readFileFlag     = R_FAILED; /* read file state (in File_handle.h) */
    uint8_t size             = 0;
    uint32_t Upper_Addr      = 0; /* merge data from data field in record type 0x02 and 0x04 */
    uint8_t RecordType_State = 0;
    
    readFileFlag = ReadFile(filename);
    
    printf("ABSOLUTE ADDRESS          DATA FIELD");
    
    while(getLine(buffer, MAX_BUFFER_LENGTH) != R_FAILED)
    {
        size = strlen(buffer);
        
        getLineRecord(buffer, size);
        
        if(hexRecordStructure.recordType == 0x04)
        {
            Upper_Addr = (hexRecordStructure.data[0] << 8) | hexRecordStructure.data[1];
            RecordType_State = 1;
        }
        else if(hexRecordStructure.recordType == 0x02)
        {
            Upper_Addr = (hexRecordStructure.data[0] << 8) | hexRecordStructure.data[1];
            RecordType_State = 2;
        }
        
        /* print parser file */
        if(hexRecordStructure.recordType == 0x00 && RecordType_State == 1)
        {
            printHexRecordStruct(buffer, EX_LINEAR, Upper_Addr);
            fflush(stdout);
        }
        else if(hexRecordStructure.recordType == 0x00 && RecordType_State == 2)
        {
            printHexRecordStruct(buffer, EX_SEGMENT, Upper_Addr);
            fflush(stdout);
        }
        else if(hexRecordStructure.recordType == 0x00)
        {
            printHexRecordStruct(buffer, DATA, 0);
            fflush(stdout);
        }
    }
}

/**
  * @brief  Checksum calculation function
  * @param  None
  * @retval Checksum value
  */
uint8_t calculateChecksum(void){
    uint8_t retVal     = 0; /* return value */
    uint16_t sum       = 0; /* sum of all field */
    uint8_t temp_count = 0; /* sum of record length field */
    uint16_t temp_addr = 0; /* sum of address field */
    uint8_t temp_type  = 0; /* sum of record type field */
    uint16_t temp_data = 0; /* sum of data field */
    uint8_t size_data  = hexRecordStructure.recordLength;
    uint8_t index      = 0;
    
    temp_count = hexRecordStructure.recordLength;
    
    temp_addr = hexRecordStructure.address[0] + hexRecordStructure.address[1];
    
    temp_type = hexRecordStructure.recordType;
    
    for(index = 0; index < size_data; index++)
    {
        temp_data += hexRecordStructure.data[index];
    }
    
    sum = temp_count + temp_addr + temp_type + temp_data;
    
    /* calculate checksum */
    retVal = (uint8_t)((~sum)+1);
    
    return retVal;
}

/**
  * @brief  Check number of character is correct function
  * @param  length: actual size of record line
  * @retval Checking state (CHECKING_VALID or CHECKING_ERROR)
  */
CHECKING_STATE checkSize(uint8_t length)
{
    uint8_t valid_size = 0; /* correct number of character */
    
    valid_size = (2 * hexRecordStructure.recordLength) + 12;
    
    if(valid_size == length)
    {
        return CHECKING_VALID;
    }
    return CHECKING_ERROR;
}

/**
  * @brief  Check record line is valid or no
  * @param  file: buffer array
  * @param  length: length of buffer
  * @retval Record line state (Can be a value of @ref RECORDLINE_STATE)
  */
RECORDLINE_STATE isRecordValid(char *file, uint8_t length)
{
    STATE state             = START_CODE;
    RECORDLINE_STATE Retval = LINE_START_ERROR;
    uint8_t exitwhile       = 0;
    
    /* use state machine */
    while(exitwhile == 0)
    {
        switch(state)
        {
            /* check start code */
            case START_CODE:
                if(file[0] == ':')
                {
                    /* move to next state */
                    state = SIZE;
                }
                else
                {
                    /* return and break */
                    Retval = LINE_START_ERROR;
                    exitwhile = 1;
                }
                break;
            /* check size */
            case SIZE:
                if(checkSize(length) == CHECKING_VALID)
                {
                    /* move to next state */
                    state = CHECK_SUM;
                }
                else
                {
                    /* return and break */
                    Retval = LINE_SIZE_ERROR;
                    exitwhile = 1;
                }
                break;
            /* check checksum field */
            case CHECK_SUM:
                if(calculateChecksum() == hexRecordStructure.checksum)
                {
                    /* return valid */
                    Retval = LINE_VALID;
                    exitwhile = 1;
                }
                else
                {
                    /* return error */
                    Retval = LINE_CHECKSUM_ERROR;
                    exitwhile = 1;
                }
                break;
            default:
                break;
        }
    }
    
    return Retval;
}

/**
  * @brief  Check record file is valid or no
  * @param  filename: name of file
  * @retval File checking state (Can be a value of @ref FILE_CHECKING_STATE)
  */
FILE_CHECKING_STATE checkRecordFile(char *filename)
{
    R_STATE readFileFlag         = R_FAILED;
    int16_t size                 = 0;
    uint8_t exitwhile_1          = 0;
    uint8_t exitwhile            = 0;
    FILE_CHECKING_STATE retVal   = CHECKING_ERROR;
    RECORD_FILE_STATE State      = PARSER_ERROR;   /* file checking process state */
    
    readFileFlag = ReadFile(filename);
    if(readFileFlag == R_FAILED)
    {
        retVal = FILE_RF_ERROR;
    }
    
    /* use state machine */
    while(exitwhile_1 == 0)
    {
        switch(State)
        {
            /* check file correct */
            case PARSER_ERROR:
                while(getLine(buffer, MAX_BUFFER_LENGTH) != R_FAILED && exitwhile == 0)
                {
                    size = strlen(buffer);
                    
                    getLineRecord(buffer, size);
                    
                    /* check size of each line */
                    if(isRecordValid(buffer, size) == LINE_SIZE_ERROR)
                    {
                        printf("ERROR->%s", buffer);
                        retVal = FILE_SIZE_ERROR;
                        exitwhile = 1;
                        exitwhile_1 = 1;
                    }
                    /* check start code of each line */
                    if(isRecordValid(buffer, size) == LINE_START_ERROR)
                    {
                        printf("ERROR->%s", buffer);
                        retVal = FILE_START_ERROR;
                        exitwhile = 1;
                        exitwhile_1 = 1;
                    }
                    /* check checksum value of each line */
                    if(isRecordValid(buffer, size) == LINE_CHECKSUM_ERROR)
                    {
                        printf("ERROR->%s", buffer);
                        retVal = FILE_CHECKSUM_ERROR;
                        exitwhile = 1;
                        exitwhile_1 = 1;
                    }
                }
                
                /* move to next state */
                State = PARSER_EOF;
                break;
            /* check end of file record */
            case PARSER_EOF:
                if(hexRecordStructure.recordType != 0x01)
                {
                    /* return error */
                    retVal = FILE_EOF_ERROR;
                    exitwhile_1 = 1;
                }
                else
                {
                    /* return valid */
                    retVal = FILE_VALID;
                    exitwhile_1 = 1;
                }
            default:
                break;
        }
    }
    
    return retVal;
}


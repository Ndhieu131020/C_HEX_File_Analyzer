#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include "Parser_hex.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
#define filename    "data.hex"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    FILE_CHECKING_STATE check;
    
    check = checkRecordFile(filename);
    
    switch(check)
    {
        case FILE_RF_ERROR:
            printf("KHONG MO DUOC FILE\n");
            break;
        case FILE_EOF_ERROR:
            printf("THIEU EOF\n");
            break;
        case FILE_START_ERROR:
            printf("LOI STARTCODE\n");
            break;
        case FILE_CHECKSUM_ERROR:
            printf("LOI CHECKSUM\n");
            break;
        case FILE_SIZE_ERROR:
            printf("SAI SO LUONG KY TU\n");
            break;
        case FILE_VALID:
            printf("KHONG TIM THAY LOI!\n");
            printfHexRecord(filename);
        default:
            break;
    }
    
    return 0;
}


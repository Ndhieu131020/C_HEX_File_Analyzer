#include "File_handle.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static FILE *pFile = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
  * @brief  This function allow user read a file
  * @param  filename: name of file
  * @retval Reading state (Can be a value of @ref Read file state)
  */
R_STATE ReadFile(char *filename)
{
    R_STATE retVal = R_FAILED;
    
    pFile = fopen(filename, "r");
    
    if(pFile != NULL)
    {
        retVal = R_SUCCESS;
    }
    else
    {
        retVal = R_FAILED;
    }
    return retVal;
}

/**
  * @brief  This function allow user read each line in a file
  * @param  buffer: array contain content of file line
  * @retval Reading state (Can be a value of @ref Read file state)
  */
R_STATE getLine(char *buffer, uint8_t size)
{
    R_STATE retVal = R_FAILED;
    uint8_t *check = NULL;
    
    memset(buffer, 0, size);
    
    check = fgets(buffer, size, pFile);
    
    if(check != NULL)
    {
        retVal = R_SUCCESS;
    }
    
    return retVal;
}

/**
  * @brief  This function allow user close the current file
  * @param  None
  * @retval Closing state (Can be a value of @ref Close file state)
  */
C_STATE closeFile(void)
{
    C_STATE retVal  = C_FAILED;
    int closeStatus = 0;
    
    closeStatus = fclose(pFile);
    
    if(closeStatus == 0)
    {
        retVal = C_SUCCESS;
    }
    else
    {
        retVal = C_FAILED;
    }
    
    return retVal;
}


#ifndef _FILE_HANDLE_H_
#define _FILE_HANDLE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

/*******************************************************************************
 * Definition
 ******************************************************************************/
/* @ref Read file state */
typedef enum
{
    R_SUCCESS = 0, /* Read success */
    R_FAILED  = 1  /* Read failed */
}R_STATE;

/* @ref Close file state */
typedef enum
{
    C_SUCCESS = 0, /* Close success */
    C_FAILED  = 1  /* Close failed */
}C_STATE;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
  * @brief  This function allow user read a file
  * @param  filename: name of file
  * @retval Reading state (Can be a value of @ref Read file state)
  */
extern R_STATE ReadFile(char *filename);

/**
  * @brief  This function allow user read each line in a file
  * @param  buffer: array contain content of file line
  * @retval Reading state (Can be a value of @ref Read file state)
  */
extern R_STATE getLine(char *buffer, uint8_t size);

/**
  * @brief  This function allow user close the current file
  * @param  None
  * @retval Closing state (Can be a value of @ref Close file state)
  */
extern C_STATE closeFile(void);

#endif /*  _FILE_HANDLE_H_ */


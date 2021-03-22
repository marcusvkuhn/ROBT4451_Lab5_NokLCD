#include <cmdNok5110LCD.h>
#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <nok5110LCD.h>
#include <ctype.h>

/************************************************************************************
* Function: initNok5110Cmds
* Purpose: initialize each index of the array with its correspondent string and nArgs
* arguments:
*   nok5110Cmds   -   CMD
* return:  none
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
void initNok5110Cmds(CMD* nok5110Cmds){
    nok5110Cmds[0].name = CMD0;        // initialize the first command name
    nok5110Cmds[0].nArgs = CMD0_NARGS; // initialize num of arguments in first command
    nok5110Cmds[1].name = CMD1;
    nok5110Cmds[1].nArgs = CMD1_NARGS;
    nok5110Cmds[2].name = CMD2;
    nok5110Cmds[2].nArgs = CMD2_NARGS;
}

/************************************************************************************
* Function: parseCmd
* Purpose: splits strings into separate tokens for specific purposes
* arguments:
*   nok5110Cmds   -   CMD
*   cmdLine   -   char*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int parseCmd(CMD* nok5110Cmds, char* cmdLine){
    char* token;  // a pointer variable to a string.
    //char* con;
    int i = 0;

    // token is assigned the pointer to the token returned by strtok_s
    token = strtok(cmdLine, DELIM);

    int cmdIndex = validateCmd(nok5110Cmds, token);     // finds which index to use for subsequent parsing

    if (cmdIndex != -1 && nok5110Cmds[cmdIndex].nArgs == 0){
        token = strtok(NULL, DELIM);
        if (token)     // if there is any arguments, function will return -1
            cmdIndex = -1;
    }
    if (cmdIndex != -1 && nok5110Cmds[cmdIndex].nArgs != 0){
//        if (cmdIndex == _IDX){      // for scaraMotorSpeed, nArgs = 1
//                token = strtok(NULL, DELIM);
//                // checks if token exists, is of length 1 and is alphabetic
//                if (token && isdigit((int)*token))
//                    ARGS[0] = atoi(token);            // casts as integer the value (of a char) token is pointing to
//                else
//                    cmdIndex = -1;      //
//                //if(!token)                 // if token is NULL, command is invalid, function will return -1
//                //    cmdIndex = -1;
//                token = strtok(NULL, DELIM);
//                if (token)     // if there is more than one argument, function will also return -1
//                    cmdIndex = -1;
//        }
        if (cmdIndex == SCRNLINE_IDX || cmdIndex == DRAWLINE_IDX){      // for nokLcdDrawScrnLine, nArgs = 2, for nokLcdDrawLine, nArgs = 4
            for (i = 0; token && i < nok5110Cmds[cmdIndex].nArgs; ++i){      // loops while token is valid and i is less than the nArgs for the command, same for every following for-loop
                token = strtok(NULL, DELIM);
                if (token)                                              // every following token here will be a double
                    NOK_ARG[i] = atoi(token);            // converts to int, then stores into the array
                else if (!token)                // token invalid in the range of nArgs
                    cmdIndex = -1;
            }
            if (token = strtok(NULL, DELIM))     // looks for more than nArgs arguments, which will also make the function return -1
                cmdIndex = -1;
        }
    }
    return cmdIndex;
}

/************************************************************************************
* Function: validadeCmd
* Purpose: uses cmdIndex to execute a scaraCmds[cmdIndex] Scara command.
* It will perform data type casting (double -> int or char) when necessary.
* arguments:
*   scaraCmds   -   CMD*
*   cmdName     -   char*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int validateCmd(CMD* nok5110Cmds, char* cmdName){
    unsigned int i = 0;
    int idx = -1;
    int invalidCmd = 1;
    while (invalidCmd && i < MAX_CMDS)
        invalidCmd = strcmp(cmdName, nok5110Cmds[i++].name);
    if (!invalidCmd)
        idx = i - 1;
    return idx;
}

/************************************************************************************
* Function: executeCMD
* Purpose: uses cmdIndex to execute a scaraCmds[cmdIndex] Scara command.
* It will perform data type casting (double -> int or char) when necessary.
* arguments:
*   scaraCmds   -   CMD*
*   cmdIndex   -   int
*   scaraState   -   SCARA_ROBOT*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int executeCMD(CMD* nok5110Cmds, int cmdIndex){
    switch (cmdIndex) {
    case SCRNLINE_IDX:
        if(NOK_ARG[2] == 0)
            nokLcdDrawScrnLine(NOK_ARG[0],NOK_ARG[1], 'H');
        else if(NOK_ARG[2] == 1)
            nokLcdDrawScrnLine(NOK_ARG[0],NOK_ARG[1], 'V');
        break;
    case DRAWLINE_IDX:
        nokLcdDrawLine(NOK_ARG[0], NOK_ARG[1], NOK_ARG[2], NOK_ARG[3]);
        break;
    case CLEAR_IDX:
        nokLcdClear();
        break;
    default:
        break;
    }
    return cmdIndex;
}

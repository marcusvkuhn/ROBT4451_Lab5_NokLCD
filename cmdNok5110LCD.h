/*
 * cmdVNH7070.h
 *
 *  Created on: Feb 23, 2021
 *      Author: Raymond
 */

#ifndef CMDNOK5110LCD_H_
#define CMDNOK5110LCD_H_

#define     MAX_CMDS             3

#define     CMD0                 "nokLcdDrawScrnLine"
#define     CMD0_NARGS           3
#define     SCRNLINE_IDX         0

#define     CMD1                 "nokLcdDrawLine"
#define     CMD1_NARGS           4
#define     DRAWLINE_IDX         1

#define     CMD2                 "nokLcdClear"
#define     CMD2_NARGS           0
#define     CLEAR_IDX            2

#define     DELIM                " ,\t"
#define     NULL                 '\0'          // null char
#define     NOK_ARG              nok5110Cmds[cmdIndex].args
#define     MAX_ARGS             4

#define     QUIT_IDX             6

typedef struct CMD {
    const char *name; // command name
    int nArgs; // number of input arguments for a command
    int args[MAX_ARGS]; // arguments
}CMD;

//-------------- func prototypes-------------

void initNok5110Cmds(CMD* vnh7070Cmds);
int parseCmd(CMD* vnh7070Cmds, char* cmdLine);
int validateCmd(CMD* scaraCmds, char* cmdName);
int executeCMD(CMD* vnh7070Cmds, int cmdIndex);

#endif /* CMDNOK5110LCD_H_ */

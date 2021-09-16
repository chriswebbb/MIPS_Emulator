#include "include.h"
#include "makeMachine.h"
#include "memory.h"
#include "control.h"

void iniMIPS();                                                 //Initialise the processor
void iniReg(mipsState & state, int sizeOfMem);                  //Initialise the register
void genUI(mipsState & state, int sizeOfMem, uint32_t * mem);   //Generate the user interface


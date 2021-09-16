//
// Created by mater on 07/04/2021.
//

#include "mipsProcessor.h"

void iniMIPS() {
    int sizeOfMem = 150;                                          //Size of the memory to be used
    bool error = false;                                           //Boolean variable used for small errors checking
    bool HALT = false;                                            //Boolean variable used to stop the program if coded as such
    mipsState state;                                              //Holds the current state of the entire MIPS processor
    state.cycle.fetch = true;                                     //Assign the fetch part of the cycle to be true to initiate the program
    uint32_t *mem = constructMem(sizeOfMem);                      //Generate the memory and pass the pointer

    moveBinaryToMem("machineTwo.txt", mem);           //Move the binary values in the text file into memory
    iniReg(state, sizeOfMem);                                  //Initialise the registers

    while (!HALT && !error) {                                     //While there are no errors or halt hasn't been called cycle through the states
        MIPS(state, error, mem, HALT);                  //Call the function responsible for the MIPS
        state.reg[0] = 0;                                         //Ensure the register $zero is always zero
    }

    genUI(state, sizeOfMem, mem);                              //Generate the UI

    delete[] mem;                                                 //Free the memory
    mem = nullptr;                                                //Set the pointer to null
}

void iniReg(mipsState & mipsState, int sizeOfMem)
{
    for (int n = 0; n < 32; n++) {          //Set all register values to zero
        mipsState.reg[n] = 0;
    }
    mipsState.reg[29] = (sizeOfMem-1)*4;    //Set the $sp register to the top of the stack
    mipsState.reg[30] = sizeOfMem*4;        //Set the $fp register to the bottom of the stack
}

void genUI(mipsState & state, int sizeOfMem, uint32_t * mem)
{
    std::cout << "Total number instructions run :" << state.totalInstructions << "\n" << "Number of R instructions :" << state.totalR << "\n" << "Number of I instructions :" << state.totalI << "\n" << "Number of J instructions :" << state.totalJ;

    std::cout << "\n\nMemory map                                                              Register map\n";
    std::cout << "__________________________________________________________                 _____________________________________\n";
    std::cout << "|Index of RAM |Physical address   |Value  at this address|                 |Index of register | Register value |\n";
    std::cout << "|-------------|-------------------|----------------------|                 |------------------|----------------|\n";

    std::string sRegVal;

    for (int i = 0; i < sizeOfMem; i++) {
        std::cout << "|    " << std::setw(3) << i << "      |    " << std::setw(9) << &mem[i] << "    |            " << std::setw(10) << mem[i] << "|";
        std::cout << "                 ";
        sRegVal = std::to_string(state.reg[i]);
        if(i<32)
        {
            std::cout << "|" << std::setw(10) << i << "        |" << std::setw(9) << state.reg[i] << "       |" << "\n";
        }else{
            if(i==32)
            {
                std::cout << "|__________________|________________|\n";
            }else {
                std::cout << "\n";
            }
        }
    }

    std::cout << "|_____________|___________________|______________________|\n";
}
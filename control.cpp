#include "control.h"

void MIPS(mipsState & state, bool & error, uint32_t * RAM, bool & HALT){

    if(state.cycle.fetch){        //Check if in the fetch stage

        ++state.totalInstructions;//Add to total instructions

        //All of the bits within the section are read left to right so
        //MSB is on the right and LSB is on the left

        //Setup for R based instructions

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //      | opcode |   rs   |   rt   |   rd   |  shamt |  func  |
        //      | 6 bits | 5 bits | 5 bits | 5 bits | 5 bits | 6 bits |
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        state.vars.fullInstruct = RAM[state.presentPC];
        state.vars.opcode = (state.vars.fullInstruct & (uint32_t)0x0000003F);
        state.vars.rs = (state.vars.fullInstruct & (uint32_t)0x000007C0) >> 6u;
        state.vars.rt = (state.vars.fullInstruct & (uint32_t)0x0000F800) >> 11u;
        state.vars.rd = (state.vars.fullInstruct & (uint32_t)0x001F0000) >> 16u;
        state.vars.shamt = (state.vars.fullInstruct & (uint32_t)0x03E00000) >> 21u;
        state.vars.func = (state.vars.fullInstruct & (uint32_t)0xFC000000) >> 26u;

        //Setup for I based instructions

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //      | opcode |   rs   |   rt   |           imm            |
        //      | 6 bits | 5 bits | 5 bits |         16 bits          |
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        state.vars.imm = (state.vars.fullInstruct & (uint32_t)0xFFFF0000) >> 16u;   //This will set all bits apart from the immediate to 0

        if (state.vars.imm & (uint32_t)0x8000) {
            state.vars.signExt = state.vars.imm | 0xFFFF0000;
        } else {
            state.vars.signExt = state.vars.imm | (uint32_t)0x00000000;
        }

        //Setup for J based instructions

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //      | opcode |           address           |
        //      | 6 bits |           26 bits           |
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        state.vars.address = (state.vars.fullInstruct & (uint32_t)0xFFFFFFC0) >> 6u; //FFFFFFC0

        //Transfer to the decode phase.

        state.cycle.fetch = false;
        state.cycle.decode = true;

    }else if(state.cycle.decode){                           //check for decode
            switch (state.vars.opcode) {                    //switch case for the opcode
                case 0b000000:                              //This is for register based instructions
                    switch (state.vars.func) {
                        case 0b000001:
                            state.exeSig.addR = true;       //Set the execute addR signal to on for executre phase
                            ++state.presentPC;              //Increase the presentPC
                            ++state.totalR;                 //Increment total register instruction
                            state.cycle.decode = false;     //Prepare for execute phase
                            state.cycle.execute = true;
                            return;
                        case 0b000010:
                            state.exeSig.subR = true;
                            ++state.presentPC;
                            ++state.totalR;
                            state.cycle.decode = false;
                            state.cycle.execute = true;
                            return;
                        default:
                            error = true;
                            HALT = true;
                    }
                    return;
                case 0b000001:
                    state.exeSig.addImm = true;
                    ++state.presentPC;
                    ++state.totalI;
                    state.cycle.decode = false;
                    state.cycle.execute = true;
                    return;
                case 0b000010:
                    state.exeSig.jump = true;
                    ++state.totalJ;
                    state.cycle.decode = false;
                    state.cycle.execute = true;
                    return;
                case 0b000011:
                    state.exeSig.branchNot = true;
                    ++state.totalI;
                    state.cycle.decode = false;
                    state.cycle.execute = true;
                    return;
                case 0b000100:
                    state.exeSig.storeWord = true;
                    ++state.presentPC;
                    ++state.totalI;
                    state.cycle.decode = false;
                    state.cycle.execute = true;
                    return;
                case 0b011111:
                    HALT = true;
                    return;
                default:
                    error = true;
                    HALT = true;
                    return;
            }
    }else if(state.cycle.execute){
        if(state.exeSig.addR){          //if the execute signal for register based adding is on then call function which will add the values but assign them to a holding variable waiting for
            add(state);              //the write back phase or the memory write phase.
            state.wbSig.addR = true;
            state.exeSig.addR = false;
        }
        else if(state.exeSig.subR){
            sub(state);
            state.wbSig.subR = true;
            state.exeSig.subR = false;
        }
        else if(state.exeSig.addImm){
            addi(state);
            state.wbSig.addImm = true;
            state.exeSig.addImm = false;
        }
        else if(state.exeSig.jump){
            j(state);
            state.exeSig.jump = false;
        }
        else if(state.exeSig.branchNot){
            bne(state);
            state.exeSig.branchNot = false;
        }
        else if(state.exeSig.storeWord){
            sw(state);
            state.acc.sw = true;
            state.exeSig.storeWord = false;
        }

        state.cycle.execute = false;
        state.cycle.memAccess = true;

    }else if(state.cycle.memAccess) {
        if (state.acc.sw)RAM[state.reg[state.vars.rt] / 4 + state.vars.signExt / 4] = state.wordVal;
        //std::cout << &swRAM[state.reg[state.vars.rt] / 4 + state.vars.signExt / 4] = state.wordVal;

        state.acc.sw = false;
        state.cycle.memAccess = false;
        state.cycle.wb = true;

    }else if(state.cycle.wb){                       //assigns the values to either memory or registers based on control signals
        if(state.wbSig.addR || state.wbSig.subR){
            state.reg[state.vars.rd] = state.regDes;
            state.wbSig.addR = false;
            state.wbSig.subR = false;
        }
        else if(state.wbSig.addImm){
            state.reg[state.vars.rt] = state.regTrg;
            state.wbSig.addImm = false;
        }

        state.cycle.wb = false;
        state.cycle.fetch = true;
    }
}

void add(mipsState & state)
{
   state.regDes = state.reg[state.vars.rs] + state.reg[state.vars.rt];  //Assign to holding varibale the sum of the contents of rs and rt
}

void sub(mipsState & state)
{
    state.regDes = state.reg[state.vars.rs] - state.reg[state.vars.rt]; //Assign to holding varibale the difference of the contents of rs and rt
}

void addi(mipsState & state)
{
    state.regTrg = state.reg[state.vars.rs] + state.vars.signExt ;      //Assign to holding varibale the sum of the contents of rs and an immediate
}

void j(mipsState & state)
{
    state.presentPC = state.presentPC + state.vars.address;             //Assign to present PC the sum of the program counter and the offset
}

void bne(mipsState & state)
{
    if(state.reg[state.vars.rs] != state.reg[state.vars.rt])            //If the contents of rt and rs do not equal then assign to present PC to be the signExt offset + present PC
    {
        state.presentPC = state.vars.signExt + state.presentPC;
    }else{
        ++state.presentPC;
    }
}

void sw(mipsState & state)
{
    state.wordVal = state.reg[state.vars.rs];                           //Assign the value desired to be stored from rs into the holding variable
}
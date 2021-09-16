#include "include.h"

struct instructReg{
    uint32_t fullInstruct=0;//holds full instructs
    uint32_t opcode=0;      //holds opcode
    uint32_t rs=0;
    uint32_t rt=0;
    uint32_t rd=0;
    uint32_t shamt=0;
    uint32_t func=0;

    uint16_t imm=0;   //This will set all bits apart from the immediate to 0
    int32_t signExt=0;

    uint32_t address=0;
};

//Structure to control the state of the processor
struct cycleState{
    bool fetch = false;
    bool decode = false;
    bool execute = false;
    bool wb = false;
    bool memAccess = false;
};

//Control signal associated with execution
struct mipsExeSig{
    bool addR = false;
    bool subR = false;
    bool addImm = false;
    bool jump = false;
    bool branchNot = false;
    bool storeWord = false;
    bool HALTProg = false;
};

//Control signal associated with write back
struct mipsWBSig{
    bool addR = false;
    bool subR = false;
    bool addImm = false;
    bool jump = false;
    bool branchNot = false;
    bool storeWord = false;
    bool HALTProg = false;
};

//Control signal associated with memory access
struct memAcc{
    bool sw = false;
};

struct mipsState{
    uint32_t presentPC = 0;
    uint32_t nextPC = 0;
    int32_t reg[32];            //Holds the registers as an array

    instructReg vars;
    cycleState cycle;
    mipsExeSig exeSig;
    mipsWBSig wbSig;
    memAcc acc;

    uint32_t regDes = 0x0000000;    //Holds value when going from decode to execute
    uint32_t regSrc = 0x0000000;
    uint32_t regTrg = 0x0000000;
    uint32_t wordVal = 0x0000000;

    int mode = 0;               //determines what mode of operation the program is in.
    int totalInstructions = 0;
    int totalI = 0;
    int totalR = 0;
    int totalJ = 0;
};

void MIPS(mipsState & mipsState, bool & executed, uint32_t * RAM, bool & HALT);
void add(mipsState & mipState);
void sub(mipsState & mipState);
void addi(mipsState & mipState);
void j(mipsState & mipState);
void bne(mipsState & mipState);
void sw(mipsState & mipsState);
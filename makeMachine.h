#include "include.h"

//Data structure for branches and jumpes to be made easier
struct hops{
    std::string name;
    int index = 0;
};

bool assembler(std::string inFile);                                                                                                                                     //Function to change MIPS assembly into machine
int lengthOfTXT(std::string fileName);                                                                                                                                  //Finds the number of rows in txt file
void fileOpen(std::vector<char> & tokens, int & sizeOfTokens, std::string fileName);                                                                                                          //Opens the file specified
void findCommands(std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop);                             //Locates the commands words, only works if they're lowercase letters
void findRegs(std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop);                                 //Locates the registers within the vector of characters
void findNum(std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop);                                  //Locates the offsets and immediate, used base ten values
void findHops(std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop, std::vector<hops> & leaps);    //Locates branches and jumps, the symbols have to capitalised
void genTokens(std::vector<char> & cChar, std::vector<std::string> & tokens, std::vector<hops> & leaps , int & sizeOfcChar);                                          //Generates the a vector of symbols
std::string regSelect(std::vector<std::string> & opcode, std::string & binOut, int & cnt);                                                                              //Allows the selection of a particular register
void addressBranch(std::vector<std::string> & tokens, std::vector<hops> & leaps);                                                                                      //Assign the index value of the branch symbols
std::string completeBranch(std::vector<std::string> & tokens, std::vector<hops> & leaps, std::string & binOut, int cnt, int presentPC);                                //Provides the offset to the branch instruction
std::string completeJump(std::vector<std::string> & tokens, std::vector<hops> & leaps, std::string & binOut, int cnt, int PC);
void genBin(std::vector<std::string> & binForOut, std::vector<std::string> & tokens, std::vector<hops> & leaps);                                                       //Generates the binary instructions
void outputToFile(std::vector<std::string> & binForOut);                                                                                                              //Places binary within a file

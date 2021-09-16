#include "mipsProcessor.h"

int main() {

    std::string name;

    std::cout << "\n\nPlease enter the assembly file name with the txt format :\n";   //Used to just increase gap between programme and commands

    std::cin >> name;

    std::cout << "\n\n";

    assembler(name);                                                                  //Converts the assembly into machine

    iniMIPS();                                                                        //Calls the function to simulate the processor

    return 0;
}
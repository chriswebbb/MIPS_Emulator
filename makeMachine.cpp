#include "include.h"
#include "makeMachine.h"

bool assembler(std::string inFile)
{
    std::vector<char> cChar;                                        //Vector which will hold all the characters seperate to be worked on
    std::vector<std::string> tokens;                                //Vector to hold the tokens extracted from the assembly
    std::vector<std::string> binOut;                                //Vector to hold the binary strings
    std::vector<hops> leaps;                                        //Vector to hold branches and jumps
    int sizeOfcChar = 0;                                            //Integer to hold size of the cChar vector so that size() doesnt need to be used so often

    //Explained in the header file
    fileOpen(cChar, sizeOfcChar, inFile);
    genTokens(cChar, tokens, leaps, sizeOfcChar);
    addressBranch(tokens, leaps);
    genBin(binOut, tokens, leaps);
    outputToFile(binOut);

    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Functions

int lengthOfTXT(std::string fileName)
{
    int length = 0;             //Holds the length to be returned
    std::ifstream f(fileName);  //Opens the file provided
    char line[64];              //Will hold the line
    while (!f.eof())            //While the file isnt at the end
    {
        f.getline(line,64);  //Get line, store in line with a max lenght of 64
        length++;               //Increment the length value
    }
    return length;              //Return the length
}

std::string convIntDenToStrBin(int val){
    std::bitset<16> b(val); //Converts the argument given to a 16 bit binary representation
    return b.to_string();   //Converts the binary value into a string and returns it
}

std::string convStrDenToStrBin(std::string val)
{
    int temp = 0;                   //The variable will hold the value of the string
    std::stringstream geek(val);    //Converts the string into an int
    geek >> temp;                   //Shifts the value into temp
    std::bitset<16> b(temp);        //Converts the value into a 16 bit number
    return b.to_string();           //Converts the value into a string
}

void fileOpen(std::vector<char> & cChar, int & sizeOfcChar, std::string fileName)
{
    char hold;                          //Character to hold the input character from file

    std::ifstream fileIn;               //Declare file and as an read only
    fileIn.open(fileName);              //Opens the file

    if(!fileIn.is_open())               //Checks if the file has been opened, if not will cause the program to exit with a value of 20
    {
        exit(20);
    }

    while(!fileIn.eof())                //While the file isnt at the end extract a character and append it to the cChar vector
    {
        fileIn.get(hold);            //Retrieves the letter and assigns it to the variable hold
        cChar.push_back(hold);          //Push the character held onto the vector
        ++sizeOfcChar;                  //Increment the integer
    }

    for(int i = 0; i < sizeOfcChar - 1; i++) std::cout << cChar[i] << "";   //Output the characters as they're read

    std::cout << "\n\n\n";
}

void findCommands(std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop)
{
    std::string temp;                       //String that will be added to, so to gather the symbols
    bool halt = false;                      //If the keyword hlt is reached stop the process

    while(islower(letter) && !halt)         //While the letter is lower case and halt is false, add a letter the temp variable
    {
        if(cnt >= sizeOfcChar) stop = true; //ensures the end of the vector hasn't been reached
        if(temp == "hlt")halt = true;
        temp += letter;
        ++cnt;
        letter = cChar[cnt];
    }
    tokens.push_back(temp);                //Symbol to be added
}

void findRegs(std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop)
{
    std::string temp;                           //String that will be added to, so to gather the symbols
    temp += letter;                             //Add the $ to the symbol
    ++cnt;                                      //Increment the character being accessed
    letter = cChar[cnt];                        //Assign the letter

    while(islower(letter) || isdigit(letter))   //While the letter is lower case or a digit is present add to the temp variable
    {
        if(cnt >= sizeOfcChar) stop = true;
        temp += letter;
        ++cnt;
        letter = cChar[cnt];
    }
    tokens.push_back(temp);                    //Symbol is added
}

void findNum(std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop)
{
    std::string temp;

   if(letter == '-')                        //If there is a minus sign add it and increment to the next character
   {
       temp += letter;
       ++cnt;
       letter = cChar[cnt];
   }

   while(isdigit(letter))                   //While there is a digit add to the symbol.
   {
       if(cnt >= sizeOfcChar) stop = true;
       temp += letter;
       ++cnt;
       letter = cChar[cnt];
   }

   tokens.push_back(temp);                 //append the number

}

void findHops( std::vector<char> & cChar, std::vector<std::string> & tokens, char & letter, int & cnt, int & sizeOfcChar, bool & stop, std::vector<hops> & leaps)
{
    std::string temp;
    hops leap;                              //Create a hops struct

    while(isupper(letter))                  //While the letters are uppercase add to the variable and append once its not
    {
        if(cnt >= sizeOfcChar) stop = true;
        temp += letter;
        ++cnt;
        letter = cChar[cnt];
    }

    if(letter == ':') {                     //checks for : as it was added during the while loop and so this seperates the location of the jump/branch and the instruction
        leap.name = temp;                   //Gives the name of the symbol
        leaps.push_back(leap);              //Pushes the jump/branch onto the vector
        tokens.push_back(temp);            //Pushed the symbol onto the symbols
    }else{
        tokens.push_back(temp);            //If its an instruction then the symbol alone is appended
    }
}

void genTokens(std::vector<char> & cChar, std::vector<std::string> & tokens, std::vector<hops> & leaps, int & sizeOfcChar)
{
    bool stop = false;                                                                                                      //stops the loop
    int index = 0;                                                                                                          //used as the index to the cChar vector
    char letter;                                                                                                            //used as a holder for the character being investigated

    while(!stop && (index < sizeOfcChar))
    {
        letter = cChar[index];
        if(islower(letter))findCommands(cChar, tokens, letter, index, sizeOfcChar, stop);              //if lowercase find commands
        if(letter == '$')findRegs(cChar, tokens, letter, index, sizeOfcChar, stop);                    //$ is found find a reg
        if(isdigit(letter) || letter == '-')findNum(cChar, tokens, letter, index, sizeOfcChar, stop);  //If a digit look for num
        if(isupper(letter))findHops(cChar, tokens, letter, index, sizeOfcChar, stop, leaps);                                //if uppercase find jumps/branches
        ++index;
    }
}

void addressBranch(std::vector<std::string> & tokens, std::vector<hops> & leaps){

    int cnt = 0;                                        //Holds the PC
    int jmpIndex = 0;                                   //Holds the index to the jumps vector to access the jumps/vectors
    std::string symbol;                                 //Holds the symbol

    for(int i = 0; i < tokens.size(); i++, cnt++){     //For loop to find the jump destinations and note their locations
        symbol = tokens[i];
        if (symbol == "add") i+=3;
        else if(symbol == "sub") i+=3;
        else if(symbol == "addi") i+=3;
        else if(symbol == "j") i++;
        else if(symbol == "bne") i+=3;
        else if(symbol == "sw") i+=3;
        else if(symbol == "hlt");
        else{
            if(symbol == leaps[jmpIndex].name)
            {
                leaps[jmpIndex].index = cnt;
                ++jmpIndex;
            }
        }
    }
    //print the jumps
    //for(int j = 0; j < tokens.size();j++) std::cout << "Jump name: " <<jumps[j].name << " Jump destination: " << jumps[j].index;

}

std::string regSelect(std::vector<std::string> & opcode, std::string & binOut, int cnt)
{
    //checks the opcode given and the concatenates the string given with the register value.
    if(opcode[cnt] == "$zero")binOut = binOut + "00000";
    else if(opcode[cnt] == "$at")binOut = binOut + "10000";
    else if(opcode[cnt] == "$v0")binOut = binOut + "01000";
    else if(opcode[cnt] == "$v1")binOut = binOut + "11000";
    else if(opcode[cnt] == "$a0")binOut = binOut + "00100";
    else if(opcode[cnt] == "$a1")binOut = binOut + "10100";
    else if(opcode[cnt] == "$a2")binOut = binOut + "01100";
    else if(opcode[cnt] == "$a3")binOut = binOut + "11100";
    else if(opcode[cnt] == "$t0")binOut = binOut + "00010";
    else if(opcode[cnt] == "$t1")binOut = binOut + "10010";
    else if(opcode[cnt] == "$t2")binOut = binOut + "01010";
    else if(opcode[cnt] == "$t3")binOut = binOut + "11010";
    else if(opcode[cnt] == "$t4")binOut = binOut + "00110";
    else if(opcode[cnt] == "$t5")binOut = binOut + "10110";
    else if(opcode[cnt] == "$t6")binOut = binOut + "01110";
    else if(opcode[cnt] == "$t7")binOut = binOut + "11110";
    else if(opcode[cnt] == "$s0")binOut = binOut + "00001";
    else if(opcode[cnt] == "$s1")binOut = binOut + "10001";
    else if(opcode[cnt] == "$s2")binOut = binOut + "01001";
    else if(opcode[cnt] == "$s3")binOut = binOut + "11001";
    else if(opcode[cnt] == "$s4")binOut = binOut + "00101";
    else if(opcode[cnt] == "$s5")binOut = binOut + "10101";
    else if(opcode[cnt] == "$s6")binOut = binOut + "01101";
    else if(opcode[cnt] == "$s7")binOut = binOut + "11101";
    else if(opcode[cnt] == "$t8")binOut = binOut + "00011";
    else if(opcode[cnt] == "$t9")binOut = binOut + "10011";
    else if(opcode[cnt] == "$k0")binOut = binOut + "01011";
    else if(opcode[cnt] == "$k1")binOut = binOut + "11011";
    else if(opcode[cnt] == "$gp")binOut = binOut + "00111";
    else if(opcode[cnt] == "$sp")binOut = binOut + "10111";
    else if(opcode[cnt] == "$fp")binOut = binOut + "01111";
    else if(opcode[cnt] == "$ra")binOut = binOut + "11111";
    else binOut = binOut + "00000";

    //Return the binary string
    return binOut;
}

std::string completeBranch(std::vector<std::string> & tokens, std::vector<hops> & jumps, std::string & binOut, int cnt, int PC)
{
    std::string jumpTo = tokens[cnt+3];                    //Assign the branch symbol to the variable
    std::string temp;                                       //Used to reverse the string order

    int i = 0;                                              //used to count find the destination label
    int offset = 0;                                         //The immediate value to be added

    while((jumpTo != jumps[i].name) && (i<jumps.size()))    //While loop to go through the jumps vector to find the relevant branch
    {
          ++i;
    }

    if(i>jumps.size())exit(10);                       //Ensures that i didn't exceed the limit

    offset = jumps[i].index - PC + 1;                       //This calculates the offset from the PC to the branching location
    temp = convIntDenToStrBin(offset);                      //Convert the int into a binary string
    reverse(temp.begin(),temp.end());                       //Reverse the order
    temp = binOut + temp;                                   //Concatinate the string

    //Prints the branch offset value to go from the instruction to the destination
    //std::cout << "Name of branch: " << jumps[i].name << ",  Offset from the branch command to branch destination" << temp;

    return temp;
}

std::string completeJump(std::vector<std::string> & tokens, std::vector<hops> & leaps, std::string & binOut, int cnt, int PC)
{
    std::string jumpTo = tokens[cnt+1];                    //Assign the branch symbol to the variable
    std::string temp;                                       //Used to reverse the string order

    int i = 0;                                              //used to count find the destination label
    int distance = 0;                                         //The immediate value to be added

    while((jumpTo != leaps[i].name) && (i<leaps.size()))    //While loop to go through the jumps vector to find the relevant branch
    {
        ++i;
    }

    if(i>leaps.size())exit(10);                       //Ensures that i didn't exceed the limits

    distance = leaps[i].index;                         //This calculates the offset from the PC to the branching location
    std::bitset<26> b(distance);                            //Converts the argument given to a 16 bit binary representation
    temp = b.to_string();                                   //Converts the binary value into a string and returns it
    reverse(temp.begin(),temp.end());                       //Reverse the order
    temp = binOut + temp;                                   //Concatinate the string
    //std::cout<<"\n\nyeet   : " << temp << "   " << leaps[i].index << " " << distance << "\n\n";
    return temp;
}

void genBin(std::vector<std::string> & binForOut, std::vector<std::string> & tokens, std::vector<hops> & leaps)
{
    std::string temp;                                           //Used to concatinate the strings to be appended to binForOut
    std::string tempTwo;                                        //Used to help immediate values be processed

    std::string opcode;                                         //Holds the opcode

    int cnt = 0;                                                //Equivalent to the program counter

    for(int i = 0; i < tokens.size(); i++, cnt++)
    {
        opcode = tokens[i];

        if(opcode == "add")
        {
            temp = "000000";
            temp = regSelect(tokens, temp, i+3);   //add rs
            temp = regSelect(tokens, temp, i+2);   //add rt
            temp = regSelect(tokens, temp, i+1);   //add rd
            temp = temp + "00000100000";                       //add shamt and opcode
            binForOut.push_back(temp);                         //append the binary string
            i += 3;                                            //Skip the next three symbols to get to the next symbol
        }else if(opcode == "sub")
        {
            temp = "000000";
            temp = regSelect(tokens, temp, i+3);   //add rs
            temp = regSelect(tokens, temp, i+2);   //add rt
            temp = regSelect(tokens, temp, i+1);   //add rd
            temp = temp + "00000010000";                       //add shamt and opcode
            binForOut.push_back(temp);                         //append the binary string
            i += 3;
        }else if(opcode == "addi"){
            temp = "100000";
            temp = regSelect(tokens, temp, i+2);
            temp = regSelect(tokens, temp, i+1);
            tempTwo = tokens[i+3];                             //Assign the immediate
            tempTwo = convStrDenToStrBin(tempTwo);              //Convert the integer string to a binary string
            reverse(tempTwo.begin(),tempTwo.end());             //Reverse the order
            temp = temp + tempTwo;                              //Concatinate
            binForOut.push_back(temp);                          //Append
            i+=3;                                               //Next instruction
        }else if(opcode == "j"){
            temp = "010000";
            temp = completeJump(tokens, leaps, temp, i, cnt);
            binForOut.push_back(temp);
            //std::cout << "\n temp" << temp << "\n";
            i++;
        }else if(opcode == "bne"){
            temp = "110000";
            temp = regSelect(tokens, temp, i+2);                       //add rs
            temp = regSelect(tokens,temp,i+1);                         //add rt
            temp = completeBranch(tokens, leaps, temp, i, cnt);         //add imm
            binForOut.push_back(temp);
            i += 3;
        }else if(opcode == "sw"){
            temp = "001000";
            temp = regSelect(tokens, temp, i+1);
            temp = regSelect(tokens, temp, i+3);
            tempTwo = tokens[i+2];
            tempTwo = convStrDenToStrBin(tempTwo);
            reverse(tempTwo.begin(),tempTwo.end());
            temp = temp + tempTwo;
            binForOut.push_back(temp);
            i +=3 ;
        }else if(opcode == "hlt"){
            temp = "11111100000000000000000000000000";
            binForOut.push_back(temp);
        }
    }
}

void outputToFile(std::vector<std::string> & binForOut){

    std::string fileName = "machineTwo.txt";                                        //Hardcode a filename

    std::ofstream fileOut;                                                          //Generate a file if one is not present or access the file
    fileOut.open(fileName);                                                         //Open the file

    if(!fileOut.is_open()) exit(30);                                          //Ensure that it is open

    for(int i = 0; i < binForOut.size(); i++)fileOut << binForOut[i] << std::endl;  //Read the binary values into the file

    fileOut.close();                                                                //Close the file
}
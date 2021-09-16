//
// Created by mater on 30/03/2021.
//

#include "include.h"

uint32_t * constructMem(int sizeOfMem)
{
    uint32_t * ram = nullptr;                   //Creating the ram pointer
    ram = new uint32_t [sizeOfMem];             //Generating the block of memory associated with the task
    for( int i = 0; i<sizeOfMem; i++)ram[i] = 0;//For loop that assigns the value zero to each element
    return &ram[0];                             //Returns the address of the first element to the array and there fore the whole array
}

void moveBinaryToMem(std::string fileNameBinary, uint32_t * pointerToMem)
{
    uint32_t total = 0;                 //This variable will hold the value of the binary string associated by the end
    int cnt;                            //This variable will be used by the for loop to count
    int index = 0;                      //This variable is used for indexing the RAM to assign the value
    std::ifstream f(fileNameBinary);    //ifstream is input file stream and will allow the reading of the file provided in the argument, automatically opens the file
    char lineBuffer[35];                //This is a string of fixed length which will hold the individual lines of machine code in the file

    while(!f.eof())                     //While the file isn't at the end of the file
    {
        f.getline(lineBuffer,34);    //Retrieves the next line in the file, stores it in line buffer and 32 is the max number of characters which will be extracted per line

        for(cnt = 0; cnt<=31; cnt++)    //For loop used to cycle through each character in the string
        {
            if(lineBuffer[cnt] == '1')  //This will determine if the character is a one or not
            {
                total |= (1<<cnt);      //If true then the cnt will signify where that one is a so shift a one to that position and bitwise or it to the previous value
            }
        }
        pointerToMem[index] = total;    //Assigns the binary value to the corresponding value in memory

        index++;                        //Increment the index to the next memory position.

        total = 0;                      //Must reset the total
    }

    f.close();                          //Close the file

}

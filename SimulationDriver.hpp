//Kyrylo Glamazdin
//CSCI 340

#ifndef SIMULATION_DRIVER_
#define SIMULATION_DRIVER_

#include "MemorySimulation.hpp"
#include <string>

//sets up the simulation
void runSimulation();
//asks user to provide the size of RAM
long long getInitialRAMData();
//asks user to provide the number of hard disks
int getInitialHDData();
//continuously listens for user input and calls the needed functions
void listenForInput(MemorySimulation& m);
//removes extra spaces
//returns true if there's two words in the input
bool trimInput(string& s);
//calls the appropriate function for the command that doesn't have spaces
void decodeSingleWord(string w, MemorySimulation& m);
//calls the appropriate function for the command that has a space in the middle
void decodeDoubleWord(string w, MemorySimulation& m);

#endif

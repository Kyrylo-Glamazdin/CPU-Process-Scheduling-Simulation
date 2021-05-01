//Kyrylo Glamazdin
//CSCI 340

#ifndef PROCESS_
#define PROCESS_
#include <iostream>
using namespace std;

enum ProcessType {regular, real_time};

class Process{
public:
    /* CONSTRUCTOR */
    
    //parameterized constructor
    //@param PID is the ID of the process
    //@param process_size is the size of the process in bytes
    //@param pt is the type of the process (regular or real_time)
    Process(int PID, long long process_size, ProcessType pt);
    
    /* MUTATOR METHODS */
    
    //defines the first and last byte of memory that the process uses
    void setMemoryLocation(const long long start, const long long end);
    
    /* ACCESSOR METHODS */
    
    //returns the first and last bytes of memory that the process uses
    pair<long long, long long> getMemoryLocation() const;
    //returns PID of this process
    int getPID() const;
    //returns the size of the process
    long long getSize() const;
    //returns the true if the process is regular, false if it is real-time
    bool isRegular() const;
    
private:
    //process ID
    int PID_;
    //size of the process in bytes
    long long size_;
    //type of the process (regular or real_time)
    ProcessType pt_;
    //the first byte of memory that the process uses
    long long starting_memory_location_;
    //the last byte of memory that the process uses
    long long ending_memory_location_;
};

#endif

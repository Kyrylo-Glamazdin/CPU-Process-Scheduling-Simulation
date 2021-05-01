//Kyrylo Glamazdin
//CSCI 340

#ifndef MEMORY_SIMULATION_
#define MEMORY_SIMULATION_
#include <iostream>
#include <vector>
#include "ReadyQueue.hpp"
using namespace std;

struct EmptyChunk{
    long long start_position_;
    long long end_position_;
};

class MemorySimulation{
public:
    /* CONSTRUCTOR */
    
    //parameterized constructor
    //@param RAM_bytes is the number of bytes that the RAM memory is going to have, must have a value between 0 and 4 billion
    //@param num_of_hard_disks is the number of hard disks in the simulation
    MemorySimulation(const long long RAM_bytes, int num_of_hard_disks);
    
    /* MUTATOR METHODS */
    
    //adds process to the ready-queue
    //@param pt is the type of the process (regular or real_time)
    //@param process_size is the size of the process in bytes
    //return true if operation was successful
    bool addProcess(const ProcessType pt, long long process_size);
    //terminates currently running process
    //returns true if operation was successful
    bool terminateCurrentProcess();
    //runs when process terminates.
    //frees RAM from the data associated with the terminated chunk.
    //assumes that chunks are presorted by their location in memory
    void insertAndUniteEmptyChunks(EmptyChunk e);
    //the currently running process is put at the end of the ready-queue
    void endTimeSlice();
    //currently running process requires to use the hard disk @disk_num
    void requestDiskUsage(int disk_num);
    //hard disk @disk_num finished working with the process
    void endDiskUsage(int disk_num);
    
    /* ACCESSOR METHODS */
    
    //returns index of process with the id equal to @PID as found in all_processes_
    int findIndexOfProcessById(int PID) const;
    //displays the process currently using the CPU and the state of the real-time and regular ready-queues
    void displayQueueInfo() const;
    //displays the state of the hard disks and the processes that currently use them
    void displayDiskUsage() const;
    //displays the processes that are in RAM
    void displayStateOfMemory() const;
    
private:
    //capacity of RAM memory in bytes
    long long RAM_bytes_;
    //number of hard disks
    int num_of_hard_disks_;
    //ready-queue with all currently existing processes
    ReadyQueue ready_queue_;
    //process ID that will be assigned to the next arriving process
    int next_PID_;
    //a vector of all processes that were created and haven't terminated yet
    vector<Process> all_processes_;
    //free chunks of memory
    vector<EmptyChunk> free_chunks_;
    //the number of bytes of RAM that are taken by different processes
    long long RAM_bytes_used_;
};

#endif

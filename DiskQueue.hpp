//Kyrylo Glamazdin
//CSCI 340

#ifndef DISK_QUEUE_
#define DISK_QUEUE_
#include <iostream>
#include <queue>
using namespace std;

struct ProcessWithType{
    int PID;
    bool isRegular;
};

class DiskQueue{
public:
    /* CONSTRUCTOR */
    
    //parameterized constructor
    //@param disk_num is the number of the disk
    DiskQueue(int disk_num);
    
    /* MUTATOR METHODS */
    
    //adds process with @PID to the I/O queue
    void requestDisk(const int PID, const bool isRegular);
    //hard disk finishes work for the current process.
    //returns the PID and the type of the process hat finished using the disk
    ProcessWithType endUsage();
    
    /* ACCESSOR METHODS */
    
    //returns the ID of the process that currently uses the hard disk
    ProcessWithType getRunningProcess() const;
    //shows which process uses the hard disk and the associated I/O queue
    void displayDiskUsage() const;
    
private:
    //process that currently uses the hard disk
    ProcessWithType running_process_;
    //I/O queue of the processes that wait to use the disk
    queue<ProcessWithType> io_queue_;
    //the number of the disk
    int disk_number_;
};

#endif

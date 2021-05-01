//Kyrylo Glamazdin
//CSCI 340

#ifndef READY_QUEUE_
#define READY_QUEUE_
#include <iostream>
#include <queue>
#include <vector>
#include "Process.hpp"
#include "DiskQueue.hpp"

using namespace std;

class ReadyQueue{
public:
    /* CONSTRUCTOR */
    
    //default cunstructor
    //initializes an empty ready-queue
    ReadyQueue(int hard_disks_num);
    
    /* MUTATOR METHODS */
    
    //determines whether @process is a regular or a RT process, and adds its PID to the corresponding level of the ready queue
    void addProcess(const Process& process);
    //put currently running process to the end of the ready-queue and starts running the next process
    void endTimeSlice();
    //removes currently running process from the ready queue and starts running the next process
    void terminateCurrentProcess();
    //takes currently running process and simulate putting it in the head of the ready queue by setting the preempted_process_ int to the PID of the process that was preempted
    void preemptProcess();
    //process that uses the CPU requests isung the hard disk
    void requestDisk(const int disk_num);
    //hard disk finishes work for the process that uses it
    void finishDiskUsage(const int disk_num);
    
    /* ACCESSOR METHODS */
    
    //returns PID of currently running process
    int getRunningProcess() const;
    //shows the contents of the real-time ready-queue
    void displayRTReadyQueue() const;
    //shows the contents of the regular ready-queue
    void displayRegularQueue() const;
    //displays the state of the hard disks
    void displayDiskUsage() const;
    
private:
    //ready-queue for regular processes
    queue<int> regular_queue_;
    //ready-queue for RT processes
    queue<int> rt_queue_;
    //id of the process that was preempted and is put to the beginning of the ready-queue.
    //has a value of -1 if no processes are currently preempted
    int preempted_process_;
    //PID of the currently running process
    int running_process_;
    //true if the process that currently uses the CPU is a real-time process
    bool running_process_real_time_;
    //I/O queues for each disk
    vector<DiskQueue> disk_queue_;
};

#endif

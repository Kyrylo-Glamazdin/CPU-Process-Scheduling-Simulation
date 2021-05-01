//Kyrylo Glamazdin
//CSCI 340

#include "DiskQueue.hpp"

/* CONSTRUCTOR */

DiskQueue::DiskQueue(int disk_num){
    disk_number_ = disk_num;
    ProcessWithType initial_process;
    initial_process.PID = -1;
    initial_process.isRegular = true;
    running_process_ = initial_process;
}

/* MUTATOR METHODS */
void DiskQueue::requestDisk(const int PID, const bool isRegular){
    ProcessWithType p;
    p.PID = PID;
    p.isRegular = isRegular;
    
    if (running_process_.PID == -1){
        running_process_ = p;
        return;
    }
    else{
        io_queue_.push(p);
    }
}

ProcessWithType DiskQueue::endUsage(){
    ProcessWithType last_process = running_process_;
    if (!io_queue_.empty()){
        running_process_ = io_queue_.front();
        io_queue_.pop();
    }
    else{
        ProcessWithType initial_process;
        initial_process.PID = -1;
        initial_process.isRegular = true;
        running_process_ = initial_process;
    }
    return last_process;
}

/* ACCESSOR METHODS */

ProcessWithType DiskQueue::getRunningProcess() const{
    return running_process_;
}

void DiskQueue::displayDiskUsage() const{
    cout << "Hard Disk " << disk_number_ << ":" << endl;
    if (running_process_.PID == -1){
        cout << "Idle" << endl << endl;
        return;
    }
    else{
        cout << "Process using the hard disk: " << "P" << running_process_.PID << endl;
        queue<ProcessWithType> display_queue = io_queue_;
        if (display_queue.empty()){
            cout << "I/O Queue: Empty." << endl << endl;
            return;
        }
        cout << "I/O Queue: " << endl;
        cout << "HD";
        while(!display_queue.empty()){
            cout << " <-- " << "P" << display_queue.front().PID;
            display_queue.pop();
        }
        cout << endl << endl;
    }
}

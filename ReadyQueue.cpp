//Kyrylo Glamazdin
//CSCI 340

#include "ReadyQueue.hpp"

/* CONSTRUCTOR */

ReadyQueue::ReadyQueue(int hard_disks_num){
    preempted_process_ = -1;
    running_process_ = -1;
    running_process_real_time_ = false;
    for (int i = 0; i < hard_disks_num; i++){
        DiskQueue next_disk(i);
        disk_queue_.push_back(next_disk);
    }
}

/* MUTATOR METHODS */

void ReadyQueue::addProcess(const Process& process){
    //if process is regular, it runs right away if there are no currently running processes.
    //otherwise it gets pushed to the end of the regular ready-queue
    if (process.isRegular()){
        bool process_can_run = (running_process_ == -1);
        if (process_can_run){
            running_process_ = process.getPID();
            running_process_real_time_ = false;
        }
        else{
            regular_queue_.push(process.getPID());
        }
    }
    //if process is real-time, it runs immediately only if the CPU is not used by the real-time process and the real-time queue is empty.
    //otherwise it gets put to the end of the real-time ready-queue
    else{
        if (rt_queue_.empty() && !running_process_real_time_){
            if (running_process_ != -1){
                preemptProcess();
            }
            running_process_ = process.getPID();
            running_process_real_time_ = true;
        }
        else{
            rt_queue_.push(process.getPID());
        }
    }
}

void ReadyQueue::endTimeSlice(){
    if (running_process_real_time_){
        if (rt_queue_.empty()){
            return; //if the running process is the only existing real-time process, it keeps running until it's terminated
        }
        //run the next real time process from the RT-queue
        else{
            rt_queue_.push(running_process_);
            running_process_ = rt_queue_.front();
            rt_queue_.pop();
        }
    }
    //if the time slice for the regular process ends, the next regular process starts using the CPU (because if a regular process was already running, it implies that there are no real-time processes)
    else{
        regular_queue_.push(running_process_);
        running_process_ = regular_queue_.front();
        regular_queue_.pop();
    }
}

void ReadyQueue::terminateCurrentProcess(){
    //if terminated process was real-time and there are no other RT processes, run the preempted regular process (if there is one) or run next regular process
    if (running_process_real_time_){
        if (rt_queue_.empty()){
            if (preempted_process_ != -1){
                running_process_ = preempted_process_;
                preempted_process_ = -1;
            }
            else{
                if (!regular_queue_.empty()){
                    running_process_ = regular_queue_.front();
                    regular_queue_.pop();
                }
                else{
                    running_process_ = -1;
                }
            }
            running_process_real_time_ = false;
        }
        else{
            running_process_ = rt_queue_.front();
            rt_queue_.pop();
        }
    }
    //if terminated process was regular, run the next regular process
    else{
        if (!regular_queue_.empty()){
            running_process_ = regular_queue_.front();
            regular_queue_.pop();
        }
        else{
            running_process_ = -1;
        }
    }
}

void ReadyQueue::preemptProcess(){
    if (running_process_ == -1){
        return;
    }
    preempted_process_ = running_process_;
    running_process_ = -1;
}

void ReadyQueue::requestDisk(const int disk_num){
    if (disk_num < 0 || disk_num >= disk_queue_.size()){
        cout << "The disk #" << disk_num << " doesn't exist" << endl;
        return;
    }
    if (running_process_ == -1){
        cout << "No processes to request the disk" << endl;
        return;
    }
    disk_queue_[disk_num].requestDisk(running_process_, !running_process_real_time_);
    //process isn't actually terminated and removed from the CPU. it is removed only from the ready-queue
    terminateCurrentProcess();
}

void ReadyQueue::finishDiskUsage(const int disk_num){
    if (disk_num < 0 || disk_num >= disk_queue_.size()){
        cout << "The disk #" << disk_num << " doesn't exist" << endl;
        return;
    }
    ProcessWithType process = disk_queue_[disk_num].endUsage();
    //if process is regular, it runs right away if there are no currently running processes.
    //otherwise it gets pushed to the end of the regular ready-queue
    if (process.PID == -1){
        cout << "No processes were using disk #" << disk_num << endl;
        return;
    }
    if (process.isRegular){
        bool process_can_run = (running_process_ == -1);
        if (process_can_run){
            running_process_ = process.PID;
            running_process_real_time_ = false;
        }
        else{
            regular_queue_.push(process.PID);
        }
    }
    //if process is real-time, it runs immediately only if the CPU is not used by the real-time process and the real-time queue is empty.
    //otherwise it gets put to the end of the real-time ready-queue
    else{
        if (rt_queue_.empty() && !running_process_real_time_){
            if (running_process_ != -1){
                preemptProcess();
            }
            running_process_ = process.PID;
            running_process_real_time_ = true;
        }
        else{
            rt_queue_.push(process.PID);
        }
    }
}

/* ACCESSOR METHODS */

int ReadyQueue::getRunningProcess() const{
    return running_process_;
}

void ReadyQueue::displayRTReadyQueue() const{
    if (rt_queue_.empty()){
        cout << "Real-Time Queue is empty." << endl;
        return;
    }
    cout << "Real-Time Queue: " << endl;
    queue<int> display_queue = rt_queue_;
    cout << "CPU";
    while(!display_queue.empty()){
        cout << " <-- " << "P" << display_queue.front();
        display_queue.pop();
    }
    cout << endl;
}

void ReadyQueue::displayRegularQueue() const{
    if (regular_queue_.empty() && preempted_process_ == -1){
        cout << "Regular Queue is empty." << endl;
        return;
    }
    cout << "Regular Queue: " << endl;
    queue<int> display_queue = regular_queue_;
    cout << "CPU";
    if (preempted_process_ != -1){
        cout << " <-- " << "P" << preempted_process_;
    }
    while(!display_queue.empty()){
        cout << " <-- " << "P" << display_queue.front();
        display_queue.pop();
    }
    cout << endl;
}

void ReadyQueue::displayDiskUsage() const{
    for (int i = 0; i < disk_queue_.size(); i++){
        disk_queue_[i].displayDiskUsage();
    }
}

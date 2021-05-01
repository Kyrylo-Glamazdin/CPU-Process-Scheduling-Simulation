//Kyrylo Glamazdin
//CSCI 340

#include "MemorySimulation.hpp"

/* CONSTRUCTOR */

//initialize everything to default
MemorySimulation::MemorySimulation(const long long RAM_bytes, int num_of_hard_disks) : ready_queue_(num_of_hard_disks){
    RAM_bytes_ = RAM_bytes;
    num_of_hard_disks_ = num_of_hard_disks;
    RAM_bytes_used_ = 0;
    next_PID_ = 1;
    all_processes_.clear();
    EmptyChunk initial_chunk;
    initial_chunk.start_position_ = 0;
    initial_chunk.end_position_ = RAM_bytes - 1;
    free_chunks_.push_back(initial_chunk);
}

/* MUTATOR METHODS */

bool MemorySimulation::addProcess(const ProcessType pt, long long process_size){
    long long first_byte_of_mem = 0;
    long long last_byte_of_mem = 0;
    //use first-fit approach for selecting the free chunk of memory
    for (unsigned i = 0; i < free_chunks_.size(); i++){
        if(process_size <= free_chunks_[i].end_position_ - free_chunks_[i].start_position_ + 1){
            first_byte_of_mem = free_chunks_[i].start_position_;
            last_byte_of_mem = free_chunks_[i].start_position_ + process_size - 1;
            free_chunks_[i].start_position_ = last_byte_of_mem + 1;
            //remove free chunk if it was entirely taken
            if (free_chunks_[i].start_position_ > free_chunks_[i].end_position_){
                free_chunks_.erase(free_chunks_.begin() + i);
            }
            //create a new process and push it to the list of processes
            Process next_p(next_PID_, process_size, pt);
            next_p.setMemoryLocation(first_byte_of_mem, last_byte_of_mem);
            all_processes_.push_back(next_p);
            //add the process to the ready-queue
            ready_queue_.addProcess(next_p);
            next_PID_++;
            RAM_bytes_used_ = RAM_bytes_used_ + process_size;
            return true; //return true if process was added
        }
    }
    return false; //return false if process was not added
}

bool MemorySimulation::terminateCurrentProcess(){
    int PID_to_terminate = ready_queue_.getRunningProcess();
    if (PID_to_terminate == -1){
        cout << "No processes to terminate." << endl;
        return false; //exit if no processes are using the CPU
    }
    //remove the process from the ready-queue
    ready_queue_.terminateCurrentProcess();
    int indexOfProcessToTerminate = findIndexOfProcessById(PID_to_terminate);
    if (indexOfProcessToTerminate == -1){
        return false;
    }
    
    Process to_terminate = all_processes_[indexOfProcessToTerminate];
    pair<long long, long long> mem_position = to_terminate.getMemoryLocation();
    RAM_bytes_used_ = RAM_bytes_used_ - to_terminate.getSize(); //update RAM usage statistics
    all_processes_.erase(all_processes_.begin() + indexOfProcessToTerminate);
    
    //clear RAM from the data occupied by the terminated process
    EmptyChunk freed_chunk;
    freed_chunk.start_position_ = mem_position.first;
    freed_chunk.end_position_ = mem_position.second;
    insertAndUniteEmptyChunks(freed_chunk);
    return true;
}

void MemorySimulation::insertAndUniteEmptyChunks(EmptyChunk e){
    if (free_chunks_.empty()){
        free_chunks_.push_back(e);
        return;
    }
    //if freed chunk is the first empty chunk in memory, insert it at position 0
    if (e.end_position_ < free_chunks_[0].start_position_){
        //unite chunks if adjacent
        if (free_chunks_[0].start_position_ - e.end_position_ == 1){
            free_chunks_[0].start_position_ = e.start_position_;
            return;
        }
        else{
            free_chunks_.insert(free_chunks_.begin(), e);
        }
    }
    //if freed chunk is the last empty chunk in memory, insert it at the last position
    else if (e.start_position_ > free_chunks_[free_chunks_.size() - 1].end_position_){
        //unite chunks if adjacent
        if (e.start_position_ - free_chunks_[free_chunks_.size() - 1].end_position_ == 1){
            free_chunks_[free_chunks_.size() - 1].end_position_ = e.end_position_;
        }
        else{
            free_chunks_.push_back(e);
        }
    }
    //if freed chunk is between the other free chunks, isert it in between the other chunks
    else{
        for (int i = 0; i < free_chunks_.size() - 1; i++){
            //find a place to insert a free chunk
            if (free_chunks_[i].end_position_ < e.start_position_ && free_chunks_[i+1].start_position_ > e.end_position_){
                //unify the chunks if the freed chunk fits perfectly between the other two
                if (e.start_position_ - free_chunks_[i].end_position_ == 1 && free_chunks_[i+1].start_position_ - e.end_position_ == 1){
                    //expand the preceding chunk by the amount of memory taken by the freed and the following chunks and erase the following chunk
                    free_chunks_[i].end_position_ = free_chunks_[i+1].end_position_;
                    free_chunks_.erase(free_chunks_.begin() + i + 1);
                }
                //unite preceding and freed chunk if adjacent
                else if (e.start_position_ - free_chunks_[i].end_position_ == 1){
                    free_chunks_[i].end_position_ = e.end_position_;
                }
                //unite following and freed chunk if adjacent
                else if (free_chunks_[i+1].start_position_ - e.end_position_ == 1){
                    free_chunks_[i+1].start_position_ = e.start_position_;
                }
                //if chunks are completely disjoint
                else{
                    free_chunks_.insert(free_chunks_.begin() + i + 1, e);
                }
            }
        }
    }
}

void MemorySimulation::endTimeSlice(){
    ready_queue_.endTimeSlice();
}

void MemorySimulation::requestDiskUsage(int disk_num){
    ready_queue_.requestDisk(disk_num);
}

void MemorySimulation::endDiskUsage(int disk_num){
    ready_queue_.finishDiskUsage(disk_num);
}

/* ACCESSOR METHODS */

int MemorySimulation::findIndexOfProcessById(int PID) const{
    for (int i = 0; i < all_processes_.size(); i++){
        if (all_processes_[i].getPID() == PID){
            return i;
        }
    }
    return -1;
}

void MemorySimulation::displayQueueInfo() const{
    int running_process = ready_queue_.getRunningProcess();
    if (running_process == -1){
        cout << "CPU is currently idle." << endl;
    }
    else{
        cout << "CPU is used by process " << "P" << running_process << endl;
    }
    
    ready_queue_.displayRTReadyQueue();
    ready_queue_.displayRegularQueue();
}

void MemorySimulation::displayStateOfMemory() const{
    //display general RAM statistics
    cout << endl << "RAM contains " << RAM_bytes_ << " bytes in total." << endl;
    cout << RAM_bytes_used_ << " bytes are in use." << endl;
    cout << RAM_bytes_ - RAM_bytes_used_ << " bytes are idle." << endl << endl;
    
    if (RAM_bytes_used_ == 0){
        cout << "No processes to display." << endl << endl;
        return;
    }
    
    //display each process with its PID, size, first byte of memory that it occupies, last byte of memory that it occupies, and whether it is regular or real-time
    cout << "PID    " << "First byte of memory   " << "Last byte of memory   " << "Process Size       " << "Process type" << endl;
    for (int i = 0; i < all_processes_.size(); i++){
        Process next_process_to_display = all_processes_[i];
        cout << next_process_to_display.getPID() << "              "
        << next_process_to_display.getMemoryLocation().first << "                     "
        << next_process_to_display.getMemoryLocation().second <<  "                "
        << next_process_to_display.getSize() << "                  ";
        if (next_process_to_display.isRegular()){
            cout << "Regular";
        }
        else{
            cout << "Real-Time";
        }
        cout << endl << endl;
    }
}

void MemorySimulation::displayDiskUsage() const{
    ready_queue_.displayDiskUsage();
}

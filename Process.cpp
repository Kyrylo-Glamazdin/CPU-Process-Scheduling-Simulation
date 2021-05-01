//Kyrylo Glamazdin
//CSCI 340

#include "Process.hpp"

/* CONSTRUCTOR */

Process::Process(int PID, long long process_size, ProcessType pt){
    PID_ = PID;
    size_ = process_size;
    pt_ = pt;
    starting_memory_location_ = -1;
    ending_memory_location_ = -1;
}

/* MUTATOR METHODS */

void Process::setMemoryLocation(const long long start, const long long end){
    starting_memory_location_ = start;
    ending_memory_location_ = end;
}

/* ACCESSOR METHODS */

pair<long long, long long> Process::getMemoryLocation() const{
    pair<long long, long long> mem_loc;
    mem_loc.first = starting_memory_location_;
    mem_loc.second = ending_memory_location_;
    return mem_loc;
}

int Process::getPID() const{
    return PID_;
}

long long Process::getSize() const{
    return size_;
}

bool Process::isRegular() const{
    return (pt_ == regular);
}

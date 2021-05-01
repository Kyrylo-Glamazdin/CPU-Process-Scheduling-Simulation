//Kyrylo Glamazdin
//CSCI 340

#include "SimulationDriver.hpp"

void runSimulation(){
    long long RAM_SIZE = getInitialRAMData();
    int NUM_OF_HARD_DISKS = getInitialHDData();
    
    MemorySimulation MEMORY_SIMULATION(RAM_SIZE, NUM_OF_HARD_DISKS);
    
    //continuously listen for user commands
    listenForInput(MEMORY_SIMULATION);
}

long long getInitialRAMData(){
    long long ram_size;
    cout << "Enter the amount of RAM memory in bytes: ";
    cin >> ram_size;
    if (ram_size < 0){
        cout << "The amount of memory cannot be negative." << endl;
        abort();
    }
    else{
        return ram_size;
    }
}

int getInitialHDData(){
    int hard_disk_num;
    cout << "Enter the number of hard disks: ";
    cin >> hard_disk_num;
    if (hard_disk_num < 0){
        cout << "The number of hard disks cannot be negative." << endl;
        abort();
    }
    else{
        return hard_disk_num;
    }
}

void listenForInput(MemorySimulation& m){
    while (true){
        string next_command = "";
        getline(cin, next_command); //receive a user command as a string
        
        if (!trimInput(next_command)){
            if (next_command.size() > 0){
                //run a command decoder for single words
                decodeSingleWord(next_command, m);
            }
        }
        else{
            //run a command decoder for commands with spaces in between
            decodeDoubleWord(next_command, m);
        }
    }
}

bool trimInput(string& s){
    int front_space_counter = 0;
    while (front_space_counter < s.size() && (int)s[front_space_counter] == 32){
        front_space_counter++;
    }
    if (front_space_counter > 0){
        //remove all front spaces
        s = s.substr(front_space_counter, s.size() - front_space_counter);
    }
    
    if (s.size() == 0){
        return false; //return if command is completely empty
    }
    
    int back_space_counter = 0;
    while ((int)s[s.size() - back_space_counter - 1] == 32) {
        back_space_counter++;
    }
    //remove all back spaces
    if (back_space_counter > 0){
        s = s.substr(0, s.size() - back_space_counter);
    }
    
    int j = 0;
    bool middle_space_found = false;
    for (j = 0; j < s.size(); j++){
        if ((int)s[j] == 32){
            middle_space_found = true;
            break;
        }
    }
    
    //return if there's no space in the middle of a command
    if (!middle_space_found){
        return false;
    }
    
    //find all extra middle spaces and remove them
    int initial_middle_space = j;
    while ((int)s[j] == 32){
        j++;
    }
    
    
    
    if (j - initial_middle_space == 1){
        return true;
    }
    
    string first_half = s.substr(0, initial_middle_space);
    string second_half = "";
    while (j < s.size()){
        second_half = second_half + s[j];
        j++;
    }
    s = first_half + " " + second_half;
    
    int final_space_counter = 0;
    for (int k = 0; k < s.size(); k++){
        if ((int)s[k] == 32){
            final_space_counter++;
        }
    }
    return true;
}

void decodeSingleWord(string w, MemorySimulation& m){
    //end time slice for the current process
    if (w == "Q"){
        m.endTimeSlice();
        return;
    }
    //terminate current process
    else if (w == "t"){
        m.terminateCurrentProcess();
        return;
    }
    else {
        cout << "Command " << w << " doesn't exist. Please enter a valid command." << endl;
    }
}

void decodeDoubleWord(string w, MemorySimulation& m){
    //counts the number of separate spaces in the command.
    //in case a user enter a command with 2 or more separate middle spaces (ex. "A 1 000 000"), the command is ignored
    int separate_space_counter = 0;
    for (int i = 0; i < w.size(); i++){
        if ((int)w[i] == 32){
            separate_space_counter++;
        }
    }
    if (separate_space_counter > 1){
        cout << "Command " << w << " doesn't exist. Please enter a valid command." << endl;
        return;
    }
    
    int space_index = 0;
    string first_half = "";
    string second_half = "";
    for (int i = 0; i < w.size(); i++){
        if ((int)w[i] == 32){
            space_index = i;
            break;
        }
    }
    first_half = w.substr(0, space_index);
    second_half = w.substr(space_index + 1, w.size() - 1 - space_index);
    
    //create a regular process
    if (first_half == "A"){
        long long process_size = stoll(second_half);
        if (process_size > 0){
            bool success = m.addProcess(regular, process_size);
            if (!success){
                cout << "The process you're trying to add is too large" << endl;
            }
        }
        else{
            cout << "Process must have a positive size" << endl;
        }
    }
    //create a real-time process
    else if (first_half == "AR"){
        long long process_size = stoll(second_half);
        if (process_size > 0){
            bool success = m.addProcess(real_time, process_size);
            if (!success){
                cout << "The process you're trying to add is too large" << endl;
            }
        }
        else{
            cout << "Process must have a positive size" << endl;
        }
    }
    //request disk usage
    else if (first_half == "d"){
        int disk_num = stoi(second_half);
        m.requestDiskUsage(disk_num);
    }
    //finish using the disk
    else if (first_half == "D"){
        int disk_num = stoi(second_half);
        m.endDiskUsage(disk_num);
    }
    else if (first_half == "S"){
        if (second_half == "r"){
            m.displayQueueInfo();
        }
        else if (second_half == "i"){
            m.displayDiskUsage();
        }
        else if (second_half == "m"){
            m.displayStateOfMemory();
        }
        else{
            cout << "Command " << w << " doesn't exist. Please enter a valid command." << endl;
        }
    }
    else {
        cout << "Command " << w << " doesn't exist. Please enter a valid command." << endl;
    }
}

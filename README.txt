# Introduction
This project simulates some aspects of operating systems such as CPU process scheduling, as well as RAM and HDD management.

# CPU Scheduling
* There are 2 levels of ready-queue: 1 for real-time processes, and 1 for common processes.
* Each level uses round-robin as a schedulig algorithm: every process has an equal time slice to be executed before being put in the back of the circular queue.
* Common processes can be executed only when real-time queue is empty.
* When real-time process arrives during the execution of a common process, the common process gets preempted and put in the head of the common queue.
* All processes have a fixed size specified by the user.

# RAM & I/O Queue Management
* RAM is simulated using a contiguous memory management with First-Fit approach.
* The size of RAM must be specified by the user right after running the project. The maximum RAM size is 4,000,000,000 bytes.
* All I/O queues are First-Come-First-Serve (FCFS).
* The number of Hard Disks is also specified by the user. The enumeration of hard disks starts with 0.

# Installation
Clone this repo, then compile by typing in your terminal:

`
make all
`

OR, to compile manually, type:

`
g++ -std=c++11 main.cpp Process.cpp ReadyQueue.cpp MemorySimulation.cpp DiskQueue.cpp SimulationDriver.cpp
`

To run, type:

`
./a.out
`

To remove executables, type:

`
make clean
`

# Running The Simulation
The simulation is controlled entirely by the user input through the terminal. The state of the simulation can be altered by the commands specified below:
* First, install and run the simulation (check **Installation** section)
* You will be prompted to enter the number of bytes you want your RAM to have. Enter any number up to 4 Billion.
* You then will be prompted to enter a number of hard disks you want your system to have. (Optimal number is in range 2-5).

##
Now, the simulation begins. The program constantly listens for new user inputs. The list of such inputs is (case sensitive):
* **A size** Create new common process with the size of **size** bytes (can be as large as the entire RAM). After the process is created, it is assigned a unique Process ID (PID) and gets put in the ready-queue or the CPU (depending on CPU availability). Example: **A 1000** creats a new common process of size 1000 bytes.
* **AR size** Create a new real-time process with the size of **size** bytes. If the CPU is not used by another real-time process, the created process is given access to the CPU. Otherwise, it gets put into the real-time queue. Example: **AR 500** creats a new common process of size 500 bytes.
* **Q** End time slice for the currently runinng process.
* **t** Terminate currently running process.
* **d number** Currently running process requests access to Hard Disk. Remove it from the CPU and from the ready-queue, and give access to Hard Disk **number** (or if that disk is in use, put the process in the I/O queue). Example: **d 0** takes currently running process and gives it access to the first Hard Disk (disk 0).
* **D number** Hard Disk **number** finished the work for 1 process. After that, the process gets put back into the ready-queue.

## At any point, you can check the current state of the simulation by any of the following commands:
* **S r** Shows what process is currently using the CPU, and what processes are currently waiting in both levels of the ready-queue.
* **S i** Shows the state of each Hard Disk (the process that currently uses the disk and the processes waiting in I/O queue).
* **S m** Shows the state of memory. Lists each process and its properties such as: PID, first and last bytes of memory it occupies, its size and its type.

# Example
Here's one possible sequence of commands a user can enter to test the simulation:
* Make sure the RAM size is 1000 and the number of hard disks is 3.
## 
* A 100
* A 150
* S r (CPU is used by P1, ready-queue contains P2)
* AR 300
* S r (CPU is used by P3, ready-queue contains P1 and P3)
* t
* d 0
* A 250
* S r (CPU is used by P2, ready-queue contains P4)
* S i (Disk 0 is used by P1, I/O queue for Disk 0 is empty. All other disks are idle)
* D 0
* Q
* S r (CPU is used by P4, ready-queue contains P1 and P2)
* S i (All Hard Disks are idle)
* S m (The list of properties of all processes introduced but not yet terminated)
  
  ##
  Created for Operating Systems class in CUNY Hunter College.

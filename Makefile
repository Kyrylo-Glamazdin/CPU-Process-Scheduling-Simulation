C++FLAG = -std=c++11

all:
	g++ $(C++FLAG) main.cpp Process.cpp ReadyQueue.cpp MemorySimulation.cpp DiskQueue.cpp SimulationDriver.cpp

clean:
	(rm -f *.o; rm -f a.out)
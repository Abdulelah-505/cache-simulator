#ifndef _MAIN_MEMORY_H
#define _MAIN_MEMORY_H


//1024 words of memory starting at address 0x003f7f00 in blackboard
#define STARTING_ADDRESS 0x003f7f00
#define DEBUG true
#define DIRTY true

#include<iostream>

using namespace std;

class MainMemory{
	private:
		int *memory;
		int capacity;
	public: 
		MainMemory();
		~MainMemory();
		int read(int address);
		bool write(int address, int data);
		void print();
};

#endif


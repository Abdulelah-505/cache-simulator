#include <iostream>
#include <iomanip>
#include <new>
#include <cmath>

#include "main_memory.h"


using namespace std;


MainMemory::MainMemory(){
	cout << "CONSTRUCTING MainMemory obj: " << this << endl;
	capacity = 16 * pow(2, 20)  / 4;	// 16 MB to 32 word
	memory = new (nothrow) int [capacity];
	 cout << "main mem capacity: " << capacity << " words" << endl;
	if (memory==0){
		cout << "Failed to allocate memory!\n";
	}
	
	for(int i = 0; i < capacity; ++i){
		memory[i] = i;
	}
}

MainMemory::~MainMemory(){
    if(memory != NULL){
		delete [] memory;
    }
	 cout << "DESTRUCTED MainMemory obj: " << this << endl;
}

int MainMemory::read (int address){
	int erorr = 400;
	if(address > -1 && address < capacity && memory != NULL){
		return memory[address];
	}else{
		cout << "Invalid memory location or memory NULL" << endl;
	}
	return erorr;
}

bool MainMemory::write(int address, int data){
	if(address > -1 && address < capacity && memory != NULL){
		memory[(unsigned)address] = data;
		
		return true;		// if write
	}else{
		cout << "Invalid memory location or memory NULL" << endl;
		 cout << "memory[" << address << "] is " << memory << endl;
		 cout << "cap: " << capacity << endl;
		
		return false;
	}
}

void MainMemory::print(){
	cout << "M M:" << endl;
	cout << "Address    Words" << endl;
	
	int to =  STARTING_ADDRESS + 1023;
	for(int i =  STARTING_ADDRESS; i < to; i=i+8){
		cout << setw(8) << setfill('0') << hex << i;
		for(int j = 0; j < 8; j++)
			cout << "   " << setw(8) << setfill('0') << memory[i+j];
		
		cout << endl;
	}
}

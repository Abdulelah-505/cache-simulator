
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <cmath>
#include "funcs.h"
#include "main_memory.h"
#include "cache_memory.h"

using namespace std;

#define CACHE_READ 0
#define CACHE_WRITE 1 




void run(int, int, int);

int main (int argc, char *argv[ ]){
	
	int cache_capacity = 0;
	int cache_blocksize = 0;
	int cache_associativity = 0;

	// sets params and checks format
	if(!parseParams(argc, argv, cache_capacity, cache_blocksize, cache_associativity) ) {
		exit (2);
	}
	
	if(DEBUG) cout << "Cache Capacity: " << cache_capacity << " Kb" << endl;
	if(DEBUG) cout << "Cache BlockSize: " << cache_blocksize << " bytes" << endl;
	if(DEBUG) cout << "Cache Associativity: " << cache_associativity << " set" << endl;
	
	run(cache_associativity, cache_blocksize, cache_capacity);
	
	return 0;
}
void run(int cache_associativity, int cache_blocksize, int cache_capacity){
	cout << "Allllllllllahhhhhhh Akkkkkkkbrr" << endl;
	
	CacheMemory cache(cache_associativity, cache_blocksize, cache_capacity);

	// Input
	int read_write;
	int address;
	unsigned int data;
	cout << "Strat Input" << endl;
	// repeat till we reach the end of the input	
	while(!feof(stdin)){
	  	//read in whether to read or write to the cache
		cin >> dec >> read_write;
		// check again if we have reached the end
		// as this flag is set only after a 'cin'
		if(feof(stdin)) break;
		cin >> hex >> address;
		//if it is a cache write, then we have to read the data
		if(read_write == CACHE_WRITE){
		  	cin >> hex >> data;
			if(DEBUG) cout << "Write memory 0x" << data << endl;
			cache.write(address, data);
		}else{		//read
			int x = cache.read(address);
			if(DEBUG) cout << "Read memory 0x" << x << endl;
		}
	}
	// write all dirty lines to memory
	cache.writeDirtyBlocks();
	cout << "End of input.\n";
	cache.print();
	cout << "Allllllllllahhhhhhh Akkkkkkkbrr" << endl;
}

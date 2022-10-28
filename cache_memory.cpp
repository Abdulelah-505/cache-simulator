#include <iostream>
#include <iomanip>
#include <new>
#include <cmath>

#include "cache_memory.h"

using namespace std;

int Set::associativity = 0;
int Set::blockSize = 0;
int CacheLine::blockSize = 0;


CacheMemory::CacheMemory(int assoc, int bSize, int cap){
	cout << "CONSTRUCTING CacheMemory obj: " << this << endl;
	this->capacity = cap * pow(2, 10) / 4;		// convert KiB to words
	mem = new MainMemory;		
	associativity = assoc;
	blockSize = bSize / 4;		// convert from bytes to words
	hits = misses = writes = reads = totalDirty = totalReads = totalWrites = 0;
	numSets = capacity / blockSize / associativity;
	

	cout << "cache capacity: " << capacity << " words" << endl;
	cout << "block size: " << blockSize << " words" << endl;
	cout << "number of sets: " << numSets << " sets" << endl;
	setBits = (float)log(numSets)/log(2);
	wordOffsetBits = (float)log(blockSize)/log(2);
	tagBits = 32 - setBits - wordOffsetBits;

	cout << "bits for set index: " << setBits << " bits" << endl;
	cout << "bits for word index: " << wordOffsetBits << " bits" << endl;
	cout << "bits for tag: " << tagBits << " bits" << endl;

	// build masks
	wMask = ( 1 << wordOffsetBits ) - 1;
	sMask = (( 1 << setBits ) - 1) << wordOffsetBits;
	tMask = (( 1 << tagBits ) - 1) << (32 - tagBits);
		
	cout << "word mask: 0x" << hex << wMask << "" << endl;
	cout << "set mask: 0x" << sMask << "" << endl;
	cout << "tag mask: 0x" << tMask << "" << endl;

	
	Set::associativity = associativity;
	Set::blockSize = blockSize;
	sets = new Set [numSets];
}


CacheMemory::~CacheMemory(){
    if(sets != NULL){
		delete [] sets;
    }
}

int CacheMemory::read (unsigned address){
	unsigned wordIdx;
	unsigned set;
	unsigned tag;
	bool found = false;
	int data = -1;
	++totalReads;
	
	parseAddress(address, wordIdx, set, tag);		
	data = sets[set].read(tag, wordIdx, found);	

	if(found){
		data = sets[set].read(tag, wordIdx, found);		
		++hits;
	}else{		//if not found in cache
		++reads;
		data = mem->read(address);
	
		if(sets[set].line[sets[set].LRU].dirty){
			int start = spliceAddress(set, sets[set].line[sets[set].LRU].tag);
			for(int i = 0; i < blockSize; i++)
				mem->write(start + i, sets[set].line[sets[set].LRU].word[i]);
			totalDirty++;
		}
		
		int start = address - (address % blockSize);		// start at first word in block
		for(int i = 0; i < blockSize; i++)
			
			sets[set].line[sets[set].LRU].word[i] = mem->read(start + i);
		
		sets[set].line[sets[set].LRU].tag = tag;
		sets[set].line[sets[set].LRU].valid = true;
		sets[set].line[sets[set].LRU].dirty = false;
		sets[set].updateLRU();
		++misses;
	}
	
	sets[set].print(set);
	
	return data;
}

void CacheMemory::write (unsigned address, int data){
	unsigned wordIdx;
	unsigned set;
	unsigned tag;
	bool found = false;
	++totalWrites;

	parseAddress(address, wordIdx, set, tag);		// parameters passed by reference
	sets[set].write(data, tag, wordIdx, found);

	if(!found)
		{
			++writes;
			if(sets[set].line[sets[set].LRU].dirty)
			{
				int start = spliceAddress(set, sets[set].line[sets[set].LRU].tag);
				for(int i = 0; i < blockSize; i++)
				{
					mem->write(start + i, sets[set].line[sets[set].LRU].word[i]);
				}
				++totalDirty;
			}
			int start = address - (address % blockSize);
			for(int i = 0; i < blockSize; i++)
			{
				sets[set].line[sets[set].LRU].word[i] = mem->read(start + i);
			}
			sets[set].line[sets[set].LRU].tag = tag;
			sets[set].line[sets[set].LRU].valid = true;
			sets[set].line[sets[set].LRU].word[address % blockSize] = data; // modify it with the write
			sets[set].line[sets[set].LRU].dirty = true; // then mark it as dirty
			sets[set].updateLRU();
		
			
		}
	else
		 {
			for(int i = 0; i < associativity; ++i)
		   {
				if(sets[set].line[i].tag == tag){
					sets[set].line[i].word[address%blockSize] = data;
					sets[set].line[i].dirty = true;
					break;
				}
			}
		 }
		//sets[set].line[] = data;		
		;
	
	sets[set].print(set);
}


void CacheMemory::parseAddress (const unsigned address, unsigned &wordIdx, unsigned &set, unsigned &tag){
	wordIdx = address & wMask;
	set = (address & sMask) >> wordOffsetBits;	
	tag = (address & tMask) >> (32 - tagBits);
}

void CacheMemory::print(){
	cout << "STATISTICS:" << dec << endl;
	cout << "Misses:" << endl;
	cout << "Total: " << writes + reads;
	cout << " DataReads: " << reads;
	cout << " DataWrites: " << writes << endl;
	cout << "Miss rate: " << endl;
	// To avoid division by zero
	if(totalReads + totalWrites == 0)
		cout << "Total: 0";
	else
		cout << "Total: " << (float(reads + writes) / float(totalReads + totalWrites))*100<<" %";
	if(totalReads == 0)
		cout << " Data Reads: 0";
	else
		cout << " Data Reads: " << (float(reads) / float(totalReads))*100<<" %";
	if(totalWrites == 0)
		cout << " Data Writes: 0" <<  endl;
	else
		cout << " Data Writes: " << (float(writes) / float(totalWrites))*100 <<" %"<< endl;
	cout << "Total Dirty Blocks : " << totalDirty << endl << endl;
	
	cout << "CACHE CONTENTS" << endl;
	//assert -1 < format < 3
	cout << "Set   V    Tag    Dirty    ";
	for(int i=0; i<blockSize; ++i) cout << "Word" << i << "      ";
	
	cout << endl;
	
	for(int set = 0; set < numSets; set++)
		sets[set].print(set);
	cout << endl;
	//print mem
	mem->print();
}

void CacheMemory::writeDirtyBlocks (){
	unsigned address = -1;
	
	for(int s = 0; s < numSets; ++s)
		for(int l = 0; l < associativity; ++l)		// check all cacheLines in set
			if(sets[s].line[l].dirty == DIRTY){
				address = spliceAddress(s, sets[s].line[l].tag);
				for(int w = 0; w < blockSize; ++w)		//write block
					mem->write(address + w, sets[s].line[l].word[w]);
			}
}

unsigned CacheMemory::spliceAddress (unsigned set, unsigned tag){	
	unsigned address = 0x00;		
	unsigned temp = 0x00;
	
	address = (address | tag) << (32 - tagBits);
	temp = (set | temp) << wordOffsetBits;
	address |= temp;
	
	return address;	
}

// Set methods
Set::Set() {
	CacheLine::blockSize = blockSize;
	line = new CacheLine [associativity];	
	LRU = 0;
}

int Set::read(unsigned tag, unsigned wordIdx, bool &found){
	for(int i = 0; i < associativity; ++i)
		if(line[i].tag == tag && line[i].valid){
			
			found = true;
			return line[i].word[wordIdx];		// requested word found
		}

	found = false;
	return 0;		// word not found
}

void Set::write(int data, unsigned tag, unsigned wordIdx, bool &found){
	for(int i = 0; i < associativity; ++i){
		if(line[i].tag == tag && line[i].valid){
			found = true;
			line[i].word[wordIdx] = data;		// requested word found
			return;
		}
	}
	
	found = false;		// not found in cache
}

void Set::print(int set){
	for(int l = 0; l < associativity; ++l){
		cout << hex << set << "     ";
		line[l].print();
	}
}

void Set::updateLRU(){
	LRU = (LRU + 1) % associativity;
}

//CacheLine methods
CacheLine::CacheLine(){
	valid = false;
	tag = 0;
	
	word = new int [blockSize];
	for(int i = 0; i < blockSize; ++i)
		word[i] = 0;
}

void CacheLine::print(){
	cout << valid << "   " << setw(8) << setfill('0') << hex << tag << "    " << dirty << " ";
	// print words
	for(int i = 0; i < blockSize; ++i)
		cout << "   " << setw(8) << setfill('0') << word[i];
	
	cout << endl;
}

#include<iostream>
#include <fstream>
#include"global_var.h"
#include"read_var.h"
#include"write_var.h"
#include<map>
#include<cmath>
#include<unordered_map>
using namespace std;


string Bin2Hex(string binary);

class node
{
    public:
    node *parent;
    node *left;
    node *right;
    bool bits;
    string tags;

    node()
    {
        this->bits = false;
        left = nullptr;
        right = nullptr;
    }
    node(string tags)
    {
        this->tags = tags;
        left = nullptr;
        right = nullptr;
    }

};





class Blocks{
    public:
    string tags;
    int LRU_counter;
    bool isDirty;
    node* parent;

    Blocks(string tags , bool isDirty = false)
    {
        this->tags = tags;
        this->LRU_counter = counter;
        counter++;
        this->isDirty = isDirty;

    }
};


class set
{
    public:
    list<Blocks> blocks;
    set(){};
};


class cache_sim
{
    public:
    int number;
    int associativity;
    int size = 0;
    map<string , set>taple; //طباعتها بالترتيب tag and index

    cache_sim(){}
    cache_sim(int number){
        this->number = number;
    }
    void values_of_set(int size , int associativity)
    {
        this-> size = size;
        this->associativity = associativity;
    }
};


void Tag(cache_sim &List , string index , string tags , string line);
void Index(cache_sim &List , string index , string tags , string line);
void repl(cache_sim &List , string tags , list<Blocks>*tagsList , string line);


enum command
{
    R, W
};



char* traceFileName = "";

cache_sim cache = cache_sim(1);


command state;

int index_bits = log2(sets_number);
int offsets_bits = log2(block_size);
int tags_bits = 32 - index_bits - offsets_bits;


void testInputs() {
	block_size = 16;
	cache_capacity = 1024;
	associativity = 2;
	cache.values_of_set(cache_capacity, associativity);
	replacement = 0;
	inclusion = 0;
	traceFileName = "test.txt"; 

	sets_number = cache_capacity / (associativity * block_size);

	index_bits = log2(sets_number); 
	offsets_bits = log2(block_size); 
	tags_bits = 32 - index_bits - offsets_bits; 
}

string Hex2Binary(string hex) {
    
	long int i = 0;
	string binary = "";

    while (hex[i]) {
 
        switch (hex[i]) {
        case '0':
            binary += "0000";
            break;
        case '1':
            binary += "0001";
            break;
        case '2':
            binary += "0010";
            break;
        case '3':
            binary += "0011";
            break;
        case '4':
            binary += "0100";
            break;
        case '5':
            binary += "0101";
            break;
        case '6':
            binary += "0110";
            break;
        case '7':
            binary += "0111";
            break;
        case '8':
            binary += "1000";
            break;
        case '9':
            binary += "1001";
            break;
        case 'A':
        case 'a':
            binary += "1010";
            break;
        case 'B':
        case 'b':
            binary += "1011";
            break;
        case 'C':
        case 'c':
            binary += "1100";
            break;
        case 'D':
        case 'd':
            binary += "1101";
            break;
        case 'E':
        case 'e':
            binary += "1110";
            break;
        case 'F':
        case 'f':
            binary += "1111";
            break;
        default:
            cout << "Error in hex digit " << hex[i]<<'\t' << binary<<endl;

        }
        i++;
    }


	return binary;
}

bool CorrectInputs(int argc, char *argv[]) {

	block_size = atoi(argv[1]);
	cache_capacity = atoi(argv[2]);
	associativity = atoi(argv[3]);
	replacement = atoi(argv[6]);
	inclusion = atoi(argv[7]);
	traceFileName = argv[8];

	sets_number = cache_capacity / associativity * block_size;


	return true;
}

string expand(string hex) {

	if (hex.size() < 8) {
		hex = "0" + hex;
		return expand(hex);
	}
	return hex;
}

string condense(string hex) {

	if (hex[0] == '0') {
		hex = hex.substr(1);
		return condense(hex);
	}
	return hex;
}

void Print(cache_sim cache) {

	cout << "===== Simulator information =====" << endl;
	cout << "BLOCKSIZE:             " << block_size << endl;
	cout << "cache_SIZE:               " << cache_capacity << endl;
	cout << "associativity:              " << associativity << endl;

	cout << "REPLACEMENT :    ";
	
		switch (replacement) {
			case 0:
				cout << "LRU" << endl;
				break;
			
		}

	
	cout << "INCLUSION PROPERTY:    ";
	
		switch (inclusion) {
			case 0:
				cout << "non-inclusive" << endl;
				break;
			case 1:
				cout << "inclusive" << endl;
				break;
		}
	
	cout << "TEST File Name:            " << traceFileName << endl;
	
	if (cache_capacity != 0) {
		cout << "===== cache contents =====" << endl;

		map<string, set>::iterator it;
		int count = 0;
		for (it = cache.taple.begin(); it != cache.taple.end(); it++)
		{
			cout << "Set     " << count << ":";
			count++;

			list<Blocks> temp = it->second.blocks;
			list<Blocks>::iterator it2;
			for (it2 = temp.begin(); it2 != temp.end(); it2++) {
				
				cout << "\t" << condense(Bin2Hex(it2->tags)) << " ";
				
				if (it2->isDirty) {
					cout << "D";
				}
				else {
					cout << " ";
				}
				if (condense(Bin2Hex(it2->tags)).size() < 6) {
					cout << "\t";
				} 
			}
			
			cout << " \t";
			cout << endl;
		}

		miss_rate = (float)(number_or_reads_misses + number_write_missess) / (float)(number_or_reads + number_or_writes);
	}




		Memory_Traffic = number_or_reads_misses + number_write_missess + number_write_back;
	

	cout << "===== Simulation results (raw) =====" << endl;
	cout << "a. number of L1 reads:        " << number_or_reads << endl;
	cout << "b. number of L1 read misses:  " << number_or_reads_misses << endl;
	cout << "c. number of L1 writes:       " << number_or_writes << endl;
	cout << "d. number of L1 write misses: " << number_write_missess << endl;
	cout << "e. L1 miss rate:              ";
	printf("%0.6lf", miss_rate);
	cout << endl;
	cout << "f. number of L1 writebacks:   " << number_write_back << endl;
	cout << "m. total memory traffic:      " << Memory_Traffic << endl;

}

string tag1_prev = "";
string tag2_prev = "";


void createMap(unordered_map<string, char> *um)
{
    (*um)["0000"] = '0';
    (*um)["0001"] = '1';
    (*um)["0010"] = '2';
    (*um)["0011"] = '3';
    (*um)["0100"] = '4';
    (*um)["0101"] = '5';
    (*um)["0110"] = '6';
    (*um)["0111"] = '7';
    (*um)["1000"] = '8';
    (*um)["1001"] = '9';
    (*um)["1010"] = 'a';
    (*um)["1011"] = 'b';
    (*um)["1100"] = 'c';
    (*um)["1101"] = 'd';
    (*um)["1110"] = 'e';
    (*um)["1111"] = 'f';
}

string Bin2Hex(string bin) {
    int BinSize = bin.size();
    int findDot = bin.find_first_of('.');
     
    int len_left = findDot != -1 ? findDot : BinSize;
     
    for (int i = 1; i <= (4 - len_left % 4) % 4; i++) {
        bin = '0' + bin;
    }
       
    if (findDot != -1) {
        int len_right = BinSize - len_left - 1;
         
        for (int i = 1; i <= (4 - len_right % 4) % 4; i++) {
            bin = bin + '0';
        }
    }
 unordered_map<string, char> bin_hex_map;
    createMap(&bin_hex_map);
     
    int i = 0;
    string hex = "";
     
    while (1) {
        hex += bin_hex_map[bin.substr(i, 4)];
        i += 4;
        if (i == bin.size()) {
            break;
        }
             
        if (bin.at(i) == '.') {
            hex += '.';
            i++;
        }
    }
     
    // required hexadecimal number
    return hex;   
}

void repl(cache_sim &L, string index, string tag, list<Blocks> *tagsList, string line) {

	list<Blocks>::iterator toReplace = tagsList->begin();

	if (replacement == 0) {
		list<Blocks>::iterator itFind;

		for (itFind = tagsList->begin(); itFind != tagsList->end(); itFind++) { //find LRU and replace
			if (itFind->LRU_counter < toReplace->LRU_counter) {
				toReplace = itFind;
			}
		}

	}




	Blocks &blk(*toReplace);

	if ((line.find("r") != std::string::npos) || (line.find("R") != std::string::npos))  {
		blk = Blocks(tag, false); //CORRECT
	}
	else if ((line.find("w") != std::string::npos) || (line.find("W") != std::string::npos)) {
		blk = Blocks(tag, true); //CORRECT
	}

}

void replaceSame(cache_sim &L, string index, string tag, list<Blocks> *tagsList, string line) {

	list<Blocks>::iterator toReplace = tagsList->begin();
	
	if (replacement == 0) {

		list<Blocks>::iterator itFind;

		for (itFind = tagsList->begin(); itFind != tagsList->end(); itFind++) { //find LRU and replace
			if (itFind->tags == tag) {
				toReplace = itFind;
			}
		}

	if (toReplace->isDirty == true) {

			number_write_back++;
		}
		else if (L.number == 2){
			//WRITE TO MAIN MEMORY
			number_write_back++;
		}

	}


	Blocks &blk(*toReplace);
	blk = Blocks(tag, true);	//CORRECT counter updated
}
void findTag (cache_sim &L, string index, string tag, string line) {

	list<Blocks>* tagsList = &L.taple[index].blocks; //grab already existing tag list

	bool tagHit = false;
	list<Blocks>::iterator it;

	for (it = tagsList->begin(); it != tagsList->end(); ++it) { //check if tag exists 
   		if (it->tags == tag) {
			tagHit = true;
			break;
		}					
	}
			
	if(tagHit) { //if tag was found
		if (state == R) {
			it->LRU_counter = counter; //refresh LRU //CORRECTT
			counter++;
		}
		else if (state == W) {
			repl(L, index, tag, tagsList, line);	
				
		}
	}
	else { //if tag was not found

		if ((line.find("r") != std::string::npos) || (line.find("R") != std::string::npos)) {
			if (L.number == 1) {
				number_or_reads_misses++;
			}
		}
		else if ((line.find("w") != std::string::npos) || (line.find("W") != std::string::npos)) { 
			if (L.number == 1) {
				number_write_missess++;
			}
		}

		if (tagsList->size() < L.associativity) {
			if ((line.find("r") != std::string::npos) || (line.find("R") != std::string::npos)) {
				tagsList->push_back(Blocks(tag, false));
			}
			else if ((line.find("w") != std::string::npos) || (line.find("W") != std::string::npos)) { 
				tagsList->push_back(Blocks(tag, true));
			}
		}
		else { //REPLACE
			repl(L, index, tag, tagsList, line);
		}			
	}			
}
void findIndex (cache_sim &L, string index, string tag, string line) {

	if (L.taple.find(index) == L.taple.end()) { //if index doesnt exist //TODO: THERE NEEDS TO BE A CAP I THINK

		if (state == R) {
			if (L.number == 1) {
				number_or_reads_misses++;
			}

		}
		else if (state == W) {			
			if (L.number == 1) {
				number_write_missess++;
			}

		}	

		list<Blocks> blocks;
		blocks.push_back(Blocks(tag)); //create list for blocks and add current tag
				
		set Set = set();
		L.taple[index] = Set;
		L.taple[index].blocks = blocks;
		
	}
			
	else { //if index found
		findTag(L, index, tag, line);
	}

}





int main(int argc, char *argv[]) {

	testInputs(); 
	string line;
	ifstream trace_file(traceFileName);

	if (trace_file.is_open()) {
		while (getline(trace_file, line)) {
			trace_file_list.push_back(line);
		}
		trace_file.close();
	}
	else {
		cout << "Cant find file!!" << endl;
		return 1;
	}


	list<string>::iterator it;
	for (it = trace_file_list.begin(); it != trace_file_list.end(); it++){

		string command = it->substr(0, 1);
		string address = Hex2Binary(expand(it->substr(it->find(" ") + 1)));
		
		//L1
		string L1tag = address.substr(0, tags_bits);
		string L1index = address.substr(tags_bits, index_bits);
		string L1blockOffset = address.substr(tags_bits + index_bits);

		if ((it->find("w") != std::string::npos) || (it->find("W") != std::string::npos)) {
			state = W;
			number_or_writes++;
		}
		if ((it->find("r") != std::string::npos) || (it->find("R") != std::string::npos)) {
			state = R;
			number_or_reads++;
		}
		
		findIndex(cache, L1index, L1tag, it->c_str());
		

	}

	Print(cache);


	return 0;
}

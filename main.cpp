#include<iostream>
#include<global_var.h>
#include<read_var.h>

#include<map>
#include<cmath>
using namespace std;


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


// https://people.computing.clemson.edu/~mark/464/p_lru.txt


class p_lru{
    public:
    node* root;

    void add()
    {
        node* temproary = root;
        while (true)
        {
            /* code */
            if (temproary->bits == false)
            {
                /* code */
                temproary = temproary->left;
            }
            if (temproary->bits==true)
            {
                /* code */
                temproary = temproary->right;
            }   
        }
    }
    void replace(){};
    p_lru(){}
};


class Blocks{
    public:
    string tags;
    int LRU_counter = 0;
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
    p_lru t;
    set(){};
};


class cache_sim
{
    public:
    int number;
    int associativity;
    int size = 0 ;
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


enum flag
{
    R, W
};


cache_sim cache= cache_sim(1);


flag state;

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
	file_name = "gcc_trace.txt"; 

	sets_number = cache_capacity / (associativity * block_size);

	index_bits = log2(sets_number); 
	offsets_bits = log2(block_size); 
	tags_bits = 32 - index_bits - offsets_bits; 
}
string Hex_To_Binary(string hex) {
    
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
            cout << "Error in hex digit " << hex[i];
        }
        i++;
    }


	return binary;
}

bool Correct_Inputs(int argc, char *argv[]) {

	block_size = atoi(argv[1]);
	cache_capacity = atoi(argv[2]);
	associativity = atoi(argv[3]);
	replacement = atoi(argv[6]);
	inclusion = atoi(argv[7]);
	file_name = argv[8];

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
	
	cout << "TEST File Name:            " << file_name << endl;
	
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
				
				cout << "\t" << condense(Binary_to_Hex(it2->tags)) << " ";
				
				if (it2->isDirty) {
					cout << "D";
				}
				else {
					cout << " ";
				}
				if (condense(Binary_to_Hex(it2->tags)).size() < 6) {
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







int main()
{



	return 0;



}

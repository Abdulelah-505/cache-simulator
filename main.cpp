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








int main()
{



	return 0;



}
#include <cmath>
#include <iostream>
#include "fully.h"
#include "random_gen.h"
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>



int cache_size = 0; // size 
int cache_block_size = 0;  // block size 
int number_of_block = 0; // number of blocks
int cache_type; // Direct or set or fully
int cache_misses = 0; // counter

char *msg[2] = {"Miss","Hit"};



using namespace std;

bool cache_simulation(unsigned int address, int cache[3][100000], int type_cache, int &block_counter, int index_address, int tag_address)
{

    int shift_offset=log2(cache_block_size);
    bool detected=false;
    bool misses_flag=true;


    // Direct Mapped     
    if (cache_type==0)    
    {
        if (cache[0][index_address]==tag_address)
        {
            return true;
        }
        else
            {
                cache[0][index_address]= tag_address;
                for (int i=0; i < number_of_block; i++)
                {
                    if (cache[1][i]!=1)
                    {   misses_flag=false;
                        i=number_of_block;}
                    
                }
                //calculating misses
                if (misses_flag)
                    cache_misses++;   // capcity miss because the cash is full
                else
                {
                    if(cache[1][index_address]==1)
                        cache_misses++;
                    else
                    {
                        cache_misses++;
                    }
                }
                cache[1][index_address]= 1;
                return 0;
            }
    } // end of directed mapped

}


int main ()
{
    int looper=10000, addr, flag, shift;
    int cash[3][100000];
    int block_counter=0;
    double hit_counter=0;
    double miss_counter = 0 ;
    int index_addr=0, tag_addr=0;
    double ratio_hit;
    double ratio_miss;
  //the values that we get from the 
  //command line will be put into these
  //variables. we can use them later in the program
  //like for checking if they have valid values


    cout << "Enter 0 for Direct mapping, 1 for set associative, 2 for fully associative: " << endl;
    cin >> cache_type;
    cout << "Enter the size of the block as a Power of 2 between 4 and 128 byte  :" << endl;
    cin >> cache_block_size;
    cout << "Enter cache size: 1KB – 64KB; in steps that are power of 2: " << endl;
    cin >> cache_size;

    if ( cache_type==0)   //Direct_mapped **************
    {
        number_of_block= (cache_size*1024)/cache_block_size;
        
        ////////////////////
        for (int i=0; i < 2; i++)   // setting all the cash with -1
            for (int j=0; j < number_of_block; j++)
                cash[i][j]=-1;
        //////////////////
        
        for(int i=0; i <looper ;i++)
        {
            addr = memGen1();
            shift= log2(cache_block_size);
            index_addr= (addr >> shift)% number_of_block;
            shift= log2(number_of_block+cache_block_size);
            tag_addr= addr >>shift;    // shifted the amount the offset and the index sizes
            flag = cache_simulation(addr, cash, 0,block_counter, index_addr, tag_addr);
            index_addr=0;
            tag_addr=0;
            cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[flag] <<")\n";
            if (msg[flag]=="Hit")
            {
                hit_counter++;
            }
            if (msg[flag] == "Miss") {
                miss_counter++;
            }
        }
    cout << "Hits: " << hit_counter<<endl << "missess:  "<< dec << cache_misses<<  endl;

    }


 ratio_hit = hit_counter/(hit_counter+miss_counter);
    cout <<"Ratio of hits : "<< ratio_hit*100 <<" %"<< endl;
    ratio_miss = miss_counter/(hit_counter+miss_counter);
   cout <<"Ratio of miss : "<<ratio_miss*100 <<" %"<< endl;




}

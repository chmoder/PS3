/*File main.cpp */

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#include "word.h"
#include "list.h"

int main( int argc, char * argv[] )
{
    List list1;
    Word word;


    if ( argc < 2 )
    {
        cerr << "wfc: You must include an input file. \n"
            << "Try `wfc filename.filetype" << endl;
        //exit(1);
    }
    ifstream inputfile ( argv[1] );
    char line[sizeof(inputfile)];
    char * delim = " \\\"/+!?.,|";
    printf("\n");
    while ( inputfile.getline( line, sizeof( inputfile ) ) )
    {
        char * ptr = strtok ( line, delim );
        while( ptr )
        {
            list1.insert ( ptr );
            ptr = strtok( NULL, delim );
        }
    }
    printf("\n");
    
    cout << list1 << endl;

/*
word = "This";
list1.insert( word );
word = "is";
list1.insert( word );
word = "a";
list1.insert( word );
word = "test";
list1.insert( word );
*/

    return 0;
}

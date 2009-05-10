/* File word.cpp */

#include<iostream>
#include<cstring>
using namespace std;
#include "word.h"

void Word::copy ( const char * satish_sucks ) // vital piece of program.
    //This function is used to convert a const * char and return a "Word"
    //for use in pgm 
{
    if (!satish_sucks)
        satish_sucks = "";
    int how_many = strlen( satish_sucks );
    ++how_many; // add one for the NULL
    array = new char [ how_many ];
    strcpy( array, satish_sucks );
}

const Word & Word::operator = ( const char * var ) //copy const char * to a
    //"Word" needed due to overloading
{
    copy( var );
    return *this;
}

const Word & Word::operator = ( const Word & var ) // used for copying word
    //and overloading the "=" again but i with another "Word"
{
    copy( var.array );
    return *this;
}

Word::~Word( void )  // destructor
{
    delete [] array;
}

Word::Word ( const char * cp )  // const char * construction
{
    copy( cp );
}

Word::Word ( const Word & w )  // copy constructor
{
    copy( w.array );
}

ostream& operator<< (ostream & ost , const Word & w ) // overload " << "
    // for successfull cout of a "Word"
{
    ost << w.array;
    return ost;
}


bool Word::operator == ( const Word & var) // Overload "==" using
    // strcasecmp and returning a bool
{
    bool rval = false;
    if(!strcasecmp(array, var.array))
    {
        rval = true;
    }
    return rval;
}


bool Word::operator != ( const Word & var ) // Overload "!="
{
    return !(*this == var);
} 


bool Word::operator < ( const Word & var ) // Overload "<"
{
    bool rval = false;
    if( strcasecmp( array, var.array ) < 0 )
        rval = true;
    return rval;    
}

bool Word::operator > ( const Word & var ) //Overload ">"
{
   bool rval = true;
   if (*this == var || *this < var )
        rval = false;
        return rval;
}

bool Word::operator <= ( const Word & var ) //Overload "<="
{
    bool rval = false;
    if ( *this == var || *this < var )
        rval = true;
        return rval;
}

bool Word::operator >= ( const Word & var ) //Overload ">="
{
    return !(*this < var);
}

char & Word::operator [] ( const int var) // Overload "[]" for storage and
    //selection of variable.  Given int return char in the array.  No
    //further work is needed du to the fact that the "=" / assignment var was
    //allready overloaded. 
{
        return array[var];
}

const char & Word::operator[] ( const int var) const //See above
{
       return array[var];
}

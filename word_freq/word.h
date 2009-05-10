/* File word.h */
#include<iostream>
using namespace std;
#ifndef WORD_H
#define WORD_H


class Word
{
    private:
        char * array; //The array used to store all the input  in our class
        void copy ( const char * ); //Prototype for cp func
    public:
        ~Word( void ); // destructor
        Word ( const char * = "" ); // const char * construction
        Word ( const Word & ); 
        friend ostream & operator<< (ostream &, const Word &);
        const Word & operator = ( const char * ); 
        const Word & operator = ( const Word & ); 
        bool operator == ( const Word & ); 
        bool operator != ( const Word & ); 
        bool operator < ( const Word & ); 
        bool operator > ( const Word & ); 
        bool operator <= ( const Word & );
        bool operator >= ( const Word & );
        char & operator [] ( const int); 
        const char & operator [] ( const int) const; 
};
#endif

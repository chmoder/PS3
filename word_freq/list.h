/* File list.h */

#include <iostream>
#include "word.h"
#ifndef LIST_H
#define LIST_H


class List
{
    private:
        class Node 
        {
            public:
                Node * prev;
                Word data;
                int count;
                Node * next;
                Node ( Node * p = NULL, Word d = "default",
                        int c = 1, Node * n = NULL )
                    : prev(p), data(d), count(c), next(n) {;;;}
        };

        Node head;

    public:
        List(void);
        ~List(void);
        void insert( const Word );
        void remove ( const Word );
        Node * trav ( const Word ); 
        void print ( ostream & ) const;
        bool find ( const Word );

};

inline ostream & operator << ( ostream & ost, const List & l )
{
    l.print( ost );
    return ost;
}

#endif

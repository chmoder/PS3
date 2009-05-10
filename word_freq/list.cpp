/* File list.cpp */

#include <iostream>
using namespace std;
#include "list.h"
#include "word.h"  

    List::List (void)
    {
        head.next = head.prev = &head;
    }
    List::~List(void)
    {
        Node * curr =  head.next;
        //cout << curr->data << "  " << curr->next << endl;

        Node * prev;
        while ( curr != &head )   
        {
        //cout << curr->data << "  " << curr->next << endl;
            curr = curr->next;
            prev = curr->prev;
            delete prev;
        }
    }

    List::Node * List::trav ( const Word foo )
    {
        Node * curr = head.next;
        while ( curr != &head && curr->data < foo )
        {
            curr = curr->next;
        }
        return curr;
    }
    
    void List::insert( const Word foo )
    {
        Node * curr = trav( foo );
        
        if ( curr->data == foo )
         ++curr->count;
        else
        {
        Node * temp = new Node ( curr->prev, foo, 1, curr );
        temp->next->prev = temp->prev->next = temp;
        }
    }

    void List::remove ( const Word foo )
    {
        Node * curr = trav( foo );
        if ( curr != &head && curr->data == foo )
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            delete curr;
        }
    }    
    
    void List::print ( ostream & ost ) const
    {
        Node * curr = head.next;
	    while ( curr != &head )
	    {
	        cout << curr->data << " " << curr->count << endl;
	        curr = curr->next;
	    }
    }

    bool List::find ( const Word foo )
    {
        Node * curr = trav( foo );
        return( curr != &head && curr->data == foo );
    }


    /*List::copy( const List & arg )
    {
         node * source = arg.head.prev;
         while ( sourc != &(arg.head) )
         insert (sourc->data );
         src = 
    }
   
    list & List::operator = ( List &arg)
    {
         if (this = &arg)
         clear ();
         copy(arg.head.next);
         return *this;
    }

    bool find ( const Word )
    {
    }*/


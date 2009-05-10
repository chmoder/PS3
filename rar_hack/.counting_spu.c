/* --------------------------------------------------------------  */
/* (C)Copyright 2001,2006,                                         */
/* International Business Machines Corporation,                    */
/* Sony Computer Entertainment, Incorporated,                      */
/* Toshiba Corporation,                                            */
/*                                                                 */
/* All Rights Reserved.                                            */
/*                                                                 */
/* Redistribution and use in source and binary forms, with or      */
/* without modification, are permitted provided that the           */
/* following conditions are met:                                   */
/*                                                                 */
/* - Redistributions of source code must retain the above copyright*/
/*   notice, this list of conditions and the following disclaimer. */
/*                                                                 */
/* - Redistributions in binary form must reproduce the above       */
/*   copyright notice, this list of conditions and the following   */
/*   disclaimer in the documentation and/or other materials        */
/*   provided with the distribution.                               */
/*                                                                 */
/* - Neither the name of IBM Corporation nor the names of its      */
/*   contributors may be used to endorse or promote products       */
/*   derived from this software without specific prior written     */
/*   permission.                                                   */
/*                                                                 */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND          */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,     */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE        */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR            */
/* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT    */
/* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;    */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN       */
/* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR    */
/* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              */
/* --------------------------------------------------------------  */
/* PROLOG END TAG zYx                                              */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int check ( int guess[], int SIZE );
void zero ( int guess[], int SIZE );

int main(unsigned long long id)
{
//printf("Hello Cell (0x%llx) ID:%lld\n",id,id);

int SIZE = 6;
int guess[SIZE];
zero( guess, SIZE ); 

while ( check( guess, SIZE ) == 0 )
{
    //int guess[SIZE + 1]; //increment size of array after all posibilities
    int i = 0;
    SIZE = 1;
    for ( i = 0; i < 37; ++i )
    {
        guess[0] = i;
	//printf("%d   %d \n", SIZE, i);
        if ( check ( guess, SIZE ) == 1 )
        {
            printf("THE PASSWORD IS ");
            exit(1);
        }
        if ( guess[0] == 36 )
        {
            ++guess[SIZE];
            i = 0;
         
            if ( guess[SIZE] == 36 )
            {
                ++SIZE;
                ++guess[SIZE + 1];
            }
        }
        printf("%d %d %d %d %d %d \n", guess[0],guess[1],guess[2],guess[3], guess[4], guess[5]);
        //else if ( guess[SIZE] == 36 )
        //{
        //    int guess[SIZE++];
        ///    zero( guess, SIZE ); 
        //}
     }  

}

  return 0;
}


//Checks password to see if it is the same at the guess.
int check ( int guess[], int SIZE )
{
int SIZE_PASS = 6;
int password [6] = {36,20,36,1,19,11};
int rval = 0;

    int i = 0;
    for ( i = 0; i < SIZE; ++i )
    {
	//printf("%d %d \n", guess[i], password[i]);
        if (guess[i] == password[i] && SIZE_PASS == SIZE)
	{
    	    rval = 1;
        }
        else
	{
            return rval = 0;		
        }
    }
return rval;
}

void zero (int guess[], int SIZE )
{
    int i = 0;
    for (i = 0; i < SIZE; ++i )
    {
        guess[i] = 0;
    }
}

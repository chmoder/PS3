/* File simple.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int check ( int * guess_pass, int SIZE );
void zero ( int guess[], int SIZE );

//start and end are of type time_t. are used for determining the 
//start and end time of the program.  dif is used to store the
//value of difference in times of type double.  int SIZE is the 
//SIZE of the array.  int * guess = calloc( SIZE + 1, 1 *
//sizeof ( int ) ) is pointer to an ZEROED out array of size
//SIZE + 1.  This compiler has issues with using [0] as an
//actual place to add data.  counter is a variable to count
//the number of password checks.
//the current array is printed out whenever it grows in size.
int main()
{
time_t start,end;
double dif = 0;
double atmps = 0;
int SIZE = 0;
int * guess = calloc( SIZE + 1, 1 * sizeof( int ) );
long long int counter = 0;


time (&start);
while ( !check( guess, SIZE ) )
{
++guess[SIZE];
++counter;

//This checks the size of the itterating element of the array (last element in the array) for the end
//of the character map.  If all characters have been checked, then it initiates the carry loop.
    if ( guess[SIZE] > 62 )
    {
    //carry loop will start at the last element of the array and evaluates the 3 if statements for each element in the array.
	    int i;
	    for ( i = SIZE; i > -1; --i )
	    {
		    if ( guess[i] > 62 && i > 1 )
		    {
			    //this if statement increases the value at the postition infront of the current
			    //pointer and zeros the current element.  The loop then checks if any of the
			    //other characters in the array have reached the end of the character map and
			    //repeats as nessisarry
			    ++guess[i - 1];
			    guess[i] = 0; 
		    }
		    else if ( guess[i] > 62 && i == 1 )
		    {
			    //this if statement will itterate the first value in the array by 6 to ensure
			    //that the multiple threads of this program are allways working independently
			    guess[i - 1] += 6;
			    guess[i] = 0;
		    }
		    else if ( guess[i] > 62 && i == 0 )
		    {
			    //this if statement is executed when the first element in the array has tried all
			    //characters in the character map.  By the time this happens all elements should be zero
			    //It then grows the array, and sets the first element equal to the thread number (0-5 on a ps3)
			    //Array size after growth is printed out as well as the number of combos attempted
			    free(guess);
			    guess = calloc( ++SIZE + 1, sizeof( int ) );
			    guess[0] = 5;  
			    printf("We are now checking a password length of %d. \n", SIZE + 1);
			    printf("Attempts == %lld \n\n", counter);
		    }	    
	    }  
    }
}

//Once the password is found it is printed on the screen
//allong with the total time in seconds the program ran 
//and the total number of attempted passwords found and checked.
//04.25.2009 added filewrite support to eleviate leaving a
//terminal session open and eventually timing out
time (&end);
dif = difftime (end,start);
atmps = counter / dif;
	FILE *file;
	file = fopen("password_file.txt", "a+");
	printf("\n The Password == " );
	fprintf(file,"\n The Password == " );

	int i = 0;
	for ( i = 0; i < SIZE + 1; ++i)
	{
		printf("%d ", guess[i] );
		fprintf(file,"%d ", guess[i] );

	}
    printf("\n Total Time= %.1f Seconds.  Total Attempts == %lld Attempts per second = %f \n", dif, counter, atmps);
	fprintf(file,"\n Total Time= %.1f Seconds.  Total Attempts == %lld Attempts per second = %f \n", dif, counter, atmps);
	fclose(file);
	system("pkill hack");
	
	
  return 0;
}


//Checks password to see if the guess password is equivilant
//to the actual password by asking if the SIZE of the guess
//array is equal to the actual password (0 inclusive) and 
//if that then it goes through both password and guess array
//and returns a 0 immediately if even one letter is wrong.
//if the entire guess is the same as the password then a one
//is returned and the wile loop is ended and information is
//output to the user.
int check ( int * guess_pass, int SIZE )
{
int password [8] = {62,62,62,62,62,62,62,62};
int rval = 0;

    int i = 0;
    for ( i = 0; i < SIZE + 1; ++i)
    {
        if ( ( guess_pass[i] == password[i]) && 7 == SIZE )
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

//I generated this before i new about calloc.  This zeros out
//an array so it can be used in the main().  calloc allocates
//and zeros the array.  
void zero (int guess[], int SIZE )
{
    int i = 0;
    for (i = 0; i < SIZE; ++i )
    {
        guess[i] = 0;
    }
}

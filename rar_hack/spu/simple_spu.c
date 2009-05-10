/* File simple.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int check ( int guess[], int SIZE );
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
while (check(guess,SIZE) != 0)
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
			guess[0] = 0;  
			printf("We are now checking a password length of %d. \n", SIZE + 1);
			printf("Attempts == %lld \n\n", counter);
		}	    
	}  
}

}
//Once the password is found it is printed on the screen
//allong with the total time in seconds the program ran 
//and the total number of attempted passwords found and checked.
time (&end);
dif = difftime (end,start);
atmps = counter / dif;
	printf("\n The Password == " );
	int i = 0;
	for ( i = 0; i < SIZE + 1; ++i)
	{
		printf("%d ", guess[i] );
	}
        printf("\n Total Time= %.10f Seconds SIZE == %d; Total Attempts == %lld Attempts per second = %f \n", dif, SIZE, counter, atmps);

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
/*
int check ( int guess[], int SIZE )
{
int password [6] = {62,62,62,62,62,62};
int rval = 0;

    int i = 0;
    for ( i = 0; i < SIZE + 1; ++i)
    {
        if ( (guess[i] == password[i]) && 5 == SIZE )
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
*/

int check ( int guess[], int SIZE )
{
  char s1[SIZE];
  strcpy(s1,"");
  char com[100];
  int i;
  int retval = 0;
  
for( i = 0; i <= SIZE; ++i )
{
    switch(guess[i])
    {
    case 0:
      strcat(s1,"0");
      break;
    case 1:
      strcat(s1,"1");
      break;
    case 2:
      strcat(s1,"2");
      break;
    case 3:
      strcat(s1,"3");
      break;
    case 4:
      strcat(s1,"4");
      break;
    case 5:
      strcat(s1,"5");
      break;
    case 6:
      strcat(s1,"6");
      break;
    case 7:
      strcat(s1,"7");
      break;
    case 8:
      strcat(s1,"8");
      break;
    case 9:
      strcat(s1,"9");
      break;
    case 10:
      strcat(s1,"a");
      break;
    case 11:
      strcat(s1,"b");
      break;
    case 12:
      strcat(s1,"c");
      break;
    case 13:
      strcat(s1,"d");
      break;
    case 14:
      strcat(s1,"e");
      break;
    case 15:
      strcat(s1,"f");
      break;
    case 16:
      strcat(s1,"g");
      break;
    case 17:
      strcat(s1,"h");
      break;
    case 18:
      strcat(s1,"i");
      break;
    case 19:
      strcat(s1,"j");
      break;
    case 20:
      strcat(s1,"k");
      break;
    case 21:
      strcat(s1,"l");
      break;
    case 22:
      strcat(s1,"m");
      break;
    case 23:
      strcat(s1,"n");
      break;
    case 24:
      strcat(s1,"o");
      break;
    case 25:
      strcat(s1,"p");
      break;
    case 26:
      strcat(s1,"q");
      break;
    case 27:
      strcat(s1,"r");
      break;
    case 28:
      strcat(s1,"s");
      break;
    case 29:
      strcat(s1,"t");
      break;
    case 30:
      strcat(s1,"u");
      break;
    case 31:
      strcat(s1,"v");
      break;
    case 32:
      strcat(s1,"w");
      break;
    case 33:
      strcat(s1,"x");
      break;
    case 34:
      strcat(s1,"y");
      break;
    case 35:
      strcat(s1,"z");
      break;
    case 36:
      strcat(s1,"A");
      break;
    case 37:
      strcat(s1,"B");
      break;
    case 38:
      strcat(s1,"C");
      break;
    case 39:
      strcat(s1,"D");
      break;
    case 40:
      strcat(s1,"E");
      break;
    case 41:
      strcat(s1,"F");
      break;
    case 42:
      strcat(s1,"G");
      break;
    case 43:
      strcat(s1,"H");
      break;
    case 44:
      strcat(s1,"I");
      break;
    case 45:
      strcat(s1,"J");
      break;
    case 46:
      strcat(s1,"K");
      break;
    case 47:
      strcat(s1,"L");
      break;
    case 48:
      strcat(s1,"M");
      break;
    case 49:
      strcat(s1,"N");
      break;
    case 50:
      strcat(s1,"O");
      break;
    case 51:
      strcat(s1,"P");
      break;
    case 52:
      strcat(s1,"Q");
      break;
    case 53:
      strcat(s1,"R");
      break;
    case 54:
      strcat(s1,"S");
      break;
    case 55:
      strcat(s1,"T");
      break;
    case 56:
      strcat(s1,"U");
      break;
    case 57:
      strcat(s1,"V");
      break;
    case 58:
      strcat(s1,"W");
      break;
    case 59:
      strcat(s1,"X");
      break;
    case 60:
      strcat(s1,"Y");
      break;
    case 61:
      strcat(s1,"Z");
      break;
    case 62:
      strcat(s1,"-");
      break;
    case 63:
      strcat(s1,"@");
      break;
    case 64:
      strcat(s1,"#");
      break;
    case 65:
      strcat(s1,"$");
      break;
    case 66:
      strcat(s1,"%");
      break;
    case 67:
      strcat(s1,"^");
      break;
    case 68:
      strcat(s1,"&");
      break;
    case 69:
      strcat(s1,"*");
      break;
    case 70:
      strcat(s1,"!");
      break;
    default:
      printf("\n\nERROR\n\n");
	  exit(1);
      break;
    }
}
  
//	strcpy(com,"/home/tcross/code/rar_hack/unrar/unrar e -o+ -p");
//	strncat(com,s1,SIZE+1);
//	strcat(com," /home/tcross/code/rar_hack/unrar/pic.rar");

//	puts(com);
	
//   retval=system(com);
//retval=system("/home/tcross/code/rar_hack/unrar/unrar e -o+ -plooser /home/tcross/code/rar_hack/unrar/pic.rar");
retval=system("whoami");
printf("return %d \n",retval);

return retval;
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

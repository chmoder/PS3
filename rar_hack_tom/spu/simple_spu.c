/* File simple.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int check ( int guess[], int SIZE );

int main(unsigned long long id)
{
printf("Hello Cell (0x%llx) ID:%lld\n",id,id);
system("ping google.com");

time_t time1 = time(NULL);
time_t time2;
int SIZE = 0;
int *guess = calloc( SIZE + 1, 1 * sizeof( int ) );
long long int counter = 0;

while (check( guess, SIZE ) != 0)
{
++counter;

    ++guess[SIZE];
    if ( guess[SIZE] > 62)
    {
        int i;
        for ( i = SIZE; i > -1; --i )
        {
            if ( guess[i] > 62 && i > 1 )
	    {
	        ++guess[i - 1];
	        guess[i] = 0; 
            }
            else if ( guess[i] > 62 && i == 1 )
            {
                guess[i - 1] += 8;
                guess[i] = 0;
            }
	    else if ( guess[i] > 62 && i == 0 )
	    {
	        free(guess);
	        guess = calloc( ++SIZE + 1, sizeof( int ) );
	        guess[0] = 0;
	    }	    
         }  
    }
        //printf("%d %d %d %d %d %d SIZE == %d attempts == %lld SPU = (0x%llx)\n", guess[0],guess[1],guess[2],guess[3], guess[4], guess[5], SIZE, counter,id);
}
        //printf("%d %d %d %d %d %d SIZE == %d attempts == %lld SPU = (0x%llx)\n", guess[0],guess[1],guess[2],guess[3], guess[4], guess[5], SIZE, counter,id);
  time2 = time(NULL) - time1;
  float atmps = ((float)counter)/time2;

  printf("\nThe monkeys hacked my archive in: %lld attemps\nIn: %ld seconds\nThats: %f attempts/second\n",counter,time2,atmps);

  //system("pkill rgm");
  
  
  return 0;
}


/*Checks password to see if it is the same at the guess.
int check ( int guess[], int SIZE )
{
int password [4] = {44,70,12,54};
int rval = 0;

    int i = 0;
    for ( i = 0; i < SIZE + 1; ++i)
    {
        if ( (guess[i] == password[i]) && 3 == SIZE )
	{
            rval = 1;
	}
        else
	{
    	    return rval = 0;
        }
    }
return rval;
}*/
int check ( int guess[], int SIZE )
{
  char s1[SIZE];
  strcpy(s1,"");
  char com[SIZE + 48];
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
  
	strcpy(com,"./unrar/unrar e -o+ -inul -p");
	strncat(com,s1,SIZE+1);
//	strcat(com,"ooser ./pic.rar");
	strcat(com," ./unrar/pcg71f.rar");

//	puts(com);
	
   retval=system(com);

if (retval == 0)
{
  char email[SIZE + 60];
  strcpy(email, "mail -s 'Password: ");
  strncat(email,s1,SIZE+1);
  strcat(email,"' kir_lar@msn.com < /dev/null");
  system(email);
}

return retval;
}

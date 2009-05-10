/* File simple.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


int main()
{
FILE *file;
file = fopen ("results.txt","a+");
fprintf(file,"%s","This is just an example :)");
fclose(file);
system("pkill a.out");//THIS DOES NOT WORK FROM SPU!!!
getchar();

  return 0;
}

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

int main(unsigned long long id)
{
  srand(time(NULL)+id);
  int seed = time(NULL);
  int seedalt = time(NULL)+id;
  int seedalt2 = time(NULL)*id;  

  printf("Hello Cell (0x%llx) ID:%lld TIME:%d ID+TIME:%d ID*TIME:%d\n",id,id,seed,seedalt,seedalt2);

  int tom = 0;
  char s1[5];
  int i;
  long long int counter = 0;

  while (!( strcmp( s1,"to do") == 0 ))
  {
   counter++;
   //puts(s1);
   strcpy(s1,"");
   
   for( i = 0; i < 5; ++i )
    {
    tom = rand()%37;
    switch(tom)
    {
    case 0:
      strcat(s1,"a");
      break;
    case 1:
      strcat(s1,"b");
      break;
    case 2:
      strcat(s1,"c");
      break;
    case 3:
      strcat(s1,"d");
      break;
    case 4:
      strcat(s1,"e");
      break;
    case 5:
      strcat(s1,"f");
      break;
    case 6:
      strcat(s1,"g");
      break;
    case 7:
      strcat(s1,"h");
      break;
    case 8:
      strcat(s1,"i");
      break;
    case 9:
      strcat(s1,"j");
      break;
    case 10:
      strcat(s1,"k");
      break;
    case 11:
      strcat(s1,"l");
      break;
    case 12:
      strcat(s1,"m");
      break;
    case 13:
      strcat(s1,"n");
      break;
    case 14:
      strcat(s1,"o");
      break;
    case 15:
      strcat(s1,"p");
      break;
    case 16:
      strcat(s1,"q");
      break;
    case 17:
      strcat(s1,"r");
      break;
    case 18:
      strcat(s1,"s");
      break;
    case 19:
      strcat(s1,"t");
      break;
    case 20:
      strcat(s1,"u");
      break;
    case 21:
      strcat(s1,"v");
      break;
    case 22:
      strcat(s1,"w");
      break;
    case 23:
      strcat(s1,"x");
      break;
    case 24:
      strcat(s1,"y");
      break;
    case 25:
      strcat(s1,"z");
      break;
    case 26:
      strcat(s1," ");
      break;
    case 27:
      strcat(s1,"1");
      break;
    case 28:
      strcat(s1,"2");
      break;
    case 29:
      strcat(s1,"3");
      break;
    case 30:
      strcat(s1,"4");
      break;
    case 31:
      strcat(s1,"5");
      break;
    case 32:
      strcat(s1,"6");
      break;
    case 33:
      strcat(s1,"7");
      break;
    case 34:
      strcat(s1,"8");
      break;
    case 35:
      strcat(s1,"9");
      break;
    case 36:
      strcat(s1,"0");
      break;
    default:
      printf("ERROR default char chosen %d",tom);
      break;
    }
    //printf("CHARECTER ONE: %c\n",s1[i]);
    }
    //printf("STRING ONE: %s\n",s1);
    //puts(s1);

    //exit(1);
    
  }
   printf("\nThe monkeys created shakespere in: %lld attemps on cell (0x%llx)\n",counter,id);
   puts(s1);



  return 0;
}

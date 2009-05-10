/* File Simple.c */
//Tentative filenames
//Created by Kirk Larson and Thomas Cross
//This program is to brute force hack
//
//


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>

extern spe_program_handle_t simple_spu;
extern spe_program_handle_t simple_spu1;
extern spe_program_handle_t simple_spu2;
extern spe_program_handle_t simple_spu3;
extern spe_program_handle_t simple_spu4;
extern spe_program_handle_t simple_spu5;

#define MAX_SPU_THREADS 	16


void *ppu_pthread_function(void *arg) {
  spe_context_ptr_t ctx;
  unsigned int entry = SPE_DEFAULT_ENTRY;
  
  ctx = *((spe_context_ptr_t *)arg);
  if (spe_context_run(ctx, &entry, 0, NULL, NULL, NULL) < 0) {
    perror ("Failed running context");
    exit (1);
  }
  pthread_exit(NULL);
}


int main()
{
  int spu_threads;
  spe_context_ptr_t ctxs[MAX_SPU_THREADS];
  pthread_t threads[MAX_SPU_THREADS];

  /* Determine the number of SPE threads to create.
   */
  spu_threads = spe_cpu_info_get(SPE_COUNT_USABLE_SPES, -1);
  if (spu_threads > MAX_SPU_THREADS) spu_threads = MAX_SPU_THREADS;

  /* Create several SPE-threads to execute 'simple_spu'.
   */
    /* Create context */
    if ((ctxs[0] = spe_context_create (0, NULL)) == NULL) {
      perror ("Failed creating context");
      exit (1);
    }
    /* Load program into context */
    if (spe_program_load (ctxs[0], &simple_spu)) {
      perror ("Failed loading program");
      exit (1);
    }
    /* Create thread for each SPE context */
    if (pthread_create (&threads[0], NULL, &ppu_pthread_function, &ctxs[0]))  {
      perror ("Failed creating thread");
      exit (1);
    }
/*******************************************/
  if ((ctxs[1] = spe_context_create (0, NULL)) == NULL) {
      perror ("Failed creating context");
      exit (1);
    }
    /* Load program into context */
    if (spe_program_load (ctxs[1], &simple_spu1)) {
      perror ("Failed loading program");
      exit (1);
    }
    /* Create thread for each SPE context */
    if (pthread_create (&threads[0], NULL, &ppu_pthread_function, &ctxs[1]))  {
      perror ("Failed creating thread");
      exit (1);
    }
/**********************************************/
if ((ctxs[2] = spe_context_create (0, NULL)) == NULL) {
      perror ("Failed creating context");
      exit (1);
    }
    /* Load program into context */
    if (spe_program_load (ctxs[2], &simple_spu2)) {
      perror ("Failed loading program");
      exit (1);
    }
    /* Create thread for each SPE context */
    if (pthread_create (&threads[0], NULL, &ppu_pthread_function, &ctxs[2]))  {
      perror ("Failed creating thread");
      exit (1);
    }


/**********************************************/
if ((ctxs[3] = spe_context_create (0, NULL)) == NULL) {
      perror ("Failed creating context");
      exit (1);
    }
    /* Load program into context */
    if (spe_program_load (ctxs[3], &simple_spu3)) {
      perror ("Failed loading program");
      exit (1);
    }
    /* Create thread for each SPE context */
    if (pthread_create (&threads[0], NULL, &ppu_pthread_function, &ctxs[3]))  {
      perror ("Failed creating thread");
      exit (1);
    }


/**********************************************/

if ((ctxs[4] = spe_context_create (0, NULL)) == NULL) {
      perror ("Failed creating context");
      exit (1);
    }
    /* Load program into context */
    if (spe_program_load (ctxs[4], &simple_spu4)) {
      perror ("Failed loading program");
      exit (1);
    }
    /* Create thread for each SPE context */
    if (pthread_create (&threads[0], NULL, &ppu_pthread_function, &ctxs[4]))  {
      perror ("Failed creating thread");
      exit (1);
    }

/**********************************************/

if ((ctxs[5] = spe_context_create (0, NULL)) == NULL) {
      perror ("Failed creating context");
      exit (1);
    }
    /* Load program into context */
    if (spe_program_load (ctxs[5], &simple_spu5)) {
      perror ("Failed loading program");
      exit (1);
    }
    /* Create thread for each SPE context */
    if (pthread_create (&threads[0], NULL, &ppu_pthread_function, &ctxs[5]))  {
      perror ("Failed creating thread");
      exit (1);
    }

/**********************************************/

/****************************************************************************/
  /* Wait for SPU-thread to complete execution.  */
    if (pthread_join (threads[0], NULL)) {
      perror("Failed pthread_join");
      exit (1);
    }

    /* Destroy context */
    if (spe_context_destroy (ctxs[0]) != 0) {
      perror("Failed destroying context");
      exit (1);
    }
  /****************************************/
    if (pthread_join (threads[0], NULL)) {
      perror("Failed pthread_join");
      exit (1);
    }

    /* Destroy context */
    if (spe_context_destroy (ctxs[1]) != 0) {
      perror("Failed destroying context");
      exit (1);
    }


  /**************************************/
if (pthread_join (threads[0], NULL)) {
      perror("Failed pthread_join");
      exit (1);
    }

    /* Destroy context */
    if (spe_context_destroy (ctxs[2]) != 0) {
      perror("Failed destroying context");
      exit (1);
    }
  /****************************************/
if (pthread_join (threads[0], NULL)) {
      perror("Failed pthread_join");
      exit (1);
    }

    /* Destroy context */
    if (spe_context_destroy (ctxs[3]) != 0) {
      perror("Failed destroying context");
      exit (1);
    }
  /****************************************/
if (pthread_join (threads[0], NULL)) {
      perror("Failed pthread_join");
      exit (1);
    }

    /* Destroy context */
    if (spe_context_destroy (ctxs[4]) != 0) {
      perror("Failed destroying context");
      exit (1);
    }
  /****************************************/
if (pthread_join (threads[0], NULL)) {
      perror("Failed pthread_join");
      exit (1);
    }

    /* Destroy context */
    if (spe_context_destroy (ctxs[5]) != 0) {
      perror("Failed destroying context");
      exit (1);
    }
  /****************************************/

  /****************************************/

  printf("\nThe program has successfully executed.\n");

  return (0);
}

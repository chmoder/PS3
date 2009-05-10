/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#include <sched.h>
#include <libspe2.h>
#include <pthread.h>
#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <simple_dma.h>

/* we allocate one control block, to correspond to one SPE */
control_block cb __attribute__ ((aligned (128)));

/* this is the pointer to the SPE code, to be used at thread creation time */
extern spe_program_handle_t simple_dma_spu;

/* this is the handle which will be returned by "spe_context_create."  */
spe_context_ptr_t speid;

/* this variable is the SPU entry point address which is initially set to the default */
unsigned int entry = SPE_DEFAULT_ENTRY;

/* this variable is used to return data regarding an abnormal return from the SPE */
spe_stop_info_t stop_info;

/* here is the variable to hold the address returned by the malloc() call. */
int *data;

int main() 
{
  int i, rc;

  /* Here is the malloc call a data array aligned to a cacheline boundary 
     for efficient transfer.
   */ 
  rc = posix_memalign ((void*)(&data), 128, DATA_BUFFER_SIZE); 
  if (rc != 0)
  {
    fprintf (stderr, "Failed allocating space for data array\n");
    exit (1);
  }

  /* Fill the data array with a fibonacci sequence.
   */
  data[0] = data[1] = 1;
  for (i=2; i<DATA_BUFFER_ENTRIES; i++) {
    data[i] = data[i-1] + data[i-2];
  }

  if (spe_cpu_info_get(SPE_COUNT_PHYSICAL_SPES, -1) < 1) {
    fprintf(stderr, "System doesn't have a working SPE.  I'm leaving.\n");
    return -1;
  }

  printf("Address being sent in control block: %p\n", data);

  /* load the address into the control block */
  cb.addr = (unsigned long long)((uintptr_t)data);
  
  /* create the SPE context */
  if ((speid = spe_context_create(0, NULL)) == NULL) {
    fprintf (stderr, "FAILED: spe_context_create(errno=%d strerror=%s)\n", errno, strerror(errno));
    exit (1);
  }
  
  /* load the SPE program into the SPE context */
  if (spe_program_load(speid, &simple_dma_spu) != 0) {
    fprintf (stderr, "FAILED: spe_program_load(errno=%d strerror=%s)\n", errno, strerror(errno));
    exit (1);
  }

  /* run the SPE context */
  if (spe_context_run(speid, &entry, 0, &cb, NULL, &stop_info) < 0) {
    fprintf (stderr, "FAILED: spe_context_run(errno=%d strerror=%s)\n", errno, strerror(errno));
    exit (1);
  }

  /* destroy the SPE context */
  if (spe_context_destroy(speid) != 0) {
    fprintf (stderr, "FAILED: spe_context_destroy(errno=%d strerror=%s)\n", errno, strerror(errno));
    exit (1);
  }

  /* check the SPE status */
  if (stop_info.stop_reason == SPE_EXIT) {
    if (stop_info.result.spe_exit_code != 0) {
      fprintf(stderr, "FAILED: SPE returned a non-zero exit status\n");
      exit(1);
    }
  } else {
    fprintf(stderr, "FAILED: SPE abnormally terminated\n");
    exit(1);
  }

  printf("PASSED\n");

  return 0;
}

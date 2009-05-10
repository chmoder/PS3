/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#include <spu_mfcio.h>
#include <stdio.h>
#include <simple_dma.h>

/* Here's the local copy of the control block, to be filled by the DMA */
volatile control_block cb __attribute__ ((aligned (128)));

/* Here's the local copy of the data array, to be filled by the DMA */
int data[DATA_BUFFER_ENTRIES] __attribute__ ((aligned (128)));


int main(unsigned long long speid __attribute__ ((unused)), 
	 unsigned long long argp, 
	 unsigned long long envp __attribute__ ((unused))) 
{
  int i;
  unsigned int tag_id;

  /* Reserve a tag for application usage */
  if ((tag_id = mfc_tag_reserve()) == MFC_TAG_INVALID) {
    printf("ERROR: unable to reserve a tag\n");
    return 1;
  }

  /* Here is the actual DMA call */
  /* the first parameter is the address in local store to place the data */
  /* the second parameter holds the main memory address                  */
  /* the third parameter holds the number of bytes to DMA                */
  /* the fourth parameter identifies a "tag" to associate with this DMA  */
  /* (this should be a number between 0 and 31, inclusive)               */
  /* the last two parameters are only useful if you've implemented your  */
  /* own cache replacement management policy.  Otherwise set them to 0.  */

  mfc_get(&cb, argp, sizeof(cb), tag_id, 0, 0);

  /* Now, we set the "tag bit" into the correct channel on the hardware  */
  /* this is always 1 left-shifted by the tag specified with the DMA     */
  /* for whose completion you wish to wait.                              */
  mfc_write_tag_mask(1<<tag_id);

  /* Now, issue the read and wait to guarantee DMA completion before we  */
  /* continue. */
  mfc_read_tag_status_all();

  /* DMA the data from system memory to our local store buffer. */
  mfc_get(data, cb.addr, DATA_BUFFER_SIZE, tag_id, 0, 0);


  printf("Address received through control block = 0x%llx\n", cb.addr);


  /* Wait for the data array DMA to complete. */
  mfc_read_tag_status_all();

  /* Verify that the data array contains a valid fibonacci sequence.
   */
  for (i=2; i<DATA_BUFFER_ENTRIES; i++) {
    if (data[i] != data[i-1] + data[i-2]) {
      printf("ERROR: fibonacci sequence error at entry %d. Expected %d, Got %d\n",
	     i, data[i-1] + data[i-2], data[i]);
      return (1);
    }
  }

  return 0;
}

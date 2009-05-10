/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#ifndef _dma_example_h_
#define _dma_example_h_

#define MAX_NUM_SPE_THREADS     1                                         //Number of SPEs to be used
#define ARRAY_SIZE              (1024 * 1024 * MAX_NUM_SPE_THREADS)         //Total array size


/* Here we define a control block data structure that contains 
 * all the data the SPE needed to get the large array of data into
 * local store.
 *
 * This data structure has a size that's a multiple of 16 bytes */
typedef struct _control_block
{
  unsigned long long in_addr;   //beginning address of the input array
  unsigned long long out_addr;  //beginning address of the output array
  unsigned int       num_elements_per_spe; //number of elements assigned to this spe for processing
  unsigned int       id;        //spe id
  unsigned int       pad[2];
} control_block_t;

#endif /* _dma_example_h_ */

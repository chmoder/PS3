/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#include <stdlib.h>
#include <stdio.h>
#include <spu_mfcio.h>
#include <single_buffer_list.h>


//#define USE_TIMER       1

#ifdef USE_TIMER
#include <spu_timer.h>
#endif /* USE_TIMER */

/* here we allocate the data buffer in local memory to hold the input
 * data as well as the resulting output data 
 *
 * This buffer is aligned on 128 byte boundary for efficient DMA transfer
 */
float local_buffer[CHUNK_SIZE * NUM_LIST_ELEMENTS] __attribute__ ((aligned (128)));

/* here we reserve space for the dma list. 
 * This array is aligned on 16 byte boundary */
mfc_list_element_t dma_list[NUM_LIST_ELEMENTS] __attribute__ ((aligned (16)));

/* control block structure 
 * This buffer is aligned on 128 byte boundary for efficient DMA transfer
 * */
control_block_t control_block __attribute__ ((aligned (128)));
 
/*
 * This routine processes the data that we DMA into local memory. 
 * In this example, all it does is incrementing the array's values
 * by 1. 
 * 
 * The function uses SIMD instructions. This function assumes
 * that buf is properly aligned, size is multiple of 16
 *
 * @param buf: the buffer to be processed, needs to be aligned on 16 byte
 *              boundary
 * @param size: number of elements in the buffer
 *
 * @return void
 */
void process_data_simd (float* buf, unsigned int size)
{
  unsigned int i;
  vector float *vbuf;
  vector float v1 = (vector float) {1.0f, 1.0f, 1.0f, 1.0f};
  vbuf = (vector float*) buf;

  for (i = 0; i < (size / 4); i++)
  {
   vbuf[i] = spu_add (vbuf[i], v1); 
  }
}

/*
 * This routine fills a dma list with the appropriate effective address 
 * and size for the dma list element. 
 *
 * @param dma_list:     the dma list to be filled
 * @param num_elements: number of elements in list
 * @param base_addr:    base effective address
 * @param elem size:    size of each dma element
 *
 * TODO: SIMDize and unroll the fill_dma_list function 
 */ 
void fill_dma_list (mfc_list_element_t* list, int num_elements, unsigned long long base_addr, unsigned int elem_size)
{
  int i;
  for (i = 0; i < num_elements; i++)
  {
    list[i].notify = 0;
    list[i].size = elem_size;
    list[i].eal = base_addr + i*elem_size;
  }
}


int main(unsigned long long speid __attribute__ ((unused)),
         unsigned long long argp, 
         unsigned long long envp __attribute__ ((unused)))
{
  unsigned int tag;
  unsigned long long in_addr, out_addr;
  unsigned int i, num_chunks;

#ifdef USE_TIMER
    uint64_t start, time_working;
      
    spu_slih_register (MFC_DECREMENTER_EVENT, spu_clock_slih);
    spu_clock_start();
    start = spu_clock_read();
#endif /* USE_TIMER */

  /* First, we reserve a MFC tag for use */
  tag = mfc_tag_reserve();
  if (tag == MFC_TAG_INVALID)
  {
    printf ("SPU ERROR, unable to reserve tag\n");
    return 1;
  }

  /* issue DMA transfer to get the control block information from 
   * system memory */
  mfc_get (&control_block, argp, sizeof (control_block_t), tag, 0, 0);

  /* wait for the DMA to complete */ 
  mfc_write_tag_mask (1 << tag);
  mfc_read_tag_status_all ();

  /* calculate the number of blocks (chunks) that this spe is assigned 
   * to process */
  num_chunks = control_block.num_elements_per_spe/CHUNK_SIZE;

  /*
   * This is the main loop.  We basically goes through the num_chunks of data
   * NUM_LIST_ELEMENTS at a time. Each list element is going to move CHUNK_SIZE
   * of data into system memory. Data is moved into local store, processed, and 
   * written back to system memory NUM_LIST_ELEMENT chunks per loop iteration.      
   */
  for (i = 0; i < num_chunks; i+= NUM_LIST_ELEMENTS)
  {
    /* set the in_addr and out_addr variables, we will use these for
     * issuing DMA get and put commands */
    in_addr = control_block.in_addr + (i * CHUNK_SIZE * sizeof (float));
    out_addr = control_block.out_addr + (i * CHUNK_SIZE * sizeof (float));

    /* fill the dma list with the appropriate lower 32bit effective address and size for
     * each dma list element. This dma list is used to gather the input data 
     * from system memory */
    fill_dma_list (dma_list, NUM_LIST_ELEMENTS, in_addr, CHUNK_SIZE * sizeof(float)); 

    /* issue a DMA get list command to gather the NUM_LIST_ELEMENT chunks of data from system memory */
    mfc_getl (local_buffer, in_addr, dma_list, NUM_LIST_ELEMENTS * sizeof(mfc_list_element_t), tag, 0, 0);

    /* wait for the DMA get list command to complete */ 
    mfc_write_tag_mask (1 << tag);
    mfc_read_tag_status_all ();

    /* invoke process_data to work on the data that's just been moved into local store*/
    process_data_simd (local_buffer, CHUNK_SIZE * NUM_LIST_ELEMENTS);

    /* fill the dma list with the appropriate lower 32 bit ea and size for each
     * dma list element. This dma list is used to scatter the output data to system memory  */
    fill_dma_list (dma_list, NUM_LIST_ELEMENTS, out_addr, CHUNK_SIZE * sizeof(float)); 

    /* issue the DMA put list command to scatter the result from local memory to 
    * different places in system memory */
    mfc_putl (local_buffer, out_addr, dma_list, NUM_LIST_ELEMENTS * sizeof(mfc_list_element_t), tag, 0, 0);

    /* wait for the DMA put list to complete */
    mfc_write_tag_mask (1 << tag);
    mfc_read_tag_status_all ();
  }

#ifdef USE_TIMER
  time_working = (spu_clock_read() - start);
  spu_clock_stop();
  printf ("SPE time_working = %lld\n", time_working);
#endif /* USE_TIMER */

  return 0;
}

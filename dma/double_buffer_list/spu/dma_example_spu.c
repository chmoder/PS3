/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#include <stdio.h>
#include <stdlib.h>
#include <spu_mfcio.h>
#include <double_buffer_list.h>

//#define USE_TIMER       1
#ifdef USE_TIMER
#include <spu_timer.h>
#endif /* USE_TIMER */


/* here we allocate two data buffers in local memory to hold the input
 * data as well as the resulting output data. 
 *
 * The two buffers are aligned by 128byte for effective DMA transfers
 */
float local_buffers[2][CHUNK_SIZE * NUM_LIST_ELEMENTS] __attribute__ ((aligned (128)));

/*
 * here we reserve space for the dma lists. 
 * There are two dma lists for input and dma lists for output. 
 *
 * These arrays are aligned on 16 byte boundary */
mfc_list_element_t dma_list_in[2][NUM_LIST_ELEMENTS] __attribute__ ((aligned (16)));
mfc_list_element_t dma_list_out[2][NUM_LIST_ELEMENTS] __attribute__ ((aligned (16)));

/* 
 * control block structure 
 *
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
void fill_dma_list (mfc_list_element_t* list, int num_elements, unsigned long long base_addr, 
    unsigned int elem_size)
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
  unsigned int tags[2];
  unsigned long long in_addr, out_addr;
  unsigned int i, num_chunks;
  int cur_buf, next_buf;

#ifdef USE_TIMER
  uint64_t start, time_working, tmp_time;
  spu_slih_register (MFC_DECREMENTER_EVENT, spu_clock_slih);
  spu_clock_start();
  start = spu_clock_read();

#endif /* USE_TIMER */
#ifdef USE_TIMER_1

  dma_time = 0;
  process_time = 0;

#endif /* USE_TIMER */

  /* First, we reserve two MFC tags for use with double buffering */
  tags[0] = mfc_tag_reserve();
  tags[1] = mfc_tag_reserve();
  if ((tags[0] == MFC_TAG_INVALID) || (tags[1] == MFC_TAG_INVALID))
  {
    printf ("SPU ERROR, unable to reserve tag\n");
    return 1;
  }
  /* issue DMA transfer to get the control block information from 
   * system memory */
  mfc_get (&control_block, argp, sizeof (control_block_t), tags[0], 0, 0);

  /* wait for the DMA to complete */ 
  mfc_write_tag_mask (1 << tags[0]);
  mfc_read_tag_status_all ();

  /* set the current working buffer to 0 */
  cur_buf = 0;
  
  /* calculate the number of blocks (chunks) that this spe is assigned 
   * to process */
  num_chunks = control_block.num_elements_per_spe/CHUNK_SIZE;

  /* in_addr is set to be the effective address of the current location
   * of the input buffer in system memory. */
  in_addr = control_block.in_addr;

  /* compute and fill out the first DMA list*/
  fill_dma_list (dma_list_in[cur_buf], NUM_LIST_ELEMENTS, in_addr, 
      CHUNK_SIZE * sizeof(float)); 

  /* Issue the first DMA get list command to fetch the NUM_LIST_ELEMENT chunks 
   * of data from system memory */
  mfc_getl (local_buffers[cur_buf], in_addr, dma_list_in[cur_buf], 
      NUM_LIST_ELEMENTS * sizeof(mfc_list_element_t), tags[cur_buf], 0, 0);

  /* increment the system memory address to the addr of the next chunk in the array */
  in_addr = control_block.in_addr + NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof(float);

  /* out_addr is set to be the effective address of the current location
   * of the output buffer in system memory */
  out_addr = control_block.out_addr;

  /*
   * This is the main loop.  We basically goes through the num_chunks of data
   * NUM_LIST_ELEMENTS at a time. Each list element is going to move CHUNK_SIZE
   * of data into system memory. Data is moved into local store, processed, and 
   * written back to system memory NUM_LIST_ELEMENT chunks per loop iteration.      
   */
  for (i = NUM_LIST_ELEMENTS; i < num_chunks; i+=NUM_LIST_ELEMENTS)
  {
    /* set the next working buffer to cur_buf^1. If cur_buf = 1, next_buf = 0
     * if next_buf = 0, cur_buf = 1 */
    next_buf = cur_buf^1;

    /* fill the dma list with the appropriate lower 32bit effective address and size for
     * each dma list element. This dma list is used to gather the input data 
     * from system memory */
    fill_dma_list (dma_list_in[next_buf], NUM_LIST_ELEMENTS, in_addr, 
        CHUNK_SIZE * sizeof(float)); 

    /* issue a DMA get list with barrier command to gather the NUM_LIST_ELEMENT 
     * chunks of data from system memory. We're using the barrier
     * get command here to ensure that the mfc_put of the same buffer with the
     * same tag_id finishes before this DMA moves data from system memory into
     * the buffer.  
     */
    mfc_getlb (local_buffers[next_buf], in_addr, dma_list_in[next_buf], 
      NUM_LIST_ELEMENTS * sizeof(mfc_list_element_t), tags[next_buf], 0, 0);
   
    /* wait for both the mfc_putl and mfc_getlb to complete */
    mfc_write_tag_mask (1 << tags[cur_buf]);
    mfc_read_tag_status_all ();

    /* invoke process_data to work on the data that's just been moved into 
     * local store*/
    process_data_simd(local_buffers[cur_buf], CHUNK_SIZE * NUM_LIST_ELEMENTS);

    /* fill the dma list with the appropriate lower 32bit effective address and size for
     * each dma list element. This dma list is used to gather the input data 
     * from system memory */   
    fill_dma_list (dma_list_out[cur_buf], NUM_LIST_ELEMENTS, out_addr, 
        CHUNK_SIZE * sizeof(float)); 

    /* issue the DMA put list command to scatter result from local memory to 
    * system memory */
    mfc_putl (local_buffers[cur_buf], out_addr, dma_list_out[cur_buf],
        NUM_LIST_ELEMENTS * sizeof(mfc_list_element_t), tags[cur_buf], 0, 0); 

    /* increment the system memory address to the addr of the next chunk in the array */
    in_addr += NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof(float);
    out_addr += NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof(float);

    /* update tag and buffer index for the next iteration */
    cur_buf = next_buf;
  }

  /* wait for the last transfer to complete */
  mfc_write_tag_mask (1 << tags[cur_buf]);
  mfc_read_tag_status_all ();

  /* process the last chunk of data */
  process_data_simd (local_buffers[cur_buf], CHUNK_SIZE * NUM_LIST_ELEMENTS);
  
  /* DMA the last output to system memory */
  fill_dma_list (dma_list_out[cur_buf], NUM_LIST_ELEMENTS, out_addr, 
      CHUNK_SIZE * sizeof(float));
  mfc_putl (local_buffers[cur_buf], out_addr, dma_list_out[cur_buf],
      NUM_LIST_ELEMENTS * sizeof(mfc_list_element_t), tags[cur_buf], 0, 0); 

  /* wait for the last out transfer to complete */
  mfc_write_tag_mask (1 << tags[cur_buf]);
  mfc_read_tag_status_all ();

#ifdef USE_TIMER
  time_working = (spu_clock_read() - start);
  spu_clock_stop();
  printf ("SPE time_working = %lld\n", time_working);
#endif /* USE_TIMER */


  return 0;
}

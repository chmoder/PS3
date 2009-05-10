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
#include <triple_buffer_list_in_out.h>

//#define USE_TIMER       1

#ifdef USE_TIMER
#include <spu_timer.h>
#endif /* USE_TIMER */

/* 
 * Here we define three data buffers in local memory to hold the input
 * or output data. The algorithm will rotate through the buffers
 * and utilize the buffers as input or output depends on the iteration   
 *
 * These buffers are aligned on 128 byte boundary for efficient DMA transfer
 */
float local_buffers[3][CHUNK_SIZE * NUM_LIST_ELEMENTS] __attribute__ ((aligned (128)));

/*
 * here we reserve space for the dma lists. 
 * There are two dma lists for input and dma lists for output. 
 *
 * These arrays are aligned on 16 byte boundary
 */ 
mfc_list_element_t dma_lists[4][NUM_LIST_ELEMENTS] __attribute__ ((aligned (16)));

/* 
 * control block structure 
 *
 * This buffer is aligned on 128 byte boundary for efficient DMA transfer 
 * */
control_block_t control_block __attribute__ ((aligned (128)));

/*
 * This routine process the data that we DMA into local memory.  
 * 
 * The function is coded using SIMD instructions. This function assumes
 * that buf is properly aligned, size is multiple of 16
 *
 * @param buf_in: the buffer to be processed, needs to be aligned on 128 byte
 *              boundary
 * @param buf_out: the buffer that stores the output, needs to be aligned on 128 byte
 *              boundary
 * @param size: number of elements in the buffer
 *
 * @return void
 */
void process_data_simd (float* in_buf, float* out_buf, unsigned int size)
{
  unsigned int i;
  vector float *vbuf_in;
  vector float *vbuf_out;
  vector float v1 = (vector float) {1.0f, 1.0f, 1.0f, 1.0f};
  vbuf_in = (vector float*) in_buf;
  vbuf_out = (vector float*) out_buf;

  for (i = 0; i < (size / 4); i++)
  {
   vbuf_out[i] = spu_add (vbuf_in[i], v1); 
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
 */
void
fill_dma_list (mfc_list_element_t * list, int num_elements,
	       unsigned long long base_addr, unsigned int elem_size)
{
  int i;

  for (i = 0; i < num_elements; i++)
  {
    list[i].notify = 0;
    list[i].size = elem_size;
    list[i].eal = base_addr + i * elem_size;
  }
}

int
main (unsigned long long speid __attribute__ ((unused)),
      unsigned long long argp,
      unsigned long long envp __attribute__ ((unused)))
{
  unsigned int tags[3];
  unsigned long long in_addr, out_addr;
  unsigned int i, num_chunks;
  int cur_in_buf, next_in_buf, cur_out_buf;
  int in_dma_list_idx, out_dma_list_idx;

#ifdef USE_TIMER
  uint64_t start, time_working;
  spu_slih_register (MFC_DECREMENTER_EVENT, spu_clock_slih);

  spu_clock_start ();
  start = spu_clock_read ();
#endif /* USE_TIMER */

  /* First, we reserve three MFC tags for use with each of the data buffer */
  tags[0] = mfc_tag_reserve();
  tags[1] = mfc_tag_reserve();
  tags[2] = mfc_tag_reserve();

  if ((tags[0] == MFC_TAG_INVALID) || (tags[1] == MFC_TAG_INVALID)
      || (tags[2] == MFC_TAG_INVALID))
  {
    printf ("SPU ERROR, unable to reserve tag\n");
    return 1;
  }
  /* DMA the control block information from system memory */
  mfc_get (&control_block, argp, sizeof (control_block_t), tags[0], 0, 0);

  /* wait for the DMA to complete */
  mfc_write_tag_mask (1 << tags[0]);
  mfc_read_tag_status_all ();

  /* calculate the number of blocks (chunks) that this spe is assigned 
   * to process */
  num_chunks = control_block.num_elements_per_spe/CHUNK_SIZE;

  /* initialize the buffer idices to the appropriate buffer.
   *    cur_in_buf: index of the current input buffer. This is the buffer
   *         that's being processed in the current iteration.
   *    next_in_buf: index of the next input buffer. This is the buffer
   *         that's being transferred in the current iteration.
   *    cur_out_buf: index of the current output buffer. This is the 
   *         buffer that contains the output in the current iteration. 
   * */
  cur_in_buf = 0;
  next_in_buf = 1;
  cur_out_buf = 2;

  /* in_addr is set to be the effective address of the current location
   * of the input buffer in system memory. */
  in_addr = control_block.in_addr;

  /* out_addr is set to be the effective address of the current location
   * of the output buffer in system memory */
  out_addr = control_block.out_addr;

  /* set the indices of the dma list that's currently being used. 
   *   in_dma_list_idx: index for the current input dma list 
   *   out_dma_list_idx: index for the current output dma list 
   *   */
  in_dma_list_idx = 0;
  out_dma_list_idx = 2;

  /* compute and fill out the first DMA list*/
  fill_dma_list (dma_lists[in_dma_list_idx], NUM_LIST_ELEMENTS, in_addr,
		 CHUNK_SIZE * sizeof (float));

  /* Issue the first DMA get list command to fetch the NUM_LIST_ELEMENT chunks 
   * of data from system memory */
  mfc_getl (local_buffers[cur_in_buf], in_addr, dma_lists[in_dma_list_idx],
	    NUM_LIST_ELEMENTS * sizeof (mfc_list_element_t), tags[cur_in_buf],
	    0, 0);
  /* increment the system memory address to the addr of the next chunk in the array */
  in_addr += NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof (float);

  /* update the index of the current dma list */
  in_dma_list_idx = in_dma_list_idx^1;

  /* compute and fill out the second input DMA list */
  fill_dma_list (dma_lists[in_dma_list_idx], NUM_LIST_ELEMENTS, in_addr,
		 CHUNK_SIZE * sizeof (float));

  /* issue the second DMA get list command to fetch the NUM_LIST_ELEMENTS chunks
   * of data from system memory */
  mfc_getl (local_buffers[next_in_buf], in_addr, dma_lists[in_dma_list_idx],
	    NUM_LIST_ELEMENTS * sizeof (mfc_list_element_t),
	    tags[next_in_buf], 0, 0);

  /* increment the system memory address to the addr of the next chunk in the array */
  in_addr += NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof (float);

  /* update the index of the current dma list */
  in_dma_list_idx = in_dma_list_idx^1;

  /* wait for the first DMA get list transfer to finish */
  mfc_write_tag_mask (1 << tags[cur_in_buf]);
  mfc_read_tag_status_all ();

  /* invoke process_data to work on the data that's just been moved into 
   * local store, the cur_in_buf*/
  process_data_simd (local_buffers[cur_in_buf], local_buffers[cur_out_buf],
		CHUNK_SIZE * NUM_LIST_ELEMENTS);

  /*
   * This is the main loop.  We basically goes through the num_chunks of data
   * NUM_LIST_ELEMENTS at a time. Each list element is going to move CHUNK_SIZE
   * of data into system memory. Data is moved into local store, processed, and 
   * written back to system memory NUM_LIST_ELEMENT chunks per loop iteration.      
   */
  for (i = 2 * NUM_LIST_ELEMENTS; i < num_chunks; i += NUM_LIST_ELEMENTS)
  {
    int tmp;

    /* compute and fill out the dma list for the current output buffer */
    fill_dma_list (dma_lists[out_dma_list_idx], NUM_LIST_ELEMENTS, out_addr,
		   CHUNK_SIZE * sizeof (float));

    /* issue the DMA put list command to scatter the output buffer to the 
     * system memory */
    mfc_putl (local_buffers[cur_out_buf], out_addr, dma_lists[out_dma_list_idx],
	      NUM_LIST_ELEMENTS * sizeof (mfc_list_element_t),
	      tags[cur_out_buf], 0, 0);

    /* update the index of the current output dma list */
    out_dma_list_idx = out_dma_list_idx^1;

    /* increment the system memory address to the addr of the next chunk in the array */
    out_addr += NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof (float);

    /* update indices */
    tmp = next_in_buf;
    next_in_buf = cur_out_buf;
    cur_out_buf = cur_in_buf;
    cur_in_buf = tmp;

    /* compute and fill out the dma list for the next input buffer */
    fill_dma_list (dma_lists[in_dma_list_idx], NUM_LIST_ELEMENTS, in_addr,
		   CHUNK_SIZE * sizeof (float));

    /* Issue a DMA get list with barrier command to gather the next parts of the 
     * input buffer into local store for processing. We're using the barrier
     * get list command here to ensure that the mfc_putl of the same buffer with the
     * same tag_id finishes before this DMA moves data from system memory into
     * the buffer.  
     */
    mfc_getlb (local_buffers[next_in_buf], in_addr, dma_lists[in_dma_list_idx],
	       NUM_LIST_ELEMENTS * sizeof (mfc_list_element_t),
	       tags[next_in_buf], 0, 0);

    /* increment in_addr to point to the addr of the next chunk in the array */
    in_addr += NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof (float);

    /* update the index of the input dma list */
    in_dma_list_idx = in_dma_list_idx^1;

    /* wait for the cur_in_buf */
    mfc_write_tag_mask (1 << tags[cur_in_buf]);
    mfc_read_tag_status_all ();

    /* compute the current input buffer  */
    process_data_simd (local_buffers[cur_in_buf], local_buffers[cur_out_buf],
		  CHUNK_SIZE * NUM_LIST_ELEMENTS);

  }

  /* issue DMA put to move the cur_out_buf to system mem */
  fill_dma_list (dma_lists[out_dma_list_idx], NUM_LIST_ELEMENTS, out_addr,
		 CHUNK_SIZE * sizeof (float));
  mfc_putl (local_buffers[cur_out_buf], out_addr, dma_lists[out_dma_list_idx],
	    NUM_LIST_ELEMENTS * sizeof (mfc_list_element_t),
	    tags[cur_out_buf], 0, 0);

  out_addr += NUM_LIST_ELEMENTS * CHUNK_SIZE * sizeof (float);
  out_dma_list_idx = out_dma_list_idx^1;

  cur_out_buf = cur_in_buf;
  cur_in_buf = next_in_buf;

  /* wait for the last buffer to arrive */
  mfc_write_tag_mask (1 << tags[cur_in_buf]);
  mfc_read_tag_status_all ();

  /* compute the current input buffer */
  process_data_simd (local_buffers[cur_in_buf], local_buffers[cur_out_buf],
		NUM_LIST_ELEMENTS * CHUNK_SIZE);

  /* DMA the last result to system memory */
  fill_dma_list (dma_lists[out_dma_list_idx], NUM_LIST_ELEMENTS, out_addr,
		 CHUNK_SIZE * sizeof (float));
  mfc_putl (local_buffers[cur_out_buf], out_addr, dma_lists[out_dma_list_idx],
	    NUM_LIST_ELEMENTS * sizeof (mfc_list_element_t),
	    tags[cur_out_buf], 0, 0);

  /* wait for the last output to complete */
  mfc_write_tag_mask (1 << tags[cur_out_buf]);
  mfc_read_tag_status_all ();

#ifdef USE_TIMER
  time_working = (spu_clock_read () - start);
  spu_clock_stop ();

  printf ("SPE time_working = %lld\n", time_working);
#endif /* USE_TIMER */

  return 0;
}

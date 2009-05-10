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
#include <triple_buffer_in_out.h>

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
 * */
float local_buffers[3][CHUNK_SIZE] __attribute__ ((aligned (128)));

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


int main(unsigned long long speid __attribute__ ((unused)),
         unsigned long long argp, 
         unsigned long long envp __attribute__ ((unused)))
{
  unsigned int tags[3];
  unsigned long long in_addr, out_addr; 
  unsigned int i, num_chunks;
  int cur_in_buf, next_in_buf, cur_out_buf;

#ifdef USE_TIMER
  uint64_t start, time_working;
  spu_slih_register (MFC_DECREMENTER_EVENT, spu_clock_slih);

  spu_clock_start();
  start = spu_clock_read();
#endif /* USE_TIMER */

  /* First, we reserve three MFC tags for use with each of the data buffer */
  tags[0] = mfc_tag_reserve();
  tags[1] = mfc_tag_reserve();
  tags[2] = mfc_tag_reserve();
  
  if ((tags[0] == MFC_TAG_INVALID) || (tags[1] == MFC_TAG_INVALID) || (tags[2] == MFC_TAG_INVALID))
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

  /* Issue the first DMA into the cur_in_buf */
  mfc_get (local_buffers[cur_in_buf], in_addr, CHUNK_SIZE * sizeof(float),
      tags[cur_in_buf], 0, 0);

  /* increment the system memory address to the addr of the next chunk in the array */
  in_addr += CHUNK_SIZE * sizeof(float);

  /* Issue the second DMA into the next_in_buf */
  mfc_get (local_buffers[next_in_buf], in_addr, CHUNK_SIZE * sizeof(float),
      tags[next_in_buf], 0, 0);

  /* increment the system memory address to the addr of the next chunk in the array */
  in_addr += CHUNK_SIZE * sizeof(float);

  /* wait for the first DMA get transfer to finish */
  mfc_write_tag_mask (1 << tags[cur_in_buf]);
  mfc_read_tag_status_all ();

  /* invoke process_data to work on the data that's just been moved into 
   * local store, the cur_in_buf*/
  process_data_simd (local_buffers[cur_in_buf], local_buffers[cur_out_buf], CHUNK_SIZE);

  /*
   * This is the main loop.  We basically goes through the num_chunks
   * and fetches one 'chunk' of data at a time, process it, and write 
   * it back to system memory until done. 
   */
  for (i = 2; i < num_chunks; i++)
  {
    int tmp;

    /* Issue a DMA get command to fetch the next part of the 
     * input buffer into local store for processing.   
     */
    mfc_put (local_buffers[cur_out_buf], out_addr, 
        CHUNK_SIZE * sizeof(float), tags[cur_out_buf], 0, 0);

    /* increment out_addr to point to the addr of the next chunk in the array */
    out_addr += CHUNK_SIZE * sizeof(float);

    /* update the buffer indices for the iteration */
    tmp = next_in_buf;
    next_in_buf = cur_out_buf;
    cur_out_buf = cur_in_buf;
    cur_in_buf = tmp;

    /* Issue a DMA get with barrier command to fetch the next part of the 
     * input buffer into local store for processing. We're using the barrier
     * get command here to ensure that the mfc_put of the same buffer with the
     * same tag_id finishes before this DMA moves data from system memory into
     * the buffer.  
     */
    mfc_getb (local_buffers[next_in_buf], in_addr, CHUNK_SIZE * sizeof(float),
        tags[next_in_buf], 0, 0);

    /* increment in_addr to point to the addr of the next chunk in the array */
    in_addr += CHUNK_SIZE * sizeof(float);
   
    /* wait for the cur_in_buf */
    mfc_write_tag_mask (1 << tags[cur_in_buf]);
    mfc_read_tag_status_all ();

    /* compute the current input buffer  */
    process_data_simd (local_buffers[cur_in_buf], local_buffers[cur_out_buf], CHUNK_SIZE);

  }

  /* issue DMA put to move the cur_out_buf to system mem */
  mfc_put (local_buffers[cur_out_buf], out_addr, 
      CHUNK_SIZE * sizeof(float), tags[cur_out_buf], 0, 0);
     
  /* increment out_addr to point to the addr of the next chunk in the array */
  out_addr += CHUNK_SIZE * sizeof(float);

  /* update the buffer indices */
  cur_out_buf = cur_in_buf; 
  cur_in_buf = next_in_buf;

  /* wait for the last buffer to arrive */
  mfc_write_tag_mask (1 << tags[cur_in_buf]);
  mfc_read_tag_status_all ();

  /* compute the current input buffer */
  process_data_simd (local_buffers[cur_in_buf], local_buffers[cur_out_buf], CHUNK_SIZE);

  /* DMA the last result to system memory */
  mfc_put (local_buffers[cur_out_buf], out_addr, 
        CHUNK_SIZE * sizeof(float), tags[cur_out_buf], 0, 0);

  /* wait for the last output to complete */
  mfc_write_tag_mask (1 << tags[cur_out_buf]);
  mfc_read_tag_status_all ();

#ifdef USE_TIMER
  time_working = (spu_clock_read() - start);
  spu_clock_stop();

  printf ("SPE time_working = %lld\n", time_working);
#endif /* USE_TIMER */

  return 0;
}

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
#include <double_buffer.h>

//#define USE_TIMER       1
#ifdef USE_TIMER
#include <spu_timer.h>
#endif /* USE_TIMER */

/* here we allocate two data buffers in local memory to hold the input
 * data as well as the resulting output data. 
 *
 * The two buffers are aligned by 128byte for effective DMA transfers
 */
float local_buffers[2][CHUNK_SIZE] __attribute__ ((aligned (128)));

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

int main(unsigned long long speid __attribute__ ((unused)),
         unsigned long long argp, 
         unsigned long long envp __attribute__ ((unused)))
{
  unsigned int tags[2];
  unsigned long long in_addr, out_addr;
  unsigned int i, num_chunks;
  int cur_buf, next_buf;

#ifdef USE_TIMER
  uint64_t start, time_working;
  spu_slih_register (MFC_DECREMENTER_EVENT, spu_clock_slih);
  spu_clock_start();
  start = spu_clock_read();
#endif /* USE_TIMER */


  /* First, we reserve two MFC tags for use with double buffering */
  tags[0] = mfc_tag_reserve();
  tags[1] = mfc_tag_reserve();
  if ((tags[0] == MFC_TAG_INVALID) || (tags[1] == MFC_TAG_INVALID))
  {
    printf ("SPU ERROR, unable to reserve tag\n");
    return 1;
  }

  /* DMA the control block information from system memory */
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

  /* Issue the first DMA get command to fetch the chunk of data from system memory */
  mfc_get (local_buffers[cur_buf], in_addr, CHUNK_SIZE * sizeof(float),
      tags[cur_buf], 0, 0);

   /* increment the system memory address to the addr of the next chunk in the array */
  in_addr = in_addr + CHUNK_SIZE * sizeof(float);

  /* out_addr is set to be the effective address of the current location
   * of the output buffer in system memory */
  out_addr = control_block.out_addr;

  /*
   * This is the main loop.  We basically goes through the num_chunks
   * and fetches one 'chunk' of data at a time, process it, and write 
   * it back to system memory until done. 
   */
  for (i = 1; i < num_chunks; i++)
  {
    /* set the next working buffer to cur_buf^1. If cur_buf = 1, next_buf = 0
     * if next_buf = 0, cur_buf = 1 */ 
    next_buf = cur_buf^1;

    /* Issue a DMA get with barrier command to fetch the next part of the 
     * input buffer into local store for processing. We're using the barrier
     * get command here to ensure that the mfc_put of the same buffer with the
     * same tag_id finishes before this DMA moves data from system memory into
     * the buffer.  
     */  
    mfc_getb (local_buffers[next_buf], in_addr, CHUNK_SIZE * sizeof(float),
       tags[next_buf], 0, 0 );
    
    /* wait for both the mfc_put and mfc_getb to complete */
    mfc_write_tag_mask (1 << tags[cur_buf]);
    mfc_read_tag_status_all ();

    /* invoke process_data to work on the data that's just been moved into 
     * local store*/
    process_data_simd (local_buffers[cur_buf], CHUNK_SIZE);

   /* issue the DMA put command to transfer result from local memory to 
    * system memory */
    mfc_put (local_buffers[cur_buf], out_addr, 
        CHUNK_SIZE * sizeof(float), tags[cur_buf], 0, 0);

    /* increment in_addr and out_addr to point to the addr of the next chunk in the array */
    in_addr += CHUNK_SIZE * sizeof(float);
    out_addr += CHUNK_SIZE * sizeof(float);

    /* update tag and buffer index for the next iteration */
    cur_buf = next_buf;
  }

  /* wait for the last mfc_put and mfc_getb from the loop to complete */
  mfc_write_tag_mask (1 << tags[cur_buf]);
  mfc_read_tag_status_all ();

  /* process the last chunk of data */
  process_data_simd (local_buffers[cur_buf], CHUNK_SIZE);

  /* DMA the last output to system memory */
  mfc_put (local_buffers[cur_buf], out_addr, 
        CHUNK_SIZE * sizeof(float), tags[cur_buf], 0, 0);

  /* wait for the last out transfer to complete */
  mfc_write_tag_mask (1 << tags[cur_buf]);
  mfc_read_tag_status_all ();

#ifdef USE_TIMER
  time_working = (spu_clock_read() - start);
  spu_clock_stop();
  printf ("SPE %d  time_working = %lld\n", control_block.id, time_working);
#endif /* USE_TIMER */

  return 0;
}

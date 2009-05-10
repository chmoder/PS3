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
#include <single_buffer.h>

//#define USE_TIMER       1

#ifdef USE_TIMER
#include <spu_timer.h>
#endif /* USE_TIMER */

/* here we allocate the data buffer in local memory to hold the input
 * data as well as the resulting output data 
 */
float local_buffer[CHUNK_SIZE] __attribute__ ((aligned (128)));

/* allocating the control_block structure on the stack.  We align
 * it by  */
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
  vector float *vbuf_in;
  vector float v1 = (vector float) {1.0f, 1.0f, 1.0f, 1.0f};
  vbuf_in = (vector float*) buf;

  for (i = 0; i < (size / 4); i++)
  {
   vbuf_in[i] = spu_add (vbuf_in[i], v1); 
  }

}


int main(unsigned long long speid __attribute__ ((unused)),
         unsigned long long argp, 
         unsigned long long envp __attribute__ ((unused)))
{
  unsigned int tag;
  unsigned long long in_addr, out_addr;
  int i, num_chunks;

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

  /* DMA the control block information from system memory */
  mfc_get (&control_block, argp, sizeof (control_block_t), tag, 0, 0);

  /* wait for the DMA to complete */ 
  mfc_write_tag_mask (1 << tag);
  mfc_read_tag_status_all ();

  /* calculate the number of blocks (chunks) that this spe is assigned 
   * to process */
  num_chunks = control_block.num_elements_per_spe/CHUNK_SIZE;

  /*
   * This is the main loop.  We basically goes through the num_chunks
   * and fetches one 'chunk' of data at a time, process it, and write 
   * it back to system memory until done. 
   */
  for (i = 0; i < num_chunks; i++)
  {
    /* set the in_addr and out_addr */
    in_addr = control_block.in_addr + (i* CHUNK_SIZE * sizeof(float));
    out_addr = control_block.out_addr + (i * CHUNK_SIZE * sizeof(float));

    /* issue a DMA get command to fetch the chunk of data from system memory */
    mfc_get (local_buffer, in_addr, CHUNK_SIZE * sizeof(float), 
        tag, 0, 0);

    /* wait for the DMA get to complete */ 
    mfc_write_tag_mask (1 << tag);
    mfc_read_tag_status_all ();

    /* invoke process_data to work on the data that's just been moved into 
     * local store*/
    process_data_simd (local_buffer, CHUNK_SIZE);

   /* issue the DMA put command to transfer result from local memory to 
    * system memory */
    mfc_put (local_buffer, out_addr, CHUNK_SIZE * sizeof(float), 
        tag, 0, 0);

    /* wait for the DMA put to complete */
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

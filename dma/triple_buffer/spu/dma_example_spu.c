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
#include <triple_buffer.h>

//#define USE_TIMER       1

#ifdef USE_TIMER
#include <spu_timer.h>
#endif /* USE_TIMER */


/* here we define the data buffer in local memory to hold the data we
 * DMA in.  We will DMA CHUNK_SIZE elements at a time.  */
float local_buffers[3][CHUNK_SIZE] __attribute__ ((aligned (128)));

/* control block structure */
control_block_t control_block __attribute__ ((aligned (128)));

/* this routine process the data that we DMA into local memory. Here
 * it checks to make sure that the data we DMA into local memory is
 * valid */
void process_data (float* buf, unsigned int chunk_size, int idx)
{
  unsigned int i;
  //printf ("process_data: first in_buf[i] = %f\n", in_buf[0]);
  for (i = 0; i < chunk_size; i++)
  {
    if (buf[i] != (idx * chunk_size) + i)
    {
      printf ("SPU_ERROR: idx = %d, buf[%d] = %f\n", idx, i, buf[i]);
      exit (1);
    }
    buf[i] = buf[i] + 1;
  }
}
 
/*
 * assume that buf is properly aligned, size is multiple of 16
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
  unsigned int tags[3];
  unsigned long long in_addr, out_addr;
  unsigned int i, num_chunks;
  int put_buf, get_buf, process_buf;

#ifdef USE_TIMER
  uint64_t start, time_working;
  spu_slih_register (MFC_DECREMENTER_EVENT, spu_clock_slih);
  spu_clock_start();
  start = spu_clock_read();
#endif /* USE_TIMER */


  /* First, we reserve two MFC tags for use with double buffering */
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
  mfc_write_tag_mask (1 << tags[0]);
  mfc_read_tag_status_all ();

  num_chunks = control_block.num_elements_per_spe/CHUNK_SIZE;

  /* initialize addr, out_addr */
  in_addr = control_block.in_addr;
  out_addr = control_block.out_addr;

  /* Issue the first DMA */
  mfc_get (local_buffers[0], in_addr, CHUNK_SIZE * sizeof(float),
      tags[0], 0, 0);

  /* increment the system memory address to the addr of the next chunk in the array */
  in_addr = in_addr + CHUNK_SIZE * sizeof(float);

  /* Issue the 2nd DMA */
  mfc_get (local_buffers[1], in_addr, CHUNK_SIZE * sizeof(float), tags[1], 0, 0);

  /* increment the system memory address to the addr of the next chunk in the array */
  in_addr = in_addr + CHUNK_SIZE * sizeof(float);

  /* wait for the first DMA to complete */
  mfc_write_tag_mask (1 << tags[0]);
  mfc_read_tag_status_all ();

  /* process local_buffers[buf0] */
  process_data_simd (local_buffers[0], CHUNK_SIZE);

  /* set the buffer indices */
  put_buf = 0;
  process_buf = 1;
  get_buf = 2;

  for (i = 2; i < num_chunks; i++)
  {
    int tmp;
    /* Issue a DMA put to move the buf0 to system mem */
    mfc_put (local_buffers[put_buf], out_addr, 
        CHUNK_SIZE * sizeof(float), tags[put_buf], 0, 0);

    /* update out_addr */
    out_addr += CHUNK_SIZE * sizeof(float);

    /* get the next input buffer */
    mfc_get (local_buffers[get_buf], in_addr, CHUNK_SIZE * sizeof(float),
        tags[get_buf], 0, 0);
    in_addr += CHUNK_SIZE * sizeof(float);
   
    /* wait for the buffers local_buffers[buf1]  */
    mfc_write_tag_mask (1 << tags[process_buf]);
    mfc_read_tag_status_all ();

    /* compute the current input buffer  */
    process_data_simd (local_buffers[process_buf], CHUNK_SIZE);

    /* update buffer indices */
    tmp = put_buf;
    put_buf = process_buf;
    process_buf = get_buf;
    get_buf = tmp;
  }

  /* issue DMA put to move the cur_out_buf to system mem */
  mfc_put (local_buffers[put_buf], out_addr, 
      CHUNK_SIZE * sizeof(float), tags[put_buf], 0, 0);

  out_addr += CHUNK_SIZE * sizeof(float);

  /* wait for the last buffer to arrive */
  mfc_write_tag_mask (1 << tags[process_buf]);
  mfc_read_tag_status_all ();

  /* compute the current input buffer */
  process_data_simd (local_buffers[process_buf], CHUNK_SIZE);

  put_buf = process_buf;

  /* DMA the last result to system memory */
  mfc_put (local_buffers[put_buf], out_addr, 
        CHUNK_SIZE * sizeof(float), tags[put_buf], 0, 0);

  /* wait for the last output to complete */
  mfc_write_tag_mask (1 << tags[put_buf]);
  mfc_read_tag_status_all ();

#ifdef USE_TIMER
  time_working = (spu_clock_read() - start);
  spu_clock_stop();
  printf ("SPE time_working = %lld\n", time_working);
#endif /* USE_TIMER */

  return 0;
}

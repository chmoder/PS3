/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#ifndef __simple_DMA_h__
#define __simple_DMA_h__

#include <stdlib.h> 

#define DATA_BUFFER_ENTRIES	64
#define DATA_BUFFER_SIZE	DATA_BUFFER_ENTRIES*sizeof(int)

typedef struct _control_block {
  unsigned int  chunk_size; /* size, in bytes, of each of these array pieces */
  unsigned char pad[4];     /* pad maintain alignment and sized for correct DMA */
  unsigned long long addr;  /* address to be filled by single-buffered DMA */
} control_block;

#endif /* __simple_DMA_h__ */

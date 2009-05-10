/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#ifndef _single_buffer_h_
#define _single_buffer_h_
#include <dma_example.h>

#define CHUNK_SIZE              4096                    //number of elements in each chunk
#define NUM_CHUNKS              (ARRAY_SIZE/CHUNK_SIZE) //Number of 4K chunks in the array

#endif /* _single_buffer_h_ */

/* --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                            */
/* 5724-S84                                                        */
/* (C) Copyright IBM Corp. 2008       All Rights Reserved          */
/* US Government Users Restricted Rights - Use, duplication or     */
/* disclosure restricted by GSA ADP Schedule Contract with         */
/* IBM Corp.                                                       */
/* --------------------------------------------------------------- */
/* PROLOG END TAG zYx                                              */
#ifndef _destructive_dma_list_h_
#define _destructive_dma_list_h_
#include <dma_example.h>

#define CHUNK_SIZE              1024                       //number of elements in each chunk
#define NUM_CHUNKS              (ARRAY_SIZE/CHUNK_SIZE)    //Number of 4K chunks in the array
#define NUM_LIST_ELEMENTS       16                         //Number of elemements in the DMA list

#endif /* _destructive_dma_list_h_ */

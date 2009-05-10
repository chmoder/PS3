%% --------------------------------------------------------------- 
%% Licensed Materials - Property of IBM                            
%% 5724-S84                                                        
%% (C) Copyright IBM Corp. 2008       All Rights Reserved          
%% US Government Users Restricted Rights - Use, duplication or     
%% disclosure restricted by GSA ADP Schedule Contract with         
%% IBM Corp.                                                       
%% --------------------------------------------------------------- 
%% PROLOG END TAG zYx                                              

Summary:
	Set of small DMA example programs.

Target:
	CBE-Linux PPE and SPE (HW or simulator)

Description:
	This directory contains a set of small example programs which
	show different ways of doing DMA operations.

	Specifically, there are the following:

          simple: demonstrates how to write simple DMA operations.  There are two
          mfc_get commands in this example. 

          single_buffer: demonstrates how to transfer a large array of data from main
          memory to SPE local storage, process, and write the result back to main memory
          using single_buffer technique.  The example assumes that the data processing
          step can overwrite the input buffer. 

          single_buffer_in_out: demonstrates how to transfer a large array of
          data from main memory to SPE local storage, process, and write the
          result back to main memory using single buffering technique.  The
          example assumes that the resulting output cannot overwrite the input
          buffer in local store.  

          single_buffer_list: demonstrates how to transfer scattered data from main
          memory to SPE local storage, process, and write the result back to main memory
          using dma list.  The example assumes that the data processing step can
          overwrite the input buffer. 

          single_buffer_list_in_out: demonstrates how to transfer scattered data 
          from main memory to SPE local storage, process, and write the result back 
          to main memory using dma list. The example assumes that the processing is
          done with two separate buffers, one for input and one for output. Data are 
          transferred and processed sequentially with no multi-buffering technique. 

          double_buffer: demonstrates how to transfer a large array of data from main
          memory to SPE local storage, process, and write the result back to main memory
          using double buffering technique.  The example demonstrates that the
          transferring latency can be hidden using double buffering. The example assumes
          that the data is processed in place so only two buffers are allocated in the
          local store. 

          double_buffer_in_out: demonstrates how to transfer a large array of data from
          main memory to SPE local storage, process, and write the result back to main
          memory using double buffering technique.  The example demonstrates that the
          transferring latency can be hidden using double buffering. The example assumes
          that the resulting output cannot overwrite the input buffer in local store.
          Therefore 4 buffers are allocated in SPE local memory, 2 for input and 2 for
          output. 

          double_buffer_list: demonstrates how to transfer scattered data from main
          memory to SPE local storage, process, and write the result back to main memory
          using dma list and double buffering technique. The example assumes that the
          result output from the data processing step can overwrite the input buffer.
          Latency is hidden from overlapping data transferring with computation. 
          
          double_buffer_list_in_out: demonstrates how to transfer scattered data from
          main memory to SPE local storage, process, and write the result back to main
          memory using dma list and double buffering technique. The example assumes that
          the result output from the data processing step can not overwrite the input
          buffer. 4 local buffers are allocated in SPE local memory, 2 for input, 2 for
          output.  In addition, 4 dma list element arrays are allocated to store the
          input dma list and output dma list.  In this example, latency is hidden from
          overlapping data transferring with computation. 

          triple_buffer: demonstrates how to transfer a large array of data from main
          memory to SPE local storage, process, and write the result back to main memory
          using triple buffering technique.  The example demonstrates that the
          transferring latency can be hidden using triple buffering. The example assumes
          that the data is processed in place so three buffers are allocated in the
          local store.

          triple_buffer_in_out: demonstrates how to transfer a large array of data from main
          memory to SPE local storage, process, and write the result back to main memory
          using double buffering technique.  The example demonstrates that the
          transferring latency can be hidden using triple buffering. The example assumes
          that the resulting output cannot overwrite the input buffer in local store.  3
          buffers are allocated in SPE local memory. The code alternates using each of
          the buffer as both input and output buffer as appropriate.  

          triple_buffer_list_in_out: demonstrates how to transfer scattered data from main
          memory to SPE local storage, process, and write the result back to main memory
          using dma list and triple buffering technique. The example assumes that the
          result output from the data processing step can not overwrite the input
          buffer. 3 local buffers are allocated in SPE local memory. The code alternates
          using each of the buffer as input and output buffer as appropriate.  In
          addition, 4 dma list element arrays are allocated to store the input dma list
          and output dma list.  In this example, latency is hidden from overlapping data
          transferring with computation.

          destructive_dma_list: demonstrates the use of destructive DMA list.  In this
          example, the list element array is constructed at the end of the input buffer
          and overwritten by the transfer.

How to run:
	See each individual directory for specific instructions.


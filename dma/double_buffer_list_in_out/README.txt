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
        demonstrates how to transfer scattered data from main memory to SPE local
        storage, process, and write the result back to main memory using dma list
        with double buffering. The example demonstrates that the
        transferring latency can be hidden using double buffering. The example
        assumes that the resulting output cannot overwrite the input buffer in
        local store.  Therefore 4 buffers are allocated in SPE local memory, 2
        for input and 2 for output.

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer scattered data from main memory to SPE local
        storage, process, and write the result back to main memory using dma list
        with double buffering.    
                   
        In this example, we allocate 4 different buffers on the stack, two for
        input (in_buf0 and in_buf1) and two for output (out_buf0 and
        out_buf1). We also reserve two MFC tag ids to group the DMAs 
        based on the buffer usage. Four dma_list_element arrays are also
        allocated on the stack to store dma list, two for input and two for
        output.   

        ------------------------------------------------------------------
        Time step       Action                                  Tag
        ------------------------------------------------------------------
            0           getl  in_buf0                             0
        ------------------------------------------------------------------
            1           getl  in_buf1                             1
                        wait  in_buf0                             0
                        out_buf0 = compute (in_buf0)
        ------------------------------------------------------------------
            2           putl  out_buf0                            0    
                        getl  in_buf0                             0
                        wait  in_buf1                             1
                        out_buf1 = compute (in_buf1)
        ------------------------------------------------------------------       
            3           putl  out_buf1                            1
                        getl  in_buf1                             1
                        wait  in_buf0 out_buf0                    0
                        out_buf0 = compute (in_buf0)
        ------------------------------------------------------------------
            4           putl  out_buf0                            0
                        getl  in_buf0                             0
                        wait  in_buf1 out_buf1                    1
                        out_buf1 = compute (in_buf1)
        ------------------------------------------------------------------
            5           putl  out_buf1                            1
                        getl  in_buf1                             1
                        wait  inbuf0 out_buf0                     0
                        out_buf0 = compute (in_buf0)
        ------------------------------------------------------------------
            6           putl  out_buf0                            0
                        wait  in_buf1 out_buf1                    1
                        out_ buf1 = compute (in_buf1)
        ------------------------------------------------------------------
            7           putl  out_buf1                            1
                        wait  out_buf1                            1
        ------------------------------------------------------------------

        This algorithm uses four local store data buffers and 4 dma list arrays. 
        Since the SPE local store is fairly limited, it is much better to use an 
        algorithm where the processing can be done in place. See the double_buffer_list
        example.    
                  
How to run:
	The actual executable is called 'double_buffer_list_in_out'
	There are no command options for this program.

        To run, at the command prompt, type
                ./double_buffer_list_in_out



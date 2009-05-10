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
        with triple buffering. The example demonstrates that the
        transferring latency can be hidden using double buffering. The example
        assumes that the resulting output cannot overwrite the input buffer in
        local store.  

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer scattered data from main memory to SPE local
        storage, process, and write the result back to main memory using dma list
        with double buffering.    
                   
        In this example, we allocate 3 different buffers on the stack. They
        are interchangeably used as both inputs and outputs. We also reserve three 
        MFC tag ids to group the DMAs based on the buffer usage. Four dma_list_element 
        arrays are also allocated on the stack to store dma list, two for input and two for
        output.   

        Time step       Action                                  Tag
        ---------------------------------------------------------------
            0           getl   buf0                               0
        ---------------------------------------------------------------
            1           getl   buf1                               1
                        wait   buf0                               0
                        buf2 = compute (buf0)
        ---------------------------------------------------------------
            2           putl   buf2                               2 
                        getlb  buf2                               2 
                        wait   buf1                               1
                        buf0 = compute (in_buf1)
        ---------------------------------------------------------------       
            3           putl   buf0                               0 
                        getlb  buf0                               0 
                        wait   buf2                               2 
                        buf1 = compute (buf2)
        ---------------------------------------------------------------
            4           putl   buf1                               1 
                        getlb  buf1                               1
                        wait   buf0                               0
                        buf2 = compute (buf0)
        ---------------------------------------------------------------
            5           putl   buf2                               2 
                        getlb  buf2                               2
                        wait   buf1                               1
                        buf0 = compute (buf1)
        ---------------------------------------------------------------
            6           putl  buf0                                0 
                        wait  buf2                                2
                        buf1 = compute (buf2)
        ---------------------------------------------------------------
            7           putl  buf1                                1 
                        wait  buf1                                1 

        To avoid a possible race condition, we use the MFC command getlb 
        (DMA get list with barrier). The next input must be ordered with
        respect to the previous put list of the output buffer.  For example, in 
        time step 2, using the getlb command guarantees that buf2 will not 
        be overrun with new data from the getlb command before the putl 
        command on the same buffer is finished
                  
How to run:
	The actual executable is called 'triple_buffer_list_in_out'
	There are no command options for this program.

        To run, at the command prompt, type
                ./triple_buffer_list_in_out



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
        with double buffering. The example assumes that the 
        processing is done in place, i.e. the result from the processing step can 
        overwrite the input buffer. 

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer scattered data from main memory to SPE local
        storage, process, and write the result back to main memory using dma list
        with double buffering. 
        
        In this example, we allocate 2 different buffers on the stack, buf0
        and buf1. The two buffers are used as input and output buffers
        interchangeably. We also reserve two MFC tag ids to group the DMAs 
        based on the buffer usage. Four dma_list_element arrays are also
        allocated on the stack to store dma list, two for input and two for
        output.   

        The following table shows the different activities that happen at each
        time step.  

        Time step       Action                                  Tag
        ---------------------------------------------------------------
            0           getl  buf0                               0
        ---------------------------------------------------------------
            1           getl  buf1                               1
                        wait  buf0                               0
                        buf0 = compute (buf0)
        ---------------------------------------------------------------
            2           putl  buf0                               0
                        getlb buf0                               0
                        wait  buf1                               1
                        buf1 = compute (in_buf1)
        ---------------------------------------------------------------       
            3           putl  buf1                               1
                        getlb buf1                               1
                        wait  buf0                               0
                        buf0 = compute (buf0)
        ---------------------------------------------------------------
            4           putl  buf0                               0
                        getlb buf0                               0
                        wait  buf1                               1
                        buf1 = compute (buf1)
        ---------------------------------------------------------------
            5           putl  buf1                               1
                        getlb buf1                               1
                        wait  buf0                               0
                        buf0 = compute (buf0)
        ---------------------------------------------------------------
            6           putl  buf0                               0
                        wait  buf1                               1 
                        buf1 = compute (buf1)
        ---------------------------------------------------------------
            7           putl buf1                                1
                        wait buf1                                1


        To avoid a possible race condition, we use the MFC command getb 
        (DMA get with barrier). The next input buffer must be ordered with
        respect to the put of the output buffer.  For example, in time step 2, 
        using the getb command guarantees that buf0 will not be overrun with
        new data from the getb command before the put command on the same
        buffer is finished

         
How to run:
	The actual executable is called 'double_buffer_list'
	There are no command options for this program.

        To run, at the command prompt, type
                ./double_buffer_list

Note:
        DMA List is typically used to transfer scattered data from main memory to SPE
        local store. Since the maximum size for one DMA transfer on CBE is
        16K, it is also useful to use DMA list to have multiple DMA
        transfers with only one DMA command being issued. This is what we have
        done in this example. 

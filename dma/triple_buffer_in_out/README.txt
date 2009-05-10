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
        demonstrates how to transfer a large array of data from main memory to SPE
        local storage, process, and write the result back to main memory using
        triple buffering technique.  The example assumes that the resulting output 
        cannot overwrite the input buffer in local store.  Three buffers are 
        allocated in SPE local memory.  They are used as both input and output
        buffers.  

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer a large array of data from main
        memory to SPE local storage, process, and write the result back to
        main memory using triple buffering technique. 
                
        In this example, we allocate 3 different buffers on the stack, buf0
        buf1, and buf2. The three buffers are used as input and output buffers
        interchangeably. We also reserve three MFC tag ids to group the DMAs 
        based on the buffer usage.  

        The following table shows the different activities that happen at each
        time step.  

        Time step       Action                                  Tag
        ---------------------------------------------------------------
            0           get   buf0                               0
        ---------------------------------------------------------------
            1           get   buf1                               1
                        wait  buf0                               0
                        buf2 = compute (buf0)
        ---------------------------------------------------------------
            2           put   buf2                               2 
                        getb  buf2                               2 
                        wait  buf1                               1
                        buf0 = compute (in_buf1)
        ---------------------------------------------------------------       
            3           put   buf0                               0 
                        getb  buf0                               0 
                        wait  buf2                               2 
                        buf1 = compute (buf2)
        ---------------------------------------------------------------
            4           put   buf1                               1 
                        getb  buf1                               1
                        wait  buf0                               0
                        buf2 = compute (buf0)
        ---------------------------------------------------------------
            5           put   buf2                               2 
                        getb  buf2                               2
                        wait  buf1                               1
                        buf0 = compute (buf1)
        ---------------------------------------------------------------
            6           put  buf0                                0 
                        wait buf2                                2
                        buf1 = compute (buf2)
        ---------------------------------------------------------------
            7           put  buf1                                1 
                        wait buf1                                1 

        To avoid a possible race condition, we use the MFC command getb 
        (DMA get with barrier). The next input buffer must be ordered with
        respect to the previous put of the output buffer.  For example, in 
        time step 2, using the getb command guarantees that buf2 will not 
        be overrun with new data from the getb command before the put 
        command on the same buffer is finished

How to run:
	The actual executable is called 'triple_buffer_in_out'
	There are no command options for this program.

        To run, at the command prompt, type
                ./triple_buffer_in_out



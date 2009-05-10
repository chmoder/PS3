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
        triple buffering technique.  The example assumes that the processing is
        done in place, i.e. the result from the processing step can overwrite 
        the input buffer. 

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer a large array of data from main
        memory to SPE local storage, process, and write the result back to
        main memory using triple buffering technique. The processing is done
        in place. 
        
        In this example, we allocate 3 different buffers on the stack, buf0
        buf1, and buf2. The three buffers are used both as input and output 
        buffers since the processing is done in place. We also reserve three 
        MFC tag ids to group the DMAs based on the buffer usage.  

        The following table shows the different activities that happen at each
        time step.  

        Time step       Action                                  Tag
        ---------------------------------------------------------------
            0           get  buf0                               0
        ---------------------------------------------------------------
            1           get  buf1                               1
                        wait buf0                               0
                        buf0 = compute (buf0)
        ---------------------------------------------------------------
            2           put  buf0                               0
                        get  buf2                               2 
                        wait buf1                               1
                        buf1 = compute (in_buf1)
        ---------------------------------------------------------------       
            3           put  buf1                               1
                        get  buf0                               0
                        wait buf2                               2 
                        buf2 = compute (buf2)
        ---------------------------------------------------------------
            4           put  buf2                               2
                        get  buf1                               1
                        wait buf0                               0
                        buf0 = compute (buf0)
        ---------------------------------------------------------------
            5           put  buf0                               0
                        get  buf2                               2
                        wait buf1                               1
                        buf1 = compute (buf1)
        ---------------------------------------------------------------
            6           put  buf1                               1
                        wait buf2                               2
                        buf2 = compute (buf2)
        ---------------------------------------------------------------
            7           put  buf2                               2
                        wait buf2                               2

How to run:
	The actual executable is called 'triple_buffer'
	There are no command options for this program.

        To run, at the command prompt, type
                ./triple_buffer



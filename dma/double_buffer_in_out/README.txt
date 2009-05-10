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
        double buffering technique. The example demonstrates that the
        transferring latency can be hidden using double buffering. The example
        assumes that the resulting output cannot overwrite the input buffer in
        local store.  Therefore 4 buffers are allocated in SPE local memory, 2
        for input and 2 for output.

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer a large array of data from main
        memory to SPE local storage, process, and write the result back to
        main memory using double buffering technique.  
               
        In this example, we allocate 4 different buffers on the stack, two for
        input (in_buf0 and in_buf1) and two for output (out_buf0 and
        out_buf1). We also need two MFC tag ids to group the transfers

        ------------------------------------------------------------------
        Time step       Action                                  Tag
        ------------------------------------------------------------------
            0           get  in_buf0                             0
        ------------------------------------------------------------------
            1           get  in_buf1                             1
                        wait in_buf0                             0
                        out_buf0 = compute (in_buf0)
        ------------------------------------------------------------------
            2           put  out_buf0                            0    
                        get  in_buf0                             0
                        wait in_buf1                             1
                        out_buf1 = compute (in_buf1)
        ------------------------------------------------------------------       
            3           put  out_buf1                            1
                        get  in_buf1                             1
                        wait in_buf0 out_buf0                    0
                        out_buf0 = compute (in_buf0)
        ------------------------------------------------------------------
            4           put  out_buf0                            0
                        get  in_buf0                             0
                        wait in_buf1 out_buf1                    1
                        out_buf1 = compute (in_buf1)
        ------------------------------------------------------------------
            5           put  out_buf1                            1
                        get  in_buf1                             1
                        wait inbuf0 out_buf0                     0
                        out_buf0 = compute (in_buf0)
        ------------------------------------------------------------------
            6           put  out_buf0                            0
                        wait in_buf1 out_buf1                    1
                        out_ buf1 = compute (in_buf1)
        ------------------------------------------------------------------
            7           put  out_buf1                            1
                        wait out_buf1                            1
        ------------------------------------------------------------------

        This algorithm uses four local store data buffers. Since the SPE local 
        store is fairly limited, it is much better to use an algorithm where
        the processing can be done in place. See the double_buffer example.    
                  
How to run:
	The actual executable is called 'double_buffer_in_out'
	There are no command options for this program.

        To run, at the command prompt, type
                ./double_buffer_in_out



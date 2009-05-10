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
        single_buffer technique.  The example assumes that the processing is
        done with two separate buffers, one for input and one for output. 

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer a large array of data from main
        memory to SPE local storage, process, and write the result back to
        main memory using single buffering technique. The processing is done
        with two separate buffers, one for input and one for output. 
         
How to run:
	The actual executable is called 'single_buffer_in_out'
	There are no command options for this program.

        To run, at the command prompt, type
                ./single_buffer_in_out

Note:
        In this example, we use two buffers for input and output for
        demonstrative purpose only. If your algorithm allows, it is probably
        better to use the same buffer for both input and output since SPE
        local store is limited.  



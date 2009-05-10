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
        storage, process, and write the result back to main memory using dma list.
        The example assumes that the processing is done with two separate buffers, 
        one for input and one for output. Data are transferred and processed
        sequentially with no multi-buffering technique. 

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer scattered data from main memory to SPE local
        storage, process, and write the result back to main memory using dma list.

        The example assumes that the processing is
        done with two separate buffers, one for input and one for output.

        However, since this example processes the data sequentially, we only
        need one dma list array.  

How to run:
	The actual executable is called 'single_buffer_list_in_out'
	There are no command options for this program.

        To run, at the command prompt, type
                ./single_buffer_list_in_out

Note:
        DMA List is typically used to transfer scattered data from main memory to SPE
        local store. Since the maximum size for one DMA transfer on CBE is
        16K, it is also useful to use DMA list to have multiple DMA
        transfers with only one DMA command being issued. This is what we have
        done in this example.  



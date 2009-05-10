%% --------------------------------------------------------------- 
%% Licensed Materials - Property of IBM                            
%% 5724-S84                                                        
%% (C) Copyright IBM Corp. 2006,2008, All Rights Reserved          
%% US Government Users Restricted Rights - Use, duplication or     
%% disclosure restricted by GSA ADP Schedule Contract with         
%% IBM Corp.                                                       
%% --------------------------------------------------------------- 
%% PROLOG END TAG zYx                                              
Summary:
        demonstrates how to transfer scattered data from main memory to SPE local
        storage, process, and write the result back to main memory using 
        destructive dma list technique.  
                
        The example assumes that the processing is done with two separate buffers, 
        one for input and one for output. Data are transferred and processed
        sequentially with no multi-buffering technique. 


Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer scattered data from main memory to SPE local
        storage, process, and write the result back to main memory using 
        destructive dma list technique. 

        Destructive DMA list technique enables users to save local storage by 
        constructing the list element array at the end of the target buffer. 
        On transfer, the list element array will be overwritten by the data. The 
        CellBE architecture requires that all list elements within the list
        command are guaranteed to be started and issued in sequence so that as
        long as every list element is of non-zero size, the list array is
        guaranteed to not be overwritten before it is processed. 

        The example assumes that the processing is
        done with two separate buffers, one for input and one for output.

        However, since this example processes the data sequentially, we only
        need one dma list array.  

How to run:
	The actual executable is called 'destructive_dma_list'
	There are no command options for this program.

        To run, at the command prompt, type
                ./destructive_dma_list




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
        done in place, i.e. the result from the processing step can overwrite 
        the input buffer. 

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating how to use
        the DMA calls on Cell to transfer a large array of data from main
        memory to SPE local storage, process, and write the result back to
        main memory using single buffering technique. The processing is done
        in place.  
         
How to run:
	The actual executable is called 'single_buffer'
	There are no command options for this program.

        To run, at the command prompt, type
                ./single_buffer



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
	Simple example of DMA operations in the SPE.

Target:
        CBE-Linux (HW or simulator)

Description:
	This directory contains an example program demonstrating a few simple 
	DMA calls within one SPE.

How to run:

	The actual executable is called 'simple_dma'
	There are no command options for this program.

        To run, at the command prompt, type
                ./simple_dma

Notes:
	The program performs the following sequence of events:
	1)  The PPE uses a malloc() command to allocate a small array in main 
		memory.
	2)  The PPE fills the array with a fibonacci sequence. 1,1,2,3,...
	3)  The PPE then loads the address of the array into a control block.
	4)  The PPU prints the address of the array.
	5)  The PPE then creates and runs an SPE context, passing the address of the 
		control block.
	6)  The SPE performs a simple DMA to load the contents of the control
		block into its local store.
	7)  The SPE prints the array address contained within the contents
		of the control block, and exits.
	8)  The SPE DMAs the data array from system memory to its local store.
	9)  The SPE verifes that the DMA contains a valid fibonacci sequence.

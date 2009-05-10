# --------------------------------------------------------------- 
# Licensed Materials - Property of IBM                            
# 5724-S84                                                        
# (C) Copyright IBM Corp. 2008       All Rights Reserved          
# US Government Users Restricted Rights - Use, duplication or     
# disclosure restricted by GSA ADP Schedule Contract with         
# IBM Corp.                                                       
# --------------------------------------------------------------- 
# PROLOG END TAG zYx                                              

########################################################################
#			Subdirectories
########################################################################

DIRS		:= 	spu


########################################################################
#                       Target
########################################################################

PROGRAM_ppu	:= 	single_buffer_in_out


########################################################################
#                       Local Defines
########################################################################
VPATH		:= ../
IMPORTS         = spu/lib_dma_example_spu.a -lspe2 -lpthread

INSTALL_DIR	= $(EXP_SDKBIN)/tutorial
INSTALL_FILES	= $(PROGRAM_ppu)

########################################################################
#			buildutils/make.footer
########################################################################

ifdef CELL_TOP
	include $(CELL_TOP)/buildutils/make.footer
else
	include ../../../../buildutils/make.footer
endif

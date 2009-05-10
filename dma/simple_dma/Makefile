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
#			Target
########################################################################

PROGRAM_ppu64	= simple_dma


########################################################################
#			Local Defines
########################################################################

IMPORTS 	= spu/lib_simple_dma_spu.a -lspe2

INSTALL_DIR	= /home/tcross/build/tutorial
INSTALL_FILES	= $(PROGRAM_ppu64)

########################################################################
#			buildutils/make.footer
########################################################################

ifdef CELL_TOP
	include $(CELL_TOP)/buildutils/make.footer
else
	include /opt/cell/sdk/buildutils/make.footer
endif

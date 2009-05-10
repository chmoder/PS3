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
#			Target
########################################################################

PROGRAMS_spu 	:= dma_example_spu

LIBRARY_embed	:= lib_dma_example_spu.a


########################################################################
#			Local Defines
########################################################################
INCLUDE         := -I ../../ -I ../

########################################################################
#			buildutils/make.footer
########################################################################

ifdef CELL_TOP
	include $(CELL_TOP)/buildutils/make.footer
else
	include ../../../../../buildutils/make.footer
endif


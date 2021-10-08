# Makefile for SMA-2 watershed model 
# 
# Authors:
# John England, Bureau of Reclamation
# Mark Velleux, Colorado State University
# James Halgren, Riverside Technologies, inc.
# send questions or comments to:
# jsh@riverside.com
#
# Updated:
# Wed Sep  5 15:41:21 MDT 2007
#
# This makefile designed for use with James Halgren
# long-term modeling code. 
#
# to run, type "make" at command line
#
# Tested Platforms:

# Path Prefix to locate headers
HDRPATH  	= ./src
SRCPATH  	= ./src
DEST    	= ./bin

## These debug variables are activated for debug builds
#OBJPATHDEBUG	= ./debugobj
#DESTDEBUG    	= ./debug
#CFLAGSDEBUG		= -g -Wall 
## Name of Target (main program)
#PROGRAMDEBUG	= $(DESTDEBUG)/sma2_d

# Fedora Core 6 Linux HP zx6000 Itanium 2 (Madison)
# Compiler: Intel C compiler
#Intel Itanium2 Madison with Intel C++ Compiler 10.0.023
# compiler: Intel C compiler
#CC		= icc
#CFLAGS		= -fast -O3 -fshort-enums -fno-common
#LIBS		= -lc \
#		  -lm 
#LD		= icc 
#LDFLAGS	= $(CFLAGS)
## compiler flag definitions for Intel C compiler:
##		-g = debug (implies -O0), -Wall = enable all warnnings
##		-O1= enable optimizations, -mcpu=<cpu> optimize for specific cpu
##		-mcpu=pentium4 (DEFAULT)
## Name of Target (main program)
#PROGRAM		= $(DEST)/sma2_icc

# Gentoo Linux 64-bit
# Distro:
# Kernel:
# Compiler: GNU C Compiler
CC		= gcc
## AMD64 Opteron GNU C compiler (gcc)
CFLAGS		= -g -Wall -fopenmp -DPARALLEL
#CFLAGS		= -mtune=prescott 
#CFLAGS		= -Wall -O4 -march=opteron -m64 -W -Wmissing-prototypes \
#-Wconversion -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align\
#-Wwrite-strings -fshort-enums -fno-common -Wnested-externs
#CFLAGS = -O3 -march=opteron -m64 -lgomp -fopenmp -DPARALLEL
# linker standard library flags: GNU C compiler
# libc.a, libm.a
LIBS		= -lc -lm -lgomp -fopenmp 
LD		= gcc
LDFLAGS 	= $(CFLAGS)
# Name of Target (main program)
PROGRAM		= $(DEST)/sma2_gcc
OBJPATH  	= ./obj_gcc

## Gentoo Linux 64-bit
## Distro:
## Kernel:
## Compiler: Portland Group Compiler AMD64 Opteron
#CC		= pgcc
##CFLAGS		= -tp k8-64 -O4 -fastsse -Mipa=fast,inline -Mpfo
##CFLAGS		= -O0
#CFLAGS		= -fast -Mipa=fast,inline -Msmartalloc -mp -tp k8-64 -O3 -fastsse -qp
##CFLAGS		= -tp k8-64 -O3 
##Portland Group Compiler AMD64 Opteron - Profiler Enabled
##CFLAGS		= -tp k8-64 -O4 -fastsse -Mipa=fast,inline -qp -Mpfi
#LD		= pgcc
#LDFLAGS		= $(CFLAGS)
## Name of Target (main program)
#PROGRAM		= $(DEST)/sma2_pg
#OBJPATH  	= ./obj_pg

# Standard Shell
SHELL		= /bin/sh


# Makefile name
MAKEFILE	= Makefile

#
EXTHDRS		=

# User-defined Headers
# {
# Headers - General (alphabetical listing order)
HDRSGEN		= $(HDRPATH)/trex_general_declarations.h\
		  $(HDRPATH)/trex_general_definitions.h

# Headers - Soil Moisture Accounting (alphabetical listing order)
HDRSGWSMA	= $(HDRPATH)/trex_SMA_declarations.h\
		  $(HDRPATH)/trex_SMA_definitions.h

# Headers - Water (alphabetical listing order)
HDRSWATER	= $(HDRPATH)/trex_water_declarations.h\
		  $(HDRPATH)/trex_water_definitions.h

# Headers - Solids (alphabetical listing order)
HDRSSOLIDS	= $(HDRPATH)/trex_solids_declarations.h\
		  $(HDRPATH)/trex_solids_definitions.h

# Headers - Environmental (alphabetical listing order)
HDRSENV		= $(HDRPATH)/trex_environmental_declarations.h\
		  $(HDRPATH)/trex_environmental_definitions.h

# Headers - Chemicals (alphabetical listing order)
HDRSCHEM	= $(HDRPATH)/trex_chemical_declarations.h\
		  $(HDRPATH)/trex_chemical_definitions.h
# }

# Concatenate headers
HDRS		= $(HDRSGEN) $(HDRSGWSMA) $(HDRSWATER) $(HDRSSOLIDS) $(HDRSENV) $(HDRSCHEM)

# List of Objects used for clean operation
# {
# Objects - Main Program Object
# {
OBJSMAIN	= $(OBJPATH)/trex-r2.o \
		  # }

##DEL The debug object is *hopefully* obsolete with
##DEL the use of a modern compiler and debugger.
##DEL (see vi -d ./src/trex-r2-debug.c ./src/trex-r2.c)
## Objects - Main Program Object used for Debugging
## {
#OBJSMAINDEBUG	= $(OBJPATH)/trex-r2-debugger.o \
#		  # }

# Objects - Soil Moisuture Accounting (alphabetical listing order)
# {
OBJSGWSMA		= $(OBJPATH)/percolationSMA.o \
		  $(OBJPATH)/ReadSMAFile.o \
		  $(OBJPATH)/WriteTimeSeriesSMA.o \
		  # }

# Objects - General (alphabetical listing order)
# {
OBJSGEN		= $(OBJPATH)/ComputeFinalState.o \
		  $(OBJPATH)/ComputeInitialState.o \
		  $(OBJPATH)/FreeMemory.o \
		  $(OBJPATH)/Grid-r2.o \
		  $(OBJPATH)/Initialize.o \
		  $(OBJPATH)/NewState.o \
		  $(OBJPATH)/ReadDataGroupA.o \
		  $(OBJPATH)/ReadDataGroupF.o \
		  $(OBJPATH)/ReadInputFile.o \
		  $(OBJPATH)/RunTime.o \
		  $(OBJPATH)/SimulationError-r2.o \
		  $(OBJPATH)/StripString.o \
		  $(OBJPATH)/TimeFunctionInit.o \
		  $(OBJPATH)/UpdateTimeFunction.o \
		  $(OBJPATH)/UtilityFunctions.o \
		  $(OBJPATH)/WriteDumpFile.o \
		  $(OBJPATH)/WriteEndGrids.o \
		  $(OBJPATH)/WriteGrids.o \
		  $(OBJPATH)/WriteMassBalance.o \
		  $(OBJPATH)/WriteSummary.o \
		  $(OBJPATH)/WriteTimeSeries.o
		  # }

# Objects - Water (alphabetical listing order)
# {
OBJSWATER       = $(OBJPATH)/ChannelWaterDepth-r7.o \
		  $(OBJPATH)/ChannelWaterRoute-r8.o \
		  $(OBJPATH)/ComputeChannelElevation.o \
		  $(OBJPATH)/ComputeChannelLength.o \
		  $(OBJPATH)/ComputeChannelTopology-r5.o \
		  $(OBJPATH)/ComputeFinalStateWater.o \
		  $(OBJPATH)/ComputeInitialStateWater.o \
		  $(OBJPATH)/FloodplainWaterTransfer.o \
		  $(OBJPATH)/FreeMemoryWater-r3.o \
		  $(OBJPATH)/Infiltration-r3.o \
		  $(OBJPATH)/InitializeWater-r4-NOAA.o \
		  $(OBJPATH)/Interception-r2.o \
		  $(OBJPATH)/NewStateWater.o \
		  $(OBJPATH)/OverlandWaterDepth-r5.o \
		  $(OBJPATH)/OverlandWaterRoute.o \
		  $(OBJPATH)/Rainfall-r3-NOAA.o \
		  $(OBJPATH)/ReadChannelFile.o \
		  $(OBJPATH)/ReadChannelTransmissionFile.o \
		  $(OBJPATH)/ReadDataGroupB-r5-NOAA.o \
		  $(OBJPATH)/ReadElevationFile.o \
		  $(OBJPATH)/ReadInitialInfiltrationFile.o \
		  $(OBJPATH)/ReadInitialTransmissionLossFile.o \
		  $(OBJPATH)/ReadInitialWaterChannelFile-r2.o \
		  $(OBJPATH)/ReadInitialWaterOverlandFile-r2.o \
		  $(OBJPATH)/ReadLandUseFile.o \
		  $(OBJPATH)/ReadLinkFile.o \
		  $(OBJPATH)/ReadMaskFile.o \
		  $(OBJPATH)/ReadNodeFile-r2.o \
		  $(OBJPATH)/ReadRainGridFile.o \
		  $(OBJPATH)/ReadStorageDepthFile.o \
		  $(OBJPATH)/TimeFunctionInitWater.o \
		  $(OBJPATH)/TransmissionLoss.o \
		  $(OBJPATH)/UpdateTimeFunctionWater.o \
		  $(OBJPATH)/WaterBalance.o \
		  $(OBJPATH)/WaterTransport-r2.o \
		  $(OBJPATH)/WriteEndGridsWater.o \
		  $(OBJPATH)/WriteGridsWater.o \
		  $(OBJPATH)/WriteMassBalanceWater.o \
		  $(OBJPATH)/WriteSummaryWater-r3.o \
		  $(OBJPATH)/WriteTimeSeriesWater.o
		  # }

# Objects - Solids (alphabetical listing order)
# {
OBJSSOLIDS	= $(OBJPATH)/ChannelSolidsAdvection-r2.o \
		  $(OBJPATH)/ChannelSolidsConcentration-r3.o \
		  $(OBJPATH)/ChannelSolidsDeposition.o \
		  $(OBJPATH)/ChannelSolidsDispersion.o \
		  $(OBJPATH)/ChannelSolidsErosion.o \
		  $(OBJPATH)/ChannelSolidsKinetics.o \
		  $(OBJPATH)/ChannelSolidsMineralization.o \
		  $(OBJPATH)/ChannelSolidsTransportCapacity-r2.o \
		  $(OBJPATH)/ChannelSolidsYield.o \
		  $(OBJPATH)/ComputeFinalStateSolids.o \
		  $(OBJPATH)/ComputeInitialStateSolids.o \
		  $(OBJPATH)/ComputeSolidsGroupConcSums.o \
		  $(OBJPATH)/ComputeSolidsGroupTransportSums.o \
		  $(OBJPATH)/FloodplainSolidsTransfer.o \
		  $(OBJPATH)/FreeMemorySolids.o \
		  $(OBJPATH)/InitializeSolids-r2.o \
		  $(OBJPATH)/NewStateSolids.o \
		  $(OBJPATH)/NewStateStack.o \
		  $(OBJPATH)/OverlandSolidsAdvection-r2.o \
		  $(OBJPATH)/OverlandSolidsConcentration-r3.o \
		  $(OBJPATH)/OverlandSolidsDeposition.o \
		  $(OBJPATH)/OverlandSolidsDispersion.o \
		  $(OBJPATH)/OverlandSolidsErosion.o \
		  $(OBJPATH)/OverlandSolidsKinetics.o \
		  $(OBJPATH)/OverlandSolidsMineralization.o \
		  $(OBJPATH)/OverlandSolidsTransportCapacity-r3.o \
		  $(OBJPATH)/OverlandSolidsYield.o \
		  $(OBJPATH)/PopSedimentStack.o \
		  $(OBJPATH)/PopSoilStack.o \
		  $(OBJPATH)/PushSedimentStack-r2.o \
		  $(OBJPATH)/PushSoilStack.o \
		  $(OBJPATH)/ReadDataGroupC-r4.o \
		  $(OBJPATH)/ReadInitialSolidsChannelFile.o \
		  $(OBJPATH)/ReadInitialSolidsOverlandFile.o \
		  $(OBJPATH)/ReadSedimentPropertiesFile.o \
		  $(OBJPATH)/ReadSoilLayerThicknessFile.o \
		  $(OBJPATH)/ReadSoilStackFile.o \
		  $(OBJPATH)/ReadSoilTypeFile-r2.o \
		  $(OBJPATH)/SolidsBalance.o \
		  $(OBJPATH)/SolidsPropertyInit.o \
		  $(OBJPATH)/SolidsTransport.o \
		  $(OBJPATH)/TimeFunctionInitSolids.o \
		  $(OBJPATH)/UpdateTimeFunctionSolids.o \
		  $(OBJPATH)/WriteEndGridsSolids.o \
		  $(OBJPATH)/WriteGridsSolids.o \
		  $(OBJPATH)/WriteMassBalanceSolids.o \
		  $(OBJPATH)/WriteSummarySolids.o \
		  $(OBJPATH)/WriteTimeSeriesSolids.o
		  # }

# Objects - Environmental (alphabetical listing order)
# {
OBJSENV		= $(OBJPATH)/InitializeEnvironment.o \
		  $(OBJPATH)/FreeMemoryEnvironment.o \
		  $(OBJPATH)/ReadDataGroupE.o \
		  $(OBJPATH)/ReadGeneralEnvironmentFile.o \
		  $(OBJPATH)/ReadGeneralTFPointerFile.o \
		  $(OBJPATH)/ReadOverlandEnvironmentFile.o \
		  $(OBJPATH)/ReadOverlandEnvironmentTFPointerFile.o \
		  $(OBJPATH)/ReadChannelEnvironmentFile.o \
		  $(OBJPATH)/ReadChannelFpocFile.o \
		  $(OBJPATH)/ReadOverlandFpocFile.o \
		  $(OBJPATH)/ReadOverlandFpocTFPointerFile.o \
		  $(OBJPATH)/TimeFunctionInitEnvironment.o \
		  $(OBJPATH)/UpdateEnvironment.o \
		  $(OBJPATH)/UpdateTimeFunctionEnvironment.o
		  # }

# Objects - Chemical (alphabetical listing order)
# {
OBJSCHEM	= $(OBJPATH)/ChannelChemicalAdvection-r2.o \
		  $(OBJPATH)/ChannelChemicalBiodegradation.o \
		  $(OBJPATH)/ChannelChemicalConcentration-r3.o \
		  $(OBJPATH)/ChannelChemicalDeposition.o \
		  $(OBJPATH)/ChannelChemicalDispersion.o \
		  $(OBJPATH)/ChannelChemicalDissolution.o \
		  $(OBJPATH)/ChannelChemicalErosion.o \
		  $(OBJPATH)/ChannelChemicalHydrolysis.o \
		  $(OBJPATH)/ChannelChemicalKinetics.o \
		  $(OBJPATH)/ChannelChemicalOxidation.o \
		  $(OBJPATH)/ChannelChemicalPartitioning.o \
		  $(OBJPATH)/ChannelChemicalPhotolysis.o \
		  $(OBJPATH)/ChannelChemicalRadioactive.o \
		  $(OBJPATH)/ChannelChemicalTransmissionLoss.o \
		  $(OBJPATH)/ChannelChemicalUDReaction.o \
		  $(OBJPATH)/ChannelChemicalVolatilization.o \
		  $(OBJPATH)/ChannelChemicalYield.o \
		  $(OBJPATH)/ChemicalBalance.o \
		  $(OBJPATH)/ChemicalPropertyInit.o \
		  $(OBJPATH)/ChemicalTransport.o \
		  $(OBJPATH)/ComputeChemicalGroupConcSums.o \
		  $(OBJPATH)/ComputeChemicalGroupTransportSums.o \
		  $(OBJPATH)/ComputeChemicalGroupPhaseAve.o \
		  $(OBJPATH)/ComputeFinalStateChemical.o \
		  $(OBJPATH)/ComputeInitialStateChemical.o \
		  $(OBJPATH)/FloodplainChemicalTransfer.o \
		  $(OBJPATH)/FreeMemoryChemical.o \
		  $(OBJPATH)/InitializeChemical.o \
		  $(OBJPATH)/NewStateChemical.o \
		  $(OBJPATH)/OverlandChemicalAdvection-r2.o \
		  $(OBJPATH)/OverlandChemicalBiodegradation.o \
		  $(OBJPATH)/OverlandChemicalConcentration-r3.o \
		  $(OBJPATH)/OverlandChemicalDeposition.o \
		  $(OBJPATH)/OverlandChemicalDispersion.o \
		  $(OBJPATH)/OverlandChemicalDissolution.o \
		  $(OBJPATH)/OverlandChemicalErosion.o \
		  $(OBJPATH)/OverlandChemicalHydrolysis.o \
		  $(OBJPATH)/OverlandChemicalInfiltration.o \
		  $(OBJPATH)/OverlandChemicalKinetics.o \
		  $(OBJPATH)/OverlandChemicalOxidation.o \
		  $(OBJPATH)/OverlandChemicalPartitioning.o \
		  $(OBJPATH)/OverlandChemicalPhotolysis.o \
		  $(OBJPATH)/OverlandChemicalRadioactive.o \
		  $(OBJPATH)/OverlandChemicalUDReaction.o \
		  $(OBJPATH)/OverlandChemicalVolatilization.o \
		  $(OBJPATH)/OverlandChemicalYield.o \
		  $(OBJPATH)/ReadDataGroupD.o \
		  $(OBJPATH)/ReadInitialChemicalChannelFile.o \
		  $(OBJPATH)/ReadInitialChemicalOverlandFile.o \
		  $(OBJPATH)/ReadSedimentChemicalFile.o \
		  $(OBJPATH)/ReadSoilLayerChemicalFile.o \
		  $(OBJPATH)/TimeFunctionInitChemical.o \
		  $(OBJPATH)/UpdateTimeFunctionChemical.o \
		  $(OBJPATH)/WriteEndGridsChemical.o \
		  $(OBJPATH)/WriteGridsChemical.o \
		  $(OBJPATH)/WriteMassBalanceChemical.o \
		  $(OBJPATH)/WriteSummaryChemical.o \
		  $(OBJPATH)/WriteTimeSeriesChemical.o
		  # }
# }

# Concatenate objects
OBJS		= $(OBJSMAIN) $(OBJSGWSMA) $(OBJSGEN) $(OBJSWATER) $(OBJSSOLIDS) $(OBJSENV) $(OBJSCHEM)
#OBJSDEBUG	= $(OBJSMAINDEBUG) $(OBJSGWSMA) $(OBJSGEN) $(OBJSWATER) $(OBJSSOLIDS) $(OBJSENV) $(OBJSCHEM)
#OBJSDEBUG = $(subst $(OBJPATH),$(OBJPATHDEBUG),$(OBJS))

# Standard Print
PRINT		= pr

# Generate list of Sources identical to Object List
# (substitute path then change suffix)
SRCS = $(patsubst %.o,%.c, $(subst $(OBJPATH),$(SRCPATH),$(OBJS)))

# Standard library headers and location
# Need to change dir depending on system
SYSHDRS		 = /usr/include/math.h \
		   /usr/include/stdio.h \
		   /usr/include/stdlib.h \
		   /usr/include/string.h \
		   /usr/include/time.h \
		   /usr/lib/gcc/x86_64-pc-linux-gnu/4.1.1/include/float.h

# Make the program binary executable from srcs, objs and dependencies
# Note that CC is invoked via an implicit rule with CFLAGS
all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) 
		@echo "Linking $(PROGRAM) ..."
		@$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
		@echo "done"

#debug:		$(PROGRAMDEBUG)
#CFLAGS = $(CFLAGSDEBUG)
#LDFLAGS = $(CFLAGSDEBUG)
#OBJPATH = $(OBJPATHDEBUG)
#$(PROGRAMDEBUG):$(OBJSDEBUG) 
#@echo "Linking $(PROGRAMDEBUG) ..."
#@$(LD) $(LDFLAGS) $(OBJSDEBUG) $(LIBS) -o $(PROGRAMDEBUG)
#@echo "done"

# Commands (lower case) to invoke with make - invoke with "make command"

# Extra precaution for clean on delete
PHONY:		clean

# Clean (delete) objects
clean:;		@rm -f $(OBJS) core

# Clobber (delete) objects and main program
clobber:;	@rm -f $(OBJS) $(PROGRAM) core tags

# Dependencies - may not work
depend:;	@mkmf -f $(MAKEFILE) ROOT=$(ROOT)

# Echo headers and sources to stdout
echo:;		@echo $(HDRS) $(SRCS)

# Index - may not work
index:	 	$(HDRS) $(SRCS);
	@cd ${SRCPATH} && ctags -wx *.h *.c

# Install - test at your own risk.... need to edit
install:	$(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		@-strip $(PROGRAM)
		@if [ $(DEST) != . ]; then \
		(rm -f $(DEST)/$(PROGRAM); $(INSTALL) -f $(DEST) $(PROGRAM)); fi

# Print headers and sources
print:;		@$(PRINT) $(HDRS) $(SRCS)

# Update tags - may not work
tags:           $(HDRS) $(SRCS);
	@cd ${SRCPATH} && ctags *.h *.c

# Update destination - may not work
update:		$(PROGRAM)

# Protect Objects in case of make hang/crash
PRECIOUS:	$(OBJS)

# These Rules allow for implementation of a stacked directory structure {
$(OBJPATH)/trex-r2.o: $(SRCPATH)/trex-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/percolationSMA.o: $(SRCPATH)/percolationSMA.c
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteTimeSeriesSMA.o: $(SRCPATH)/WriteTimeSeriesSMA.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSMAFile.o: $(SRCPATH)/ReadSMAFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeFinalState.o: $(SRCPATH)/ComputeFinalState.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeInitialState.o: $(SRCPATH)/ComputeInitialState.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FreeMemory.o: $(SRCPATH)/FreeMemory.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/Grid-r2.o: $(SRCPATH)/Grid-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/Initialize.o: $(SRCPATH)/Initialize.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/NewState.o: $(SRCPATH)/NewState.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadDataGroupA.o: $(SRCPATH)/ReadDataGroupA.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadDataGroupF.o: $(SRCPATH)/ReadDataGroupF.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInputFile.o: $(SRCPATH)/ReadInputFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/RunTime.o: $(SRCPATH)/RunTime.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/SimulationError-r2.o: $(SRCPATH)/SimulationError-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/StripString.o: $(SRCPATH)/StripString.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/TimeFunctionInit.o: $(SRCPATH)/TimeFunctionInit.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/UpdateTimeFunction.o: $(SRCPATH)/UpdateTimeFunction.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/UtilityFunctions.o: $(SRCPATH)/UtilityFunctions.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteDumpFile.o: $(SRCPATH)/WriteDumpFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteEndGrids.o: $(SRCPATH)/WriteEndGrids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteGrids.o: $(SRCPATH)/WriteGrids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteMassBalance.o: $(SRCPATH)/WriteMassBalance.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteSummary.o: $(SRCPATH)/WriteSummary.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteTimeSeries.o: $(SRCPATH)/WriteTimeSeries.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelWaterDepth-r7.o: $(SRCPATH)/ChannelWaterDepth-r7.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelWaterRoute-r8.o: $(SRCPATH)/ChannelWaterRoute-r8.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeChannelElevation.o: $(SRCPATH)/ComputeChannelElevation.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeChannelLength.o: $(SRCPATH)/ComputeChannelLength.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeChannelTopology-r5.o: $(SRCPATH)/ComputeChannelTopology-r5.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeFinalStateWater.o: $(SRCPATH)/ComputeFinalStateWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeInitialStateWater.o: $(SRCPATH)/ComputeInitialStateWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FloodplainWaterTransfer.o: $(SRCPATH)/FloodplainWaterTransfer.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FreeMemoryWater-r3.o: $(SRCPATH)/FreeMemoryWater-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/Infiltration-r3.o: $(SRCPATH)/Infiltration-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/InitializeWater-r4-NOAA.o: $(SRCPATH)/InitializeWater-r4-NOAA.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/Interception-r2.o: $(SRCPATH)/Interception-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/NewStateWater.o: $(SRCPATH)/NewStateWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandWaterDepth-r5.o: $(SRCPATH)/OverlandWaterDepth-r5.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandWaterRoute.o: $(SRCPATH)/OverlandWaterRoute.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/Rainfall-r3-NOAA.o: $(SRCPATH)/Rainfall-r3-NOAA.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadChannelFile.o: $(SRCPATH)/ReadChannelFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadChannelTransmissionFile.o: $(SRCPATH)/ReadChannelTransmissionFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadDataGroupB-r5-NOAA.o: $(SRCPATH)/ReadDataGroupB-r5-NOAA.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadElevationFile.o: $(SRCPATH)/ReadElevationFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialInfiltrationFile.o: $(SRCPATH)/ReadInitialInfiltrationFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialTransmissionLossFile.o: $(SRCPATH)/ReadInitialTransmissionLossFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialWaterChannelFile-r2.o: $(SRCPATH)/ReadInitialWaterChannelFile-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialWaterOverlandFile-r2.o: $(SRCPATH)/ReadInitialWaterOverlandFile-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadLandUseFile.o: $(SRCPATH)/ReadLandUseFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadLinkFile.o: $(SRCPATH)/ReadLinkFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadMaskFile.o: $(SRCPATH)/ReadMaskFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadNodeFile-r2.o: $(SRCPATH)/ReadNodeFile-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadRainGridFile.o: $(SRCPATH)/ReadRainGridFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSoilTypeFile.o: $(SRCPATH)/ReadSoilTypeFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadStorageDepthFile.o: $(SRCPATH)/ReadStorageDepthFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/TimeFunctionInitWater.o: $(SRCPATH)/TimeFunctionInitWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/TransmissionLoss.o: $(SRCPATH)/TransmissionLoss.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/UpdateTimeFunctionWater.o: $(SRCPATH)/UpdateTimeFunctionWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WaterBalance.o: $(SRCPATH)/WaterBalance.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteMassBalanceWater.o: $(SRCPATH)/WriteMassBalanceWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WaterTransport-r2.o: $(SRCPATH)/WaterTransport-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteEndGridsWater.o: $(SRCPATH)/WriteEndGridsWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteGridsWater.o: $(SRCPATH)/WriteGridsWater.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteSummaryWater-r3.o: $(SRCPATH)/WriteSummaryWater-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteTimeSeriesWater.o: $(SRCPATH)/WriteTimeSeriesWater.c
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsAdvection-r2.o: $(SRCPATH)/ChannelSolidsAdvection-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsConcentration-r3.o: $(SRCPATH)/ChannelSolidsConcentration-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsDeposition.o: $(SRCPATH)/ChannelSolidsDeposition.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsDispersion.o: $(SRCPATH)/ChannelSolidsDispersion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsErosion.o: $(SRCPATH)/ChannelSolidsErosion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsKinetics.o: $(SRCPATH)/ChannelSolidsKinetics.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsMineralization.o: $(SRCPATH)/ChannelSolidsMineralization.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsTransportCapacity-r2.o: $(SRCPATH)/ChannelSolidsTransportCapacity-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelSolidsYield.o: $(SRCPATH)/ChannelSolidsYield.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeFinalStateSolids.o: $(SRCPATH)/ComputeFinalStateSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeInitialStateSolids.o: $(SRCPATH)/ComputeInitialStateSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeSolidsGroupConcSums.o: $(SRCPATH)/ComputeSolidsGroupConcSums.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeSolidsGroupTransportSums.o: $(SRCPATH)/ComputeSolidsGroupTransportSums.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FloodplainSolidsTransfer.o: $(SRCPATH)/FloodplainSolidsTransfer.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FreeMemorySolids.o: $(SRCPATH)/FreeMemorySolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/InitializeSolids-r2.o: $(SRCPATH)/InitializeSolids-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/NewStateSolids.o: $(SRCPATH)/NewStateSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/NewStateStack.o: $(SRCPATH)/NewStateStack.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsAdvection-r2.o: $(SRCPATH)/OverlandSolidsAdvection-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsConcentration-r3.o: $(SRCPATH)/OverlandSolidsConcentration-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsDeposition.o: $(SRCPATH)/OverlandSolidsDeposition.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsDispersion.o: $(SRCPATH)/OverlandSolidsDispersion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsErosion.o: $(SRCPATH)/OverlandSolidsErosion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsKinetics.o: $(SRCPATH)/OverlandSolidsKinetics.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsMineralization.o: $(SRCPATH)/OverlandSolidsMineralization.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsTransportCapacity-r3.o: $(SRCPATH)/OverlandSolidsTransportCapacity-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandSolidsYield.o: $(SRCPATH)/OverlandSolidsYield.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/PopSedimentStack.o: $(SRCPATH)/PopSedimentStack.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/PopSoilStack.o: $(SRCPATH)/PopSoilStack.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/PushSedimentStack-r2.o: $(SRCPATH)/PushSedimentStack-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/PushSoilStack.o: $(SRCPATH)/PushSoilStack.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadDataGroupC-r4.o: $(SRCPATH)/ReadDataGroupC-r4.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialSolidsChannelFile.o: $(SRCPATH)/ReadInitialSolidsChannelFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialSolidsOverlandFile.o: $(SRCPATH)/ReadInitialSolidsOverlandFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSedimentPropertiesFile.o: $(SRCPATH)/ReadSedimentPropertiesFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSoilLayerThicknessFile.o: $(SRCPATH)/ReadSoilLayerThicknessFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSoilStackFile.o: $(SRCPATH)/ReadSoilStackFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSoilTypeFile-r2.o: $(SRCPATH)/ReadSoilTypeFile-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/SolidsBalance.o: $(SRCPATH)/SolidsBalance.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/SolidsPropertyInit.o: $(SRCPATH)/SolidsPropertyInit.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/SolidsTransport.o: $(SRCPATH)/SolidsTransport.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/TimeFunctionInitSolids.o: $(SRCPATH)/TimeFunctionInitSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/UpdateTimeFunctionSolids.o: $(SRCPATH)/UpdateTimeFunctionSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteEndGridsSolids.o: $(SRCPATH)/WriteEndGridsSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteGridsSolids.o: $(SRCPATH)/WriteGridsSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteMassBalanceSolids.o: $(SRCPATH)/WriteMassBalanceSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteSummarySolids.o: $(SRCPATH)/WriteSummarySolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteTimeSeriesSolids.o: $(SRCPATH)/WriteTimeSeriesSolids.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/InitializeEnvironment.o: $(SRCPATH)/InitializeEnvironment.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FreeMemoryEnvironment.o: $(SRCPATH)/FreeMemoryEnvironment.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadDataGroupE.o: $(SRCPATH)/ReadDataGroupE.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadGeneralEnvironmentFile.o: $(SRCPATH)/ReadGeneralEnvironmentFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadGeneralTFPointerFile.o: $(SRCPATH)/ReadGeneralTFPointerFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadOverlandEnvironmentFile.o: $(SRCPATH)/ReadOverlandEnvironmentFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadOverlandEnvironmentTFPointerFile.o: $(SRCPATH)/ReadOverlandEnvironmentTFPointerFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadChannelEnvironmentFile.o: $(SRCPATH)/ReadChannelEnvironmentFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadChannelFpocFile.o: $(SRCPATH)/ReadChannelFpocFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadOverlandFpocFile.o: $(SRCPATH)/ReadOverlandFpocFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadOverlandFpocTFPointerFile.o: $(SRCPATH)/ReadOverlandFpocTFPointerFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/TimeFunctionInitEnvironment.o: $(SRCPATH)/TimeFunctionInitEnvironment.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/UpdateEnvironment.o: $(SRCPATH)/UpdateEnvironment.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/UpdateTimeFunctionEnvironment.o: $(SRCPATH)/UpdateTimeFunctionEnvironment.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalAdvection-r2.o: $(SRCPATH)/ChannelChemicalAdvection-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalBiodegradation.o: $(SRCPATH)/ChannelChemicalBiodegradation.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalConcentration-r3.o: $(SRCPATH)/ChannelChemicalConcentration-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalDeposition.o: $(SRCPATH)/ChannelChemicalDeposition.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalDispersion.o: $(SRCPATH)/ChannelChemicalDispersion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalDissolution.o: $(SRCPATH)/ChannelChemicalDissolution.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalErosion.o: $(SRCPATH)/ChannelChemicalErosion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalHydrolysis.o: $(SRCPATH)/ChannelChemicalHydrolysis.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalKinetics.o: $(SRCPATH)/ChannelChemicalKinetics.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalOxidation.o: $(SRCPATH)/ChannelChemicalOxidation.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalPartitioning.o: $(SRCPATH)/ChannelChemicalPartitioning.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalPhotolysis.o: $(SRCPATH)/ChannelChemicalPhotolysis.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalRadioactive.o: $(SRCPATH)/ChannelChemicalRadioactive.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalTransmissionLoss.o: $(SRCPATH)/ChannelChemicalTransmissionLoss.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalUDReaction.o: $(SRCPATH)/ChannelChemicalUDReaction.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalVolatilization.o: $(SRCPATH)/ChannelChemicalVolatilization.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChannelChemicalYield.o: $(SRCPATH)/ChannelChemicalYield.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChemicalBalance.o: $(SRCPATH)/ChemicalBalance.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChemicalPropertyInit.o: $(SRCPATH)/ChemicalPropertyInit.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ChemicalTransport.o: $(SRCPATH)/ChemicalTransport.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeChemicalGroupConcSums.o: $(SRCPATH)/ComputeChemicalGroupConcSums.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeChemicalGroupTransportSums.o: $(SRCPATH)/ComputeChemicalGroupTransportSums.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeChemicalGroupPhaseAve.o: $(SRCPATH)/ComputeChemicalGroupPhaseAve.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeFinalStateChemical.o: $(SRCPATH)/ComputeFinalStateChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ComputeInitialStateChemical.o: $(SRCPATH)/ComputeInitialStateChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FloodplainChemicalTransfer.o: $(SRCPATH)/FloodplainChemicalTransfer.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/FreeMemoryChemical.o: $(SRCPATH)/FreeMemoryChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/InitializeChemical.o: $(SRCPATH)/InitializeChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/NewStateChemical.o: $(SRCPATH)/NewStateChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalAdvection-r2.o: $(SRCPATH)/OverlandChemicalAdvection-r2.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalBiodegradation.o: $(SRCPATH)/OverlandChemicalBiodegradation.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalConcentration-r3.o: $(SRCPATH)/OverlandChemicalConcentration-r3.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalDeposition.o: $(SRCPATH)/OverlandChemicalDeposition.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalDispersion.o: $(SRCPATH)/OverlandChemicalDispersion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalDissolution.o: $(SRCPATH)/OverlandChemicalDissolution.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalErosion.o: $(SRCPATH)/OverlandChemicalErosion.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalHydrolysis.o: $(SRCPATH)/OverlandChemicalHydrolysis.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalInfiltration.o: $(SRCPATH)/OverlandChemicalInfiltration.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalKinetics.o: $(SRCPATH)/OverlandChemicalKinetics.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalOxidation.o: $(SRCPATH)/OverlandChemicalOxidation.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalPartitioning.o: $(SRCPATH)/OverlandChemicalPartitioning.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalPhotolysis.o: $(SRCPATH)/OverlandChemicalPhotolysis.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalRadioactive.o: $(SRCPATH)/OverlandChemicalRadioactive.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalUDReaction.o: $(SRCPATH)/OverlandChemicalUDReaction.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalVolatilization.o: $(SRCPATH)/OverlandChemicalVolatilization.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/OverlandChemicalYield.o: $(SRCPATH)/OverlandChemicalYield.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadDataGroupD.o: $(SRCPATH)/ReadDataGroupD.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialChemicalChannelFile.o: $(SRCPATH)/ReadInitialChemicalChannelFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadInitialChemicalOverlandFile.o: $(SRCPATH)/ReadInitialChemicalOverlandFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSedimentChemicalFile.o: $(SRCPATH)/ReadSedimentChemicalFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/ReadSoilLayerChemicalFile.o: $(SRCPATH)/ReadSoilLayerChemicalFile.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/TimeFunctionInitChemical.o: $(SRCPATH)/TimeFunctionInitChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/UpdateTimeFunctionChemical.o: $(SRCPATH)/UpdateTimeFunctionChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteEndGridsChemical.o: $(SRCPATH)/WriteEndGridsChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteGridsChemical.o: $(SRCPATH)/WriteGridsChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteMassBalanceChemical.o: $(SRCPATH)/WriteMassBalanceChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteSummaryChemical.o: $(SRCPATH)/WriteSummaryChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
$(OBJPATH)/WriteTimeSeriesChemical.o: $(SRCPATH)/WriteTimeSeriesChemical.c 
	${CC} ${CFLAGS} -c $<
	@make -k -f ./$(MAKEFILE) ${@F}
	@-[ -f ${@F} ] && mv ${@F} $(OBJPATH) || :
# }

# Include dependencies for each object
# these can be generated automatically using makedepend
# but be careful -- makedepend will change this file. 
# {
./$(OBJPATH)/percolationSMA.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/percolationSMA.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/percolationSMA.o: ./$(HDRPATH)/trex_SMA_declarations.h
./$(OBJPATH)/ReadLinkFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadLinkFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadLinkFile.o: ./$(HDRPATH)/trex_SMA_declarations.h
./$(OBJPATH)/ChannelChemicalAdvection-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalAdvection-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalAdvection-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalAdvection-r2.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalAdvection-r2.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalBiodegradation.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalBiodegradation.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalBiodegradation.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalBiodegradation.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalBiodegradation.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalConcentration-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalConcentration-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalConcentration-r3.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalConcentration-r3.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalConcentration-r3.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalDeposition.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalDeposition.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalDeposition.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalDeposition.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalDeposition.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalDispersion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalDispersion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalDispersion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalDispersion.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalDispersion.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalDissolution.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalDissolution.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalDissolution.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalDissolution.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalDissolution.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalErosion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalErosion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalErosion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalErosion.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalErosion.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalHydrolysis.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalHydrolysis.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalHydrolysis.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalHydrolysis.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalHydrolysis.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalKinetics.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalKinetics.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalKinetics.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalKinetics.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalKinetics.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalOxidation.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalOxidation.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalOxidation.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalOxidation.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalOxidation.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalPartitioning.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalPartitioning.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalPartitioning.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalPartitioning.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalPartitioning.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalPhotolysis.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalPhotolysis.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalPhotolysis.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalPhotolysis.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalPhotolysis.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalRadioactive.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalRadioactive.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalRadioactive.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalRadioactive.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalRadioactive.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalTransmissionLoss.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalTransmissionLoss.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalTransmissionLoss.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalTransmissionLoss.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalTransmissionLoss.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalUDReaction.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalUDReaction.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalUDReaction.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalUDReaction.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalUDReaction.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalVolatilization.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalVolatilization.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalVolatilization.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalVolatilization.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalVolatilization.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelChemicalYield.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelChemicalYield.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelChemicalYield.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelChemicalYield.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChannelChemicalYield.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ChannelSolidsAdvection-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsAdvection-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsAdvection-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsConcentration-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsConcentration-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsConcentration-r3.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsDeposition.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsDeposition.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsDeposition.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsDispersion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsDispersion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsDispersion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsErosion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsErosion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsErosion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsKinetics.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsKinetics.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsKinetics.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsMineralization.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsMineralization.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsMineralization.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsTransportCapacity-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsTransportCapacity-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsTransportCapacity-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelSolidsYield.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelSolidsYield.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelSolidsYield.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChannelWaterDepth-r7.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelWaterDepth-r7.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChannelWaterRoute-r8.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChannelWaterRoute-r8.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChemicalBalance.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChemicalBalance.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChemicalBalance.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChemicalBalance.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChemicalPropertyInit.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChemicalPropertyInit.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChemicalPropertyInit.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChemicalPropertyInit.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ChemicalTransport.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ChemicalTransport.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ChemicalTransport.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ChemicalTransport.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeChannelElevation.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeChannelElevation.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeChannelLength.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeChannelLength.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeChannelTopology-r5.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeChannelTopology-r5.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeChemicalGroupConcSums.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeChemicalGroupConcSums.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeChemicalGroupConcSums.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeChemicalGroupConcSums.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeChemicalGroupPhaseAve.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeChemicalGroupPhaseAve.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeChemicalGroupPhaseAve.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeChemicalGroupPhaseAve.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeChemicalGroupTransportSums.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeChemicalGroupTransportSums.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeChemicalGroupTransportSums.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeChemicalGroupTransportSums.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeFinalState.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeFinalState.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeFinalState.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeFinalState.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeFinalStateChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeFinalStateChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeFinalStateChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeFinalStateChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeFinalStateSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeFinalStateSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeFinalStateSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeFinalStateSolids.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeFinalStateWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeFinalStateWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeInitialState.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeInitialState.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeInitialState.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeInitialState.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeInitialStateChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeInitialStateChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeInitialStateChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeInitialStateChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ComputeInitialStateSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeInitialStateSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeInitialStateSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeInitialStateWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeInitialStateWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeSolidsGroupConcSums.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeSolidsGroupConcSums.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeSolidsGroupConcSums.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ComputeSolidsGroupTransportSums.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ComputeSolidsGroupTransportSums.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ComputeSolidsGroupTransportSums.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/FloodplainChemicalTransfer.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/FloodplainChemicalTransfer.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/FloodplainChemicalTransfer.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/FloodplainChemicalTransfer.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/FloodplainChemicalTransfer.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/FloodplainSolidsTransfer.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/FloodplainSolidsTransfer.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/FloodplainSolidsTransfer.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/FloodplainWaterTransfer.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/FloodplainWaterTransfer.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/FreeMemory.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/FreeMemory.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/FreeMemory.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/FreeMemory.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/FreeMemoryChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/FreeMemoryChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/FreeMemoryChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/FreeMemoryChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/FreeMemoryEnvironment.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/FreeMemoryEnvironment.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/FreeMemoryEnvironment.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/FreeMemoryEnvironment.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/FreeMemoryEnvironment.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/FreeMemorySolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/FreeMemorySolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/FreeMemorySolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/FreeMemoryWater-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/FreeMemoryWater-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/Grid-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/Grid-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/Grid-r2.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/Infiltration-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/Infiltration-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/Infiltration-r3.o: ./$(HDRPATH)/trex_SMA_declarations.h
./$(OBJPATH)/Initialize.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/Initialize.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/Initialize.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/Initialize.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/InitializeChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/InitializeChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/InitializeChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/InitializeChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/InitializeEnvironment.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/InitializeEnvironment.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/InitializeEnvironment.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/InitializeEnvironment.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/InitializeEnvironment.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/InitializeSolids-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/InitializeSolids-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/InitializeSolids-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/InitializeWater-r4-NOAA.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/InitializeWater-r4-NOAA.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/InitializeWater-r4.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/InitializeWater-r4.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/Interception-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/Interception-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/NewState.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/NewState.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/NewState.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/NewStateChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/NewStateChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/NewStateChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/NewStateChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/NewStateSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/NewStateSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/NewStateSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/NewStateStack.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/NewStateStack.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/NewStateStack.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/NewStateWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalAdvection-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalAdvection-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalAdvection-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalAdvection-r2.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalAdvection-r2.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalBiodegradation.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalBiodegradation.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalBiodegradation.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalBiodegradation.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalBiodegradation.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalConcentration-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalConcentration-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalConcentration-r3.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalConcentration-r3.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalConcentration-r3.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalDeposition.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalDeposition.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalDeposition.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalDeposition.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalDeposition.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalDispersion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalDispersion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalDispersion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalDispersion.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalDispersion.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalDissolution.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalDissolution.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalDissolution.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalDissolution.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalDissolution.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalErosion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalErosion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalErosion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalErosion.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalErosion.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalHydrolysis.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalHydrolysis.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalHydrolysis.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalHydrolysis.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalHydrolysis.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalInfiltration.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalInfiltration.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalInfiltration.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalInfiltration.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalInfiltration.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalKinetics.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalKinetics.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalKinetics.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalKinetics.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalKinetics.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalOxidation.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalOxidation.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalOxidation.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalOxidation.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalOxidation.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalPartitioning.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalPartitioning.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalPartitioning.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalPartitioning.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalPartitioning.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalPhotolysis.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalPhotolysis.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalPhotolysis.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalPhotolysis.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalPhotolysis.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalRadioactive.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalRadioactive.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalRadioactive.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalRadioactive.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalRadioactive.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalUDReaction.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalUDReaction.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalUDReaction.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalUDReaction.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalUDReaction.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalVolatilization.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalVolatilization.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalVolatilization.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalVolatilization.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalVolatilization.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandChemicalYield.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandChemicalYield.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandChemicalYield.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandChemicalYield.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/OverlandChemicalYield.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/OverlandSolidsAdvection-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsAdvection-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsAdvection-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsConcentration-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsConcentration-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsConcentration-r3.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsDeposition.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsDeposition.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsDeposition.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsDispersion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsDispersion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsDispersion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsErosion.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsErosion.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsErosion.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsKinetics.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsKinetics.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsKinetics.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsMineralization.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsMineralization.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsMineralization.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsTransportCapacity-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsTransportCapacity-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsTransportCapacity-r3.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandSolidsYield.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandSolidsYield.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandSolidsYield.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/OverlandWaterDepth-r5.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandWaterDepth-r5.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/OverlandWaterRoute.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/OverlandWaterRoute.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/PopSedimentStack.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/PopSedimentStack.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/PopSedimentStack.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/PopSedimentStack.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/PopSoilStack.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/PopSoilStack.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/PopSoilStack.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/PushSedimentStack-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/PushSedimentStack-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/PushSedimentStack-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/PushSedimentStack-r2.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/PushSoilStack.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/PushSoilStack.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/PushSoilStack.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/Rainfall-r3-NOAA.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/Rainfall-r3-NOAA.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/Rainfall-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadChannelEnvironmentFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadChannelEnvironmentFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadChannelEnvironmentFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadChannelEnvironmentFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadChannelEnvironmentFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadChannelFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadChannelFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadChannelFpocFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadChannelFpocFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadChannelFpocFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadChannelFpocFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadChannelFpocFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadChannelTransmissionFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadChannelTransmissionFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadDataGroupA.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadDataGroupB-r5-NOAA.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadDataGroupB-r5-NOAA.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadDataGroupB-r5.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadDataGroupB-r5.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadDataGroupC-r4.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadDataGroupC-r4.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadDataGroupC-r4.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadDataGroupD.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadDataGroupD.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadDataGroupD.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadDataGroupD.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadDataGroupE.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadDataGroupE.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadDataGroupE.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadDataGroupE.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadDataGroupE.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadDataGroupF.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadDataGroupF.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadDataGroupF.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadDataGroupF.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadElevationFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadElevationFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadElevationFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadGeneralEnvironmentFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadGeneralEnvironmentFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadGeneralEnvironmentFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadGeneralEnvironmentFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadGeneralEnvironmentFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadGeneralTFPointerFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadGeneralTFPointerFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadGeneralTFPointerFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadGeneralTFPointerFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadGeneralTFPointerFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadInitialChemicalChannelFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialChemicalChannelFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInitialChemicalChannelFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadInitialChemicalChannelFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadInitialChemicalOverlandFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialChemicalOverlandFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInitialChemicalOverlandFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadInitialChemicalOverlandFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadInitialInfiltrationFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialInfiltrationFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInitialInfiltrationFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadInitialSolidsChannelFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialSolidsChannelFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInitialSolidsChannelFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadInitialSolidsOverlandFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialSolidsOverlandFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInitialSolidsOverlandFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadInitialTransmissionLossFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialTransmissionLossFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInitialWaterChannelFile-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialWaterChannelFile-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInitialWaterOverlandFile-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadInitialWaterOverlandFile-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInputFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadInputFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadInputFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadInputFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadLandUseFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadLandUseFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadLandUseFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadLinkFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadMaskFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadNodeFile-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadNodeFile-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentTFPointerFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentTFPointerFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentTFPointerFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentTFPointerFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadOverlandEnvironmentTFPointerFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadOverlandFpocFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadOverlandFpocFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadOverlandFpocFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadOverlandFpocFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadOverlandFpocFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadOverlandFpocTFPointerFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadOverlandFpocTFPointerFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadOverlandFpocTFPointerFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadOverlandFpocTFPointerFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadOverlandFpocTFPointerFile.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/ReadRainGridFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadRainGridFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadSedimentChemicalFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadSedimentChemicalFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadSedimentChemicalFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadSedimentChemicalFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadSedimentPropertiesFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadSedimentPropertiesFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadSedimentPropertiesFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadSoilLayerChemicalFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadSoilLayerChemicalFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadSoilLayerChemicalFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadSoilLayerChemicalFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/ReadSoilLayerThicknessFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadSoilLayerThicknessFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadSoilLayerThicknessFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadSoilStackFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadSoilStackFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadSoilStackFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadSoilTypeFile-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadSoilTypeFile-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/ReadSoilTypeFile-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/ReadStorageDepthFile.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/ReadStorageDepthFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/RunTime.o: ./$(SRCPATH)/RunTime.c
./$(OBJPATH)/SimulationError-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/SimulationError-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/SimulationError-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/SimulationError-r2.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/SolidsBalance.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/SolidsBalance.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/SolidsPropertyInit.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/SolidsPropertyInit.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/SolidsPropertyInit.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/SolidsTransport.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/SolidsTransport.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/SolidsTransport.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/StripString.o: ./$(SRCPATH)/StripString.c
./$(OBJPATH)/TimeFunctionInit.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/TimeFunctionInit.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/TimeFunctionInit.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/TimeFunctionInit.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/TimeFunctionInit.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/TimeFunctionInitChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/TimeFunctionInitChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/TimeFunctionInitChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/TimeFunctionInitChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/TimeFunctionInitEnvironment.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/TimeFunctionInitEnvironment.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/TimeFunctionInitEnvironment.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/TimeFunctionInitEnvironment.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/TimeFunctionInitEnvironment.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/TimeFunctionInitSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/TimeFunctionInitSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/TimeFunctionInitSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/TimeFunctionInitWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/TimeFunctionInitWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/TransmissionLoss.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/TransmissionLoss.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/UpdateEnvironment.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/UpdateEnvironment.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/UpdateEnvironment.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/UpdateEnvironment.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/UpdateEnvironment.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/UpdateTimeFunction.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/UpdateTimeFunction.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/UpdateTimeFunction.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/UpdateTimeFunction.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/UpdateTimeFunction.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/UpdateTimeFunctionChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/UpdateTimeFunctionChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/UpdateTimeFunctionChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/UpdateTimeFunctionChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/UpdateTimeFunctionEnvironment.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/UpdateTimeFunctionEnvironment.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/UpdateTimeFunctionEnvironment.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/UpdateTimeFunctionEnvironment.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/UpdateTimeFunctionEnvironment.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/UpdateTimeFunctionSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/UpdateTimeFunctionSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/UpdateTimeFunctionSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/UpdateTimeFunctionWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/UpdateTimeFunctionWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/UtilityFunctions.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WaterBalance.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WaterTransport-r2.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WaterTransport-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteDumpFile.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteDumpFile.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteDumpFile.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteEndGrids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteEndGrids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteEndGrids.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteEndGridsChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteEndGridsChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteEndGridsChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteEndGridsChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteEndGridsSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteEndGridsSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteEndGridsSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteEndGridsWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteEndGridsWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteGrids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteGrids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteGrids.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteGridsChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteGridsChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteGridsChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteGridsChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteGridsSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteGridsSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteGridsSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteGridsWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteGridsWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteMassBalance.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteMassBalance.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteMassBalance.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteMassBalance.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteMassBalanceChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteMassBalanceChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteMassBalanceChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteMassBalanceChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteMassBalanceSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteMassBalanceSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteMassBalanceSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteMassBalanceWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteMassBalanceWater.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteSummary.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteSummary.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteSummary.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteSummaryChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteSummaryChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteSummaryChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteSummaryChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteSummarySolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteSummarySolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteSummarySolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteSummaryWater-r3.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteSummaryWater-r3.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteTimeSeries.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteTimeSeries.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteTimeSeries.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteTimeSeries.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteTimeSeriesChemical.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteTimeSeriesChemical.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteTimeSeriesChemical.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteTimeSeriesChemical.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/WriteTimeSeriesSolids.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteTimeSeriesSolids.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/WriteTimeSeriesSolids.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/WriteTimeSeriesWater.o: ./$(HDRPATH)/trex_general_declarations.h
./$(OBJPATH)/WriteTimeSeriesWater.o: ./$(HDRPATH)/trex_water_declarations.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_general_declarations.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_general_definitions.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_water_declarations.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_water_definitions.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_solids_declarations.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_SMA_declarations.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_SMA_definitions.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_solids_definitions.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_chemical_declarations.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_chemical_definitions.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_environmental_declarations.h
#./$(OBJPATH)/trex-r2-debugger.o: ./$(HDRPATH)/trex_environmental_definitions.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_general_definitions.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_water_declarations.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_water_definitions.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_SMA_declarations.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_SMA_definitions.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_solids_declarations.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_solids_definitions.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_chemical_declarations.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_chemical_definitions.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_environmental_declarations.h
./$(OBJPATH)/trex-r2.o: ./$(HDRPATH)/trex_environmental_definitions.h
# }
# DO NOT DELETE

# SVN Info: $Id$

#####################################################################
##                           TAPSMaintain                          ##
##                                                                 ##
##               Universal TAPS maintenance software               ##
##                                                                 ##
##                                                                 ##
##                           (C) 2008 by                           ##
##      Dominik Werthmueller <dominik.werthmueller@unibas.ch>      ##
##                                                                 ##
#####################################################################



# --------------------------- System and ROOT variables ---------------------------

S             = src
I             = include
O             = obj
L             = lib
B             = bin
LTCPIP        = libtcpip

SRC           = $(wildcard $(S)/TM*.cxx) $(S)/Dict.cxx
INCD          = $(wildcard $(I)/TM*.h)
INC           = $(notdir $(INCD))
OBJD          = $(patsubst $(S)/%.cxx, $(O)/%.o, $(SRC))
OBJ           = $(notdir $(OBJD))

OSTYPE       := $(subst -,,$(shell uname))

ROOTGLIBS    := $(shell root-config --glibs) -lASImage
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)

BIN_INSTALL_DIR = $(HOME)/$(B)

vpath %.cxx $(S)
vpath %.h  $(I)
vpath %.o  $(O)

# ------------------------ Architecture dependent settings ------------------------

ifeq ($(OSTYPE),Darwin)
	SOFLAGS = -dynamiclib -single_module -undefined dynamic_lookup -install_name $(CURDIR)/$(L)/libTAPSMaintain.dylib
	LIBTM = $(L)/libTAPSMaintain.dylib
	POST_LIB_BUILD = @ln $(L)/libTAPSMaintain.dylib $(L)/libTAPSMaintain.so
endif

ifeq ($(OSTYPE),Linux)
	SOFLAGS = -shared
	LIBTM = $(L)/libTAPSMaintain.so
	POST_LIB_BUILD = 
endif

# -------------------------------- Compile options --------------------------------

CCCOMP      = g++
CCOMP       = gcc
CXXFLAGS    = -O3 -g -Wall -fPIC $(ROOTCFLAGS) -I./$(I)
LDFLAGS     = -O3 -g $(ROOTLDFLAGS)

# ------------------------------------ targets ------------------------------------

all:	begin $(L)/libTAPSMaintain.so $(B)/TAPSMaintain end

begin:
	@echo
	@echo "-> Building TAPSMaintain on a $(OSTYPE) system"
	@echo

end:
	@echo
	@echo "-> Finished!"
	@echo

$(B)/TAPSMaintain: $(L)/libTAPSMaintain.so $(L)/libtcpip.so $(S)/MainTAPSMaintain.cxx
	@echo
	@echo "Building TAPSMaintain application ..."
	@mkdir -p $(B)
	@$(CCCOMP) $(CXXFLAGS) $(ROOTGLIBS) $(CURDIR)/$(LIBTM) $(CURDIR)/$(L)/libtcpip.so -o $(B)/TAPSMaintain $(S)/MainTAPSMaintain.cxx

$(L)/libTAPSMaintain.so: $(OBJ)
	@echo
	@echo "Building libTAPSMaintain ..."
	@mkdir -p $(L)
	@rm -f $(LIBTM) $(L)/libTAPSMaintain.so
	@$(CCCOMP) $(LDFLAGS) $(ROOTGLIBS) $(SOFLAGS) $(OBJD) -o $(LIBTM)
	@$(POST_LIB_BUILD)

$(L)/libtcpip.so: $(LTCPIP)/hw_write.c $(LTCPIP)/tcpip.c $(LTCPIP)/checkopts.c
	@echo
	@echo "Building TAPS Slowcontrol libtcpip ..."
	@mkdir -p $(L)
	@$(CCOMP) -shared -O3 -DOSF1 -DLINUX -DLIBC6 -DWARPFILL $(LTCPIP)/hw_write.c \
	          $(LTCPIP)/tcpip.c $(LTCPIP)/checkopts.c -o $(L)/libtcpip.so

$(S)/Dict.cxx: $(INC) $(I)/LinkDef.h 
	@echo
	@echo "Creating TAPSMaintain dictionary ..."
	@rootcint -v -f $@ -c -I./$(I) -p $(INC) $(I)/LinkDef.h

%.o: %.cxx
	@echo "Compiling $(notdir $<) ..."
	@mkdir -p $(O)
	@$(CCCOMP) $(CXXFLAGS) -o $(O)/$@ -c $< 

docs:
	@echo "Creating HTML documentation ..."
	@rm -r -f htmldoc
	@root -b -n -q $(S)/htmldoc.C > /dev/null 2>&1
	@echo "Done."

install: $(B)/TAPSMaintain
	@echo "Installing TAPSMaintain in $(BIN_INSTALL_DIR) ..."
	@mkdir -p $(BIN_INSTALL_DIR)
	@cp $(B)/TAPSMaintain $(BIN_INSTALL_DIR) 
	@echo "Done."

uninstall:
	@echo "Uninstalling TAPSMaintain ..."
	@rm -f $(BIN_INSTALL_DIR)/TAPSMaintain
	@echo "Done."
	
clean:
	@echo "Cleaning TAPSMaintain distribution ..."
	@rm -f $(S)/Dict.*
	@rm -r -f $(L)
	@rm -f -r $(O)
	@rm -r -f $(B)
	@rm -r -f htmldoc
	@echo "Done."
  

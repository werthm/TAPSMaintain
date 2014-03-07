#####################################################################
##                           TAPSMaintain                          ##
##                                                                 ##
##               Universal TAPS maintenance software               ##
##                                                                 ##
##                                                                 ##
##                       (C) 2008-2013 by                          ##
##      Dominik Werthmueller <dominik.werthmueller@unibas.ch>      ##
##                                                                 ##
#####################################################################


# --------------------------- System and ROOT variables ---------------------------

S             = src
I             = include
O             = obj
L             = lib
B             = bin

SRC           = $(wildcard $(S)/TM*.cxx) $(S)/G__TAPSMaintain.cxx
INCD          = $(wildcard $(I)/TM*.h)
INC           = $(notdir $(INCD))
OBJD          = $(patsubst $(S)/%.cxx, $(O)/%.o, $(SRC))
OBJ           = $(notdir $(OBJD))

ROOTGLIBS     = $(shell root-config --glibs) -lASImage
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLDFLAGS   = $(shell root-config --ldflags)

BIN_INST_DIR  = $(HOME)/$(B)

LIB_TM        = $(L)/libTAPSMaintain.so
SOFLAGS_TM    = -shared

vpath %.cxx $(S)
vpath %.h  $(I)
vpath %.o  $(O)

# -------------------------------- Compile options --------------------------------

CCCOMP      = g++
CXXFLAGS    = -g -O3  -Wall -fPIC $(ROOTCFLAGS) -I./$(I) -I$(TAPSSC)/include
LDFLAGS     = -g -O3  $(ROOTLDFLAGS)

# ------------------------------------ targets ------------------------------------

all:	begin $(LIB_TM) $(B)/TAPSMaintain end

begin:
	@echo
	@echo "-> Building TAPSMaintain"
	@echo

end:
	@echo
	@echo "-> Finished!"
	@echo

$(B)/TAPSMaintain: $(LIB_TM) $(S)/MainTAPSMaintain.cxx
	@echo
	@echo "Building TAPSMaintain application ..."
	@mkdir -p $(B)
	@$(CCCOMP) $(CXXFLAGS) $(ROOTGLIBS) $(CURDIR)/$(LIB_TM) -L$(TAPSSC)/lib -lTAPSsc -o \
        $(B)/TAPSMaintain $(S)/MainTAPSMaintain.cxx

$(LIB_TM): $(OBJ)
	@echo
	@echo "Building libTAPSMaintain ..."
	@mkdir -p $(L)
	@rm -f $(L)/libTAPSMaintain.*
	@$(CCCOMP) $(LDFLAGS) $(ROOTGLIBS) $(SOFLAGS_TM) $(OBJD) -o $(LIB_TM)

$(S)/G__TAPSMaintain.cxx: $(INC) $(I)/LinkDef.h 
	@echo
	@echo "Creating TAPSMaintain dictionary ..."
	@rootcint -v -f $@ -c -I./$(I) -I$(TAPSSC)/include -p $(INC) $(I)/LinkDef.h

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
	@echo "Installing TAPSMaintain in $(BIN_INST_DIR) ..."
	@mkdir -p $(BIN_INST_DIR)
	@cp $(B)/TAPSMaintain $(BIN_INST_DIR) 
	@echo "Done."

uninstall:
	@echo "Uninstalling TAPSMaintain ..."
	@rm -f $(BIN_INST_DIR)/TAPSMaintain
	@echo "Done."
	
clean:
	@echo "Cleaning TAPSMaintain distribution ..."
	@rm -f $(S)/G__*
	@rm -r -f $(L)
	@rm -f -r $(O)
	@rm -r -f $(B)
	@rm -r -f htmldoc
	@echo "Done."
  

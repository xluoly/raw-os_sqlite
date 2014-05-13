##############################################################################################
# Start of default section
#

CC   = gcc -g

# List all default C defines here, like -D_DEBUG=1
DDEFS = -D__MINGW32__ -DUNICODE -D_UNICODE -DWIN32 -DRAW_OS -DUSE_FATFS -D_HAVE_SQLITE_CONFIG_H

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS = -lws2_32 -lgdi32 -lwinmm

# Make sure this empty for now
SRC = 

#
# End of default section
##############################################################################################

##############################################################################################
# Start of user section
#

# Define project name here
PROJECT = sqlite

# Imported source files and paths
ROOT = ../../..
PORT_DIR = $(ROOT)/port/win32

VPATH += $(ROOT)

include $(PORT_DIR)/in.mk
include $(ROOT)/raw-os/in.mk
include $(ROOT)/fatfs/in.mk
include $(ROOT)/sqlite/in.mk
include $(ROOT)/rtc/in.mk
include $(ROOT)/examples/in.mk

# Define optimisation level here
OPT = -ggdb -O0 -fomit-frame-pointer

#
# End of user defines
##############################################################################################


# Output directory and files
ifeq ($(BUILDDIR),)
  BUILDDIR = .build
endif
ifeq ($(BUILDDIR),.)
  BUILDDIR = .build
endif

OBJDIR  = $(BUILDDIR)/obj
LSTDIR  = $(BUILDDIR)/lst
MAPDIR  = $(BUILDDIR)/map
DEPDIR  = $(BUILDDIR)/dep

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(INC))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
COBJ	= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
AOBJ	= $(addprefix $(OBJDIR)/, $(ASRC:.s=.o))
OBJS    = $(AOBJ) $(COBJ)
LIBS    = $(DLIBS) $(ULIBS)

LDFLAGS = -Wl,-Map=$(MAPDIR)/$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR)
CPFLAGS = $(OPT) $(DEFS) -Wall -Wextra -Wstrict-prototypes

# Generate dependency information
CPFLAGS += -MD -MP -MF $(DEPDIR)/$(@F).d

# Target
TARGET  = $(BUILDDIR)/$(PROJECT).exe

#
# makefile rules
#

all: $(BUILDDIR) $(OBJS) $(TARGET)


$(BUILDDIR) $(OBJDIR) $(LSTDIR) $(DEPDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(LSTDIR)
	mkdir -p $(MAPDIR)
	mkdir -p $(DEPDIR)

$(OBJDIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@echo
	$(CC) -c $(CPFLAGS) -I. $(INCDIR) $< -o $@

$(OBJDIR)/%.o : %.s
	@mkdir -p $(dir $@)
	@echo
	$(AS) -c $(ASFLAGS) -I. $(INCDIR) $< -o $@

$(TARGET): $(OBJS)
	@echo
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

gcov:
	-mkdir gcov
	$(COV) -u $(subst /,\,$(SRC))
	-mv *.gcov ./gcov

clean:
	-rm -fR $(BUILDDIR)

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir $(DEPDIR) 2>/dev/null) $(wildcard $(DEPDIR)/*)

# *** EOF ***

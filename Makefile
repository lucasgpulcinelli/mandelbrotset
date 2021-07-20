
MKDIR ?= mkdir -p
CHMOD ?= chmod
CP    ?= cp

BUILDD ?=./build
SRCD =./src
PYSRCD = ./py_src
SHAREDLIB  ?=$(BUILDD)/libmandel.so

C_FILES = $(wildcard $(SRCD)/*.c)
O_FILES = $(patsubst %.c,%.o, $(C_FILES))
PY_FILES = $(wildcard $(PYSRCD)/*.py)

CFLAGS +=-Wall -O3 -fPIC
LDFLAGS +=-ljpeg -lpng -shared


.PHONY: all clean pycopy

all: $(SHAREDLIB) pycopy

$(SHAREDLIB): $(O_FILES)
	@$(MKDIR) $(BUILDD)
	$(CC) $(LDFLAGS) $(O_FILES) -o $(SHAREDLIB)

pycopy: $(PY_FILES)
	@$(MKDIR) $(BUILDD)
	@$(CP) $(PY_FILES) $(BUILDD)
	@$(CHMOD) +x $(BUILDD)/*.py

clean:
	@$(RM) -f $(O_FILES)
	@$(RM) -f $(SHAREDLIB)

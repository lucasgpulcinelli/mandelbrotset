
MKDIR ?= mkdir -p

BUILDD ?=./build
SRCD =./src
SHAREDLIB  ?=$(BUILDD)/libmandel.so

C_FILES = $(wildcard $(SRCD)/*.c)
O_FILES = $(patsubst %.c,%.o, $(C_FILES))

CFLAGS +=-Wall -O3 -fPIC
LDFLAGS +=-ljpeg -lpng -shared


.PHONY: all clean

all: $(SHAREDLIB)

$(SHAREDLIB): $(O_FILES)
	@$(MKDIR) $(BUILDD)
	$(CC) $(LDFLAGS) $(O_FILES) -o $(EXECUTABLE)

clean:
	@$(RM) -f $(O_FILES)
	@$(RM) -f $(SHAREDLIB)

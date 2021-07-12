
MKDIR ?= mkdir -p

BUILDD?=./build
SRCD =./src

C_FILES = $(wildcard $(SRCD)/*.c)
O_FILES = $(patsubst %.c,%.o, $(C_FILES))

CFLAGS +=-Wall -O3
LDFLAGS +=-ljpeg -lpng

all: $(BUILDD)/main

$(BUILDD)/main: $(O_FILES)
	@$(MKDIR) $(BUILDD)
	$(CC) $(LDFLAGS) $(O_FILES) -o $(BUILDD)/main

clean:
	@$(RM) -f $(O_FILES)
	@$(RM) -f $(BUILDD)/main
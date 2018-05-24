INCLUDE_DIR = include
SOURCE_DIR = src
OBJECT_DIR = obj
CC = gcc
CFLAGS = -w -Wno-write-strings -g -I$(INCLUDE_DIR)
#CFLAGS = -w -Wno-write-strings -O2 -I$(INCLUDE_DIR) -DDEBUG
CC_SOURCES = $(wildcard src/*)
STRING_OBJ_AUX = $(CC_SOURCES:.c=.o)
STRING_OBJ = $(subst src/,,$(STRING_OBJ_AUX))
CC_OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(STRING_OBJ))
PROGRAM_NAME = pth_code
LINKER = -lm -lpthread

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(CC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKER)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LINKER) 

clean:
	rm -f $(OBJECT_DIR)/*.o $(PROGRAM_NAME)
	rm -f Outputs/*
	rm -f Plot/times.dat Plot/*.pdf

plot:
	python Plot/plot.py Plot/times.dat

print-%  : ; @echo $* = $($*)


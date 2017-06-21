CC = gcc
CFLAGS = -Wall -O2
LINKER = -lpthread
PROGRAM_NAME = hello


all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(PROGRAM_NAME).c
	$(CC) $(CFLAGS) -o $@ $< $(LINKER)

clean:
	rm -r $(PROGRAM_NAME)
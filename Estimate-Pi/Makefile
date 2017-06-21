CC = gcc
CFLAGS = -Wall -O2
LINKER = -lpthread -lm
PROGRAM_NAME = pth_pi_mutex


all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(PROGRAM_NAME).c
	$(CC) $(CFLAGS) -o $@ $< $(LINKER)

clean:
	rm -r $(PROGRAM_NAME)

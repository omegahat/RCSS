CC=gcc
CFLAGS=-c
XTRA_CFLAGS=$(shell pkg-config --cflags libcroco-0.6)

%.tu: %.c
	$(CC) -fdump-translation-unit $(CFLAGS) $(XTRA_CFLAGS) $< -o /dev/null

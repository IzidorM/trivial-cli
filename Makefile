APP_NAME=trivial_cli
SOURCES=trivial_cli.c
CFLAGS= -Og -g -ggdb3 -Wall -ffunction-sections -fdata-sections -std=c11 -pedantic
CC=gcc

DEFINES=
INCLUDE_PATH= -I.

all: 
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE_PATH) $(SOURCES) main.c -o $(APP_NAME)

test: 
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE_PATH) $(SOURCES) test.c -o test


rtags:
	rc --project-root . -c clang $(CFLAGS) $(DEFINES) $(INCLUDE_PATH) $(SOURCES)

clean:
	rm $(APP_NAME)

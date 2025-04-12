CC ?= gcc
CFLAGS = -I./include -Wall -Wextra -O3 -std=c11
LDFLAGS = -lpthread

ifeq ($(OS),Windows_NT)
    LDFLAGS += -lws2_32
    LIBEXT = .dll
else
    LIBEXT = .so
endif

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/ai/*.c $(SRC_DIR)/net/*.c $(SRC_DIR)/utils/*.c)
OBJ = $(SRC:.c=.o)
LIB = libswiftnetpp$(LIBEXT)

all: $(LIB)

$(LIB): $(OBJ)
	$(CC) -shared $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

clean:
	find src -name "*.o" -delete
	rm -f $(LIB)

install: $(LIB)
	mkdir -p /usr/local/include/snpp
	cp -r include/snpp/* /usr/local/include/snpp/
	cp $(LIB) /usr/local/lib/

.PHONY: all clean install
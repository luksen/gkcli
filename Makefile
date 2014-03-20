PREFIX = /usr/local/bin
TARGET = gkcli

CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Werror -D_XOPEN_SOURCE=600 -g
LDFLAGS = `pkg-config --cflags --libs gnome-keyring-1`

.PHONY: all clean install uninstall

all: ${TARGET}

${TARGET}: ${TARGET}.c
	${CC} ${CFLAGS} ${LDFLAGS} $< -o $@

clean:
	-rm ${TARGET}

install: ${TARGET}
	cp ${TARGET} ${PREFIX}

uninstall:
	-rm ${PREFIX}/${TARGET}

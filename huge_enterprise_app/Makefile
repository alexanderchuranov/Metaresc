
NUMFILES  ?= 100

BINARY = huge_enterprise_app

CFLAGS += -Wall -I../src `xml2-config --cflags`
LDADD += ../src/.libs/libreslib.a `xml2-config --libs`

all: ${BINARY}

SOURCES = $(shell seq -f file_%05g.c -s " " ${NUMFILES}) main.c

${BINARY}: $(patsubst %.c,%.o, ${SOURCES})
	${CC} -o $@ $? ${LDADD}

main.c: main.h

file_%.c: file_%.h main.h
	sed -e s/@FILENO@/$(patsubst file_%.c,%,$@)/ < file.c.in > $@

file_%.h:
	sed -e s/@FILENO@/$(patsubst file_%.h,%,$@)/ < file.h.in > $@

main.h: $(patsubst %.c, %.h, $(patsubst main.c, , ${SOURCES})) 
	echo -e "#ifndef _MAIN_H_\n#define _MAIN_H_\n" $(patsubst %,"#include \"%\"\n", $?) "#endif\n" > main.h
	
clean:
	${RM} ${BINARY} *.o

distclean:
	${RM} ${BINARY} *.o file_*.[ch] main.h

.PHONY: clean distclean all

.PRECIOUS: file_%.[ch]
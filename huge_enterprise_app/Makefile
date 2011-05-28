
NUMFILES  ?= 500
OPTIMIZED ?= NO

BINARY = huge_enterprise_app

CFLAGS += -Wall -I../src `xml2-config --cflags`
LDADD += ../src/.libs/libreslib.a

all: ${BINARY}

SOURCES = $(shell seq -f file_%05g.c -s " " ${NUMFILES}) main.c
OBJECTS = $(patsubst %.c,%.o, ${SOURCES})

${BINARY}: ${OBJECTS}
	${CC} ${CFLAGS} -o $@ $? ${LDADD}

${SOURCES}: generate

generate:
	./generate.sh ${NUMFILES} ${OPTIMIZED}
	
clean:
	${RM} ${BINARY} *.o

distclean:
	${RM} ${BINARY} *.o header_*.h file_*.c main.c

.PHONY: clean distclean


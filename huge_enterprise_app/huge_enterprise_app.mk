
BINARY = huge_enterprise_app
CFLAGS += -Wall -Werror -std=gnu99

CFLAGS += -I../src -I/usr/local/include/libxml2 -I/usr/local/include
LDADD += ../src/.libs/libreslib.a

OBJECTS != find . -not -type d -name 'file_*.c' | sed 's|.c$$|.o|'
OBJECTS += main.o

all: ${BINARY}

${BINARY}: ${OBJECTS}
	cc ${CFLAGS} -o ${.TARGET} ${.ALLSRC} ${LDADD}

clean:
	rm -f ${BINARY} *.o

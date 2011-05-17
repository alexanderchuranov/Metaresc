
NUMFILES  ?= 500
OPTIMIZED ?= NO

all: main.c
	make -f huge_enterprise_app.mk all

clean:
	make -f huge_enterprise_app.mk clean

distclean:
	rm -f header_*.h file_*.c main.c

.PHONY: clean distclean

main.c:
	./generate.sh ${NUMFILES} ${OPTIMIZED}

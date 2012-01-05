
SOURCE = user-guide.xml
HTML_DEST = ${HOME}/web/nginx-root/html/metaresc/user-guide

# docbook stuff

DOCBOOK_ROOT = /usr/local/share/xml/docbook/5.0

STYLESHEET = ./customization.xsl
SCHEMA = ${DOCBOOK_ROOT}/xsd/docbook.xsd

all: ${SOURCE}
	xsltproc --xinclude ${XSLT_PARAMS} ${STYLESHEET} $<

test: ${SOURCE}
	xmllint --xinclude --noout --schema ${SCHEMA} ${.ALLSRC}

deploy: all
	rm -f ${HTML_DEST}/*
	mv *html ${HTML_DEST}/

clean:
	rm -f *.html *.tmp

.PHONY: test clean


SOURCE = reference.xml
HTML_DEST = ${HOME}/web/nginx-root/html/metaresc/reference

# docbook stuff

STYLE ?= xhtml/chunk.xsl

XSLT_PARAMS = --param use.id.as.filename 1 --stringparam html.stylesheet '/css/metaresc/screen.css'

DOCBOOK_XSL_STYLES = /usr/local/share/xsl/docbook
DOCBOOK_ROOT = /usr/local/share/xml/docbook/5.0

all: ${SOURCE}
	xsltproc --xinclude ${XSLT_PARAMS} ${DOCBOOK_XSL_STYLES}/${STYLE} ${.ALLSRC}

clean:
	rm -f *.html *.tmp

test: ${SOURCE}
	xmllint --xinclude --noout --schema ${DOCBOOK_ROOT}/xsd/docbook.xsd ${.ALLSRC}

deploy: all
	rm -f ${HTML_DEST}/*
	mv *html ${HTML_DEST}/

.PHONY: test clean

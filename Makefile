all: grace2html/grace2html htparse/htparse mksite/mksite parsechanges/parsechanges xml2html/xml2html
	./build_site

grace2html/grace2html:
	cd grace2html  && make
	
htparse/htparse:
	cd htparse  && make
	
mksite/mksite:
	cd mksite  && make

parsechanges/parsechanges:
	cd parsechanges  && make

xml2html/xml2html:
	cd xml2html  && make

clean:
	rm -f toc.xml
	rm -rf site && mkdir site
	cd grace2html && make clean
	cd htparse && make clean
	cd mksite && make clean
	cd parsechanges && make clean
	cd xml2html && make clean

all-clean: clean
	rm -f */makeinclude
	rm -f */configure.paths
	rm -f */platform.h

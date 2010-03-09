all:
	cd grace2html && ./configure && make
	cd htparse && ./configure && make
	cd mksite && ./configure && make
	cd parsechanges && ./configure && make
	cd xml2html && ./configure && make
	./build_site

clean:
	rm -f toc.xml
	rm -rf site && mkdir site
	cd grace2html && make clean
	cd htparse && make clean
	cd mksite && make clean
	cd parsechanges && make clean
	cd xml2html && make clean

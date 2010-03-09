include makeinclude

OBJ	= main.o

all: xml2html

xml2html: $(OBJ)
	$(LD) $(LDFLAGS) -o xml2html $(OBJ) $(LIBS)

clean:
	rm -f *.o
	rm -f xml2html

allclean: clean
	rm -f makeinclude configure.paths platform.h
	
install: all
	./makeinstall

makeinclude:
	@echo please run ./configure
	@false

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

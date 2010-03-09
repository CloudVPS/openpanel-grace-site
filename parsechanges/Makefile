include makeinclude

OBJ	= main.o

all: parsechanges

parsechanges: $(OBJ)
	$(LD) $(LDFLAGS) -o parsechanges $(OBJ) $(LIBS)

clean:
	rm -f *.o
	rm -f parsechanges

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

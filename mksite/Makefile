include makeinclude

OBJ	= main.o

all: mksite

mksite: mksite.exe
	grace mkapp mksite

mksite.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o mksite.exe $(OBJ) $(LIBS)

clean:
	rm -f *.o *.exe
	rm -rf mksite.app
	rm -f mksite
	
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

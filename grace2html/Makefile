include makeinclude

OBJ	= main.o

all: grace2html

grace2html: $(OBJ)
	$(LD) $(LDFLAGS) -o grace2html $(OBJ) $(LIBS)

clean:
	rm -f *.o
	rm -f grace2html

makeinclude:
	@echo please run ./configure
	@false

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

include makeinclude

OBJ	= main.o

all: htparse

htparse: $(OBJ)
	$(LD) $(LDFLAGS) -o htparse $(OBJ) $(LIBS)

clean:
	rm -f *.o
	rm -f htparse

makeinclude:
	@echo please run ./configure
	@false

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

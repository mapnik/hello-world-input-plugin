# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)

CXXFLAGS := $(shell mapnik-config --cflags)

LIBS = $(shell mapnik-config --libs --ldflags --dep-libs)

SRC = $(wildcard *.cpp)

OBJ = $(SRC:.cpp=.o)

MAPNIK_PLUGIN = hello.input

all : $(SRC) $(MAPNIK_PLUGIN)

$(MAPNIK_PLUGIN) : $(OBJ)
	$(CXX) -shared $(OBJ) $(LIBS) -o $@

.cpp.o :
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean test

clean:
	rm -f $(OBJ)
	rm -f $(MAPNIK_PLUGIN)
	rm -f ./test/run

install: all
	cp $(MAPNIK_PLUGIN) $(shell mapnik-config --input-plugins)

test/run: $(MAPNIK_PLUGIN) test/unit.cpp
	$(CXX) -o ./test/run test/unit.cpp $(CXXFLAGS) $(LIBS)

test: test/run
	./test/run
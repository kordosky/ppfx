PPFX_SOURCES = $(wildcard src/*.cpp)
PPFX_OBJS    = $(PPFX_SOURCES:.cpp=.o)

PROGS = $(shell ls src/*.C | sed 's/\.C//' | sed 's/src\///')
INCLUDES = -I./include -I$(BOOSTROOT) -I$(shell root-config --incdir)
DEPLIBS=$(shell root-config --libs) -lEG

DK2NU_INCLUDES= include/dk2nu.h include/dkmeta.h include/LinkDef.h
DK2NU_SRCS= src/dk2nu.cc src/dkmeta.cc
DK2NU_OBJS = $(DK2NU_SRCS:.cc=.o)

CC	=	g++
COPTS	=	-fPIC -DLINUX -O0  -g $(shell root-config --cflags)
FLAGS   =       -g

all:    lib programs doxy

lib: libppfx.so libDKLib.so

libppfx.so: $(PPFX_OBJS)
	if [ ! -d lib ]; then mkdir -p lib; fi
	$(CC) -shared -o lib/$@ $^

programs: $(PROGS)
	echo making $(PROGS)

$(PROGS): % : src/%.o $(PPFX_OBJS) libDKLib.so
	if [ ! -d bin ]; then mkdir -p bin; fi
	$(CC) -Wall -o bin/$@ $< $(PPFX_OBJS) $(DEPLIBS) lib/libDKLib.so

%.o: %.cpp
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.cxx
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.cc
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.C
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

DKDict.cxx: $(DK2NU_INCLUDES)
	rootcint -f $@ -c $(FLAGS) -p ${INCLUDES} $^

libDKLib.so: DKDict.o $(DK2NU_OBJS)
	g++ -shared -o$@ `root-config --ldflags` $(FLAGS) ${INCLUDES} $^
	mv $@ lib
	mv DKDict* lib

doxy: 
	doxygen doxygen/config_doxygen

clean:  deldoxy delobj dellib delbin

delobj:
	-rm src/*.o

dellib:
	if [ -d lib ]; then rm -rf lib; fi 		

delbin:
	if [ -d bin ]; then rm -rf bin; fi

deldoxy:
	if [ -d html ]; then rm -rf html; fi
	if [ -d latex ]; then rm -rf latex; fi




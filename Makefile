
OBJS_LIB = $(shell ls src/*.cpp | sed 's/\.cpp/.o/')
PROGS = $(shell ls src/*.C | sed 's/\.C//' | sed 's/src\///')
INCLUDES = -I./include -I$(shell root-config --incdir) -I$(BOOST)
DEPLIBS=$(shell root-config --libs) -lEG
DK2NU_INCLUDES= include/dk2nu.h include/dkmeta.h include/LinkDef.h
DK2NU_SRCS= src/dk2nu.cc src/dkmeta.cc
CC	=	g++
COPTS	=	-fPIC -DLINUX -O0  -g -m32
FLAGS   =       -g

all:    lib programs doxy

lib: libppfx.so libDKLib.so

libppfx.so: $(OBJS_LIB)
	if [ ! -d lib ]; then mkdir -p lib; fi
	$(CC) -shared -m32 -o lib/libppfx.so $(OBJS_LIB)

programs: $(PROGS)
	echo making $(PROGS)

$(PROGS): % : src/%.o $(OBJS_LIB) libDKLib.so
	if [ ! -d bin ]; then mkdir -p bin; fi
	$(CC) -m32 -Wall -o bin/$@ $< $(OBJS_LIB) $(DEPLIBS) lib/libDKLib.so

%.o: %.cpp
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.C
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

DKDict.cxx: $(DK2NU_INCLUDES)
	rootcint -f $@ -c $(FLAGS) -p ${INCLUDES} $^

libDKLib.so: DKDict.cxx $(DK2NU_SRCS)
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




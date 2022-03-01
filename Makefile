PPFX_MODE = ${MODE}
MODE_NUMI = NUMI
OBJS_LIB = $(shell ls src/*.cpp | sed 's/\.cpp/.o/')
PROGS = $(shell ls src/*.C | sed 's/\.C//' | sed 's/src\///')
# Split for NOVA and DUNE. Probably can be done more elagently --Pierce Weatherly
#$(info $(PPFX_MODE) )
ifeq ($(PPFX_MODE), $(MODE_NUMI)) # NUMI is NOVA & defalt other
$(info $$PPFX_MODE is [${PPFX_MODE}])
INCLUDES = -I./include -I$(shell root-config --incdir) -I$(BOOSTROOT) -I${DK2NU}/include
else # DUNE is OPT or REF
$(info $$PPFX_MODE is [${PPFX_MODE}] DUNE)
INCLUDES = -I./include -I$(shell root-config --incdir) -I$(BOOST_INC) -I${DK2NU}/include
endif

DEPLIBS=$(shell root-config --libs) -lEG

CC	=	g++
COPTS	=	-fPIC -DLINUX -O0  -g $(shell root-config --cflags)
FLAGS   =       -g

all:    lib programs doxy

lib: libppfx.so

libppfx.so: $(OBJS_LIB)
	if [ ! -d lib ]; then mkdir -p lib; fi

ifeq ($(PPFX_MODE), $(MODE_NUMI))
	$(info $$PPFX_MODE is [${PPFX_MODE}])
	$(CC) -shared -o lib/$@ $^ -L${DK2NU_LIB} -ldk2nuTree
else
	$(info $$PPFX_MODE is [${PPFX_MODE}] DUNE)
	$(CC) -shared -o lib/$@ $^ -L${DK2NU}/lib -ldk2nuTree
endif


programs: $(PROGS)
	echo making $(PROGS)

$(PROGS): % : src/%.o $(OBJS_LIB)  libppfx.so
	if [ ! -d bin ]; then mkdir -p bin; fi

ifeq ($(PPFX_MODE), $(MODE_NUMI))
	$(info $$PPFX_MODE is [${PPFX_MODE}])
	$(CC) -Wall -o bin/$@ $< $(PPFX_OBJS) $(DEPLIBS) -L$(PPFX_DIR)/lib -lppfx -L${DK2NU_LIB} -ldk2nuTree
else
	$(info $$PPFX_MODE is [${PPFX_MODE}] DUNE)
	$(CC) -Wall -o bin/$@ $< $(PPFX_OBJS) $(DEPLIBS) -L$(PPFX_DIR)/lib -lppfx -L${DK2NU}/lib -ldk2nuTree
endif

%.o: %.cpp
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.cc
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.C
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.cxx
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

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

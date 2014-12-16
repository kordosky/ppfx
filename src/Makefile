
#for commands:
CXX       = g++
CXXFLAGS  = -g -Wall -fPIC
#COPTS	  = -fPIC -DLINUX -O0 -std=c++0x -g
COPTS	  = -fPIC -DLINUX -O0 -g
ROOTFLAGS = `root-config --cflags --glibs` -lEG
INCLUDE   = -I./../include
#INCBOOST  = -I/grid/fermiapp/minerva/software_releases/lcg_61_root_patch/external/Boost/1.44.0_python2.6/x86_64-slc5-gcc43-opt/include/boost-1_44
#INCBOOST  = -I/grid/fermiapp/products/lariat/boost/v1_55_0/source/boost_1_55_
INCBOOST  = -I/nusoft/app/externals/boost/v1_55_0/source/boost_1_55_0

#files:
TARGETFILES=$(shell ls *.cpp | sed 's/\.cpp//')
OBJECTFILES=$(shell ls *.cpp | sed 's/\.cpp/.o/')

all:  $(TARGETFILES)
$(TARGETFILES): % : %.cpp
	$(CXX) $(COPTS) $(INCLUDE) $(INCBOOST) -o $*.o -c $*.cpp $(ROOTFLAGS)

clean:  
	rm *.exe *~ *.o ../include/*~ 


LIB = $(shell pwd)
INCLUDE = $(shell pwd)/include

ROOT_LIBS = $(shell root-config --glibs)
ROOT_GCC_FLAGS = $(shell root-config --cflags)
ROOT_LIBSEXTRA =  -lTreePlayer -lMathMore -lSpectrum -lMinuit 
# ROOT_LIBSEXTRA =  -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT

CC = g++
CFLAGS = -std=c++11 -g -fPIC $(ROOT_GCC_FLAGS) $(GRSI_GCC_FLAGS) -I$(INCLUDE) 
LIBRS = -L$(INCLUDE) $(EXTERNAL_LIBS) $(ROOT_LIBS) $(ROOT_LIBSEXTRA) -L$(LIB) -L$(LIB)/bin

SYSHEAD = $(wildcard include/*.h)
OBJECTS = $(patsubst include/%.h,bin/%.o,$(SYSHEAD))
HEAD = $(patsubst %.h,$(shell pwd)/%.h,$(SYSHEAD))

bin/SurrogateSort: Sort.cpp $(OBJECTS) bin/DictOutput.cxx
	$(CC) $(CFLAGS) -o $@ $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS)
	chmod +x $@

bin/%.o: src/%.cpp include/%.h 
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBRS)
	
bin/DictOutput.cxx: $(HEAD)
	bash bin/link.sh $(HEAD)
	rootcint -f $@ -c -I$(INCLUDE) $(HEAD) bin/Linkdef.h
	
clean:
	rm -f $(LIB)/bin/*.o
	rm -f $(LIB)/bin/Linkdef.h
	rm -f $(LIB)/bin/DictOutput*
	rm -f $(LIB)/bin/SurrogateSort


LIB = $(shell pwd)
INCLUDE = $(shell pwd)/include

ROOT_LIBS = $(shell root-config --glibs)
ROOT_GCC_FLAGS = $(shell root-config --cflags)
ROOT_LIBSEXTRA =  -lTreePlayer -lMathMore -lSpectrum -lMinuit 
# ROOT_LIBSEXTRA =  -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT

CC = g++
CFLAGS = -std=c++11 -g -fPIC $(ROOT_GCC_FLAGS) $(GRSI_GCC_FLAGS) -I$(INCLUDE) 
LIBRS = -L$(INCLUDE) $(EXTERNAL_LIBS) $(ROOT_LIBS) $(ROOT_LIBSEXTRA) -L$(LIB) -L$(LIB)/bin


NONHEAD = $(wildcard src/*.h)
SYSHEAD = $(wildcard include/*.h)
OBJECTS = $(patsubst include/%.h,bin/%.o,$(SYSHEAD))
HEAD = $(patsubst %.h,$(shell pwd)/%.h,$(SYSHEAD))

SHAREDLIB = bin/SS.so
TARG = bin/SurrogateSort

$(TARG): Sort.cpp $(SHAREDLIB)
	$(CC) $(CFLAGS) -o $@ $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS)
	chmod +x $@

debug:  Sort.cpp $(SHAREDLIB)
	$(CC) -DDEBUG $(CFLAGS) -o  $(TARG) $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS)
	chmod +x $(TARG)
	touch Sort.cpp
	
cal:  Sort.cpp $(SHAREDLIB)
	$(CC) -DCALIBRATE $(CFLAGS) -o  $(TARG) $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS) -ljroot_phys
	chmod +x $(TARG)
	touch Sort.cpp

$(SHAREDLIB): $(OBJECTS) $(NONHEAD) bin/DictOutput.cxx
	$(CC) $(CFLAGS) -o $@ -shared bin/DictOutput.cxx $(OBJECTS) -I. $(ROOT_LIBS) $(ROOT_LIBSEXTRA)
	bash bin/MakeExport.sh
	
QuickCal : bin/SurrogateCal	
	
bin/SurrogateCal: scripts/QuickCal.cpp $(OBJECTS) $(NONHEAD) bin/DictOutput.cxx
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
	rm -f $(LIB)/bin/Surrogate*

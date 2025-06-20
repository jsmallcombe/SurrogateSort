
LIB = $(shell pwd)
INCLUDE = $(shell pwd)/include


ROOT_LIBS = `root-config --glibs`
ROOT_GCC_FLAGS = `root-config --cflags`
# ROOT_LIBS = $(shell root-config --glibs)
# ROOT_GCC_FLAGS = $(shell root-config --cflags)
ROOT_LIBSEXTRA =  -lTreePlayer -lMathMore -lSpectrum -lMinuit 
# ROOT_LIBSEXTRA =  -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT

CC = g++
CFLAGS = -std=c++11 -g -fPIC $(ROOT_GCC_FLAGS) -I$(INCLUDE) 
LIBRS = -L$(INCLUDE) $(EXTERNAL_LIBS) $(ROOT_LIBS) $(ROOT_LIBSEXTRA) -L$(LIB) -L$(LIB)/bin


SYSHEAD = $(wildcard include/*.h)
OBJECTS = $(patsubst include/%.h,bin/%.o,$(SYSHEAD))
HEAD = $(patsubst %.h,$(shell pwd)/%.h,$(SYSHEAD))
SORTS = $(patsubst sort_files/%Loop.h,bin/%Sort,$(wildcard sort_files/*Loop.h))

CALEXEC = bin/SurrogateCal
SHAREDLIB = bin/libSurrogateSort.so
TARG = bin/SurrogateSort


$(TARG): Sort.cpp $(SHAREDLIB) $(SORTS) $(CALEXEC) src/RangeData.h
	$(CC) $(CFLAGS) -o $@ $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS)
	chmod +x $@
	
$(CALEXEC) : scripts/SurrogateCalFit.cpp $(SHAREDLIB)
	$(CC) $(CFLAGS) -o $@ $< -I. $(OBJECTS) $(LIBRS)

	
JLIB_FOUND := $(shell echo "int main(){}" | gcc -x c++ -o /dev/null -ljroot_phys - 2>/dev/null && echo yes || echo no)
ifeq ($(JLIB_FOUND),yes)
bin/%Sort: Sort.cpp sort_files/%HistList.h sort_files/%Loop.h $(SHAREDLIB) src/RangeData.h 
	SORTFILE1="$(word 2, $^)"; \
	SORTFILE2="\"$(word 3, $^)\""; \
	$(CC) -DJPHYS -DSORTFILE1=$$SORTFILE1 -DSORTFILE2=$$SORTFILE2 $(CFLAGS) -o $@ $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS) -ljroot_phys
else
bin/%Sort: Sort.cpp sort_files/%HistList.h sort_files/%Loop.h $(SHAREDLIB)  RangeData.h
	SORTFILE1="$(word 2, $^)"; \
	SORTFILE2="\"$(word 3, $^)\""; \
	$(CC) -DSORTFILE1=$$SORTFILE1 -DSORTFILE2=$$SORTFILE2 $(CFLAGS) -o $@ $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS)
endif
# echo "SORTFILE1: $$SORTFILE1"; \

$(SHAREDLIB): $(OBJECTS) bin/DictOutput.cxx
	$(CC) $(CFLAGS) -o $@ -shared bin/DictOutput.cxx $(OBJECTS) -I. $(ROOT_LIBS) $(ROOT_LIBSEXTRA)
	bash bin/MakeExport.sh
	
# QuickCal : bin/SurrogateCal	
# bin/SurrogateCal: scripts/QuickCal.cpp $(OBJECTS) bin/DictOutput.cxx
# 	$(CC) $(CFLAGS) -o $@ $< bin/DictOutput.cxx -I. $(OBJECTS) $(LIBRS)
# 	chmod +x $@
	
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
	rm -f $(LIB)/bin/*Sort
	rm -f $(LIB)/bin/*.so
	rm -f $(LIB)/bin/*.so

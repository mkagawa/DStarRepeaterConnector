CXXFLAGS += -g -std=gnu++11 -DwxUSE_THREADS=1 $(shell wx-config --cxxflags base --debug)
LDLIBS = -g -lutil -pthread $(shell wx-config --libs --debug)


#-std=c++0x 
#wx-config -libs

.PHONY: all clean distclean

all: repeaterconnector

repeaterconnector: DVMegaWorkerThread.o DVAPWorkerThread.o BaseWorkerThread.o RepeaterConnector.o DVAPWorkerThread.h BaseWorkerThread.h RepeaterConnector.h
	$(LINK.cpp) $^ $(LDLIBS) -o $@


clean:
	@- $(RM) *.o repeaterconnector

distclean: clean

CXXFLAGS += -g -I/usr/lib64/wx/include/gtk3-unicode-3.1 -I/usr/include/wx-3.1 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -DwxUSE_GUI=0 -pthread

#-std=c++0x 
#wx-config -libs
LDLIBS = -g -lutil -pthread   -lwx_gtk3u_xrc-3.1 -lwx_gtk3u_html-3.1 -lwx_gtk3u_qa-3.1 -lwx_gtk3u_adv-3.1 -lwx_gtk3u_core-3.1 -lwx_baseu_xml-3.1 -lwx_baseu_net-3.1 -lwx_baseu-3.1

.PHONY: all clean distclean

all: dvmega_sim

dvmega_sim: WorkerThread.o DVMegaSimApp.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@

clean:
	@- $(RM) dvmega_sim.o dvmega_sim

distclean: clean

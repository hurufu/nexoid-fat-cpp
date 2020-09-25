# User config #################################################################
OL                  := 0
DL                  := gdb3
WARNINGS            := all extra no-unused-parameter
GCC_FEATURES        :=
GCC_FEATURES        += permissive
GCC_FEATURES        += lto=auto use-linker-plugin
#GCC_FEATURES        := instrument-functions
#GCC_FEATURES        += analyzer

# Project configuration #######################################################
EXECUTABLE          := nexoid-cpp
SOURCES             := $(wildcard *.cpp)
LIBNEXOID_NAME      := libnexoid.a

#TODO: Remove hardcoded prefix of libnexoid.a
LIBNEXOID_PATH      := /usr/local/lib/$(LIBNEXOID_NAME)
CPPFLAGS            += -I/usr/local/include
CPPFLAGS            += -I/usr/local/include/nexoid
CXXFLAGS              := -g$(DL) -O$(OL)
CXXFLAGS              += $(addprefix -W,$(WARNINGS))
CXXFLAGS              += $(addprefix -f,$(GCC_FEATURES))
CXXFLAGS              += $(if $(USE_COLOR),-fdiagnostics-color=always,)
TMPDIR              ?= /tmp
TRACE_LOG           := $(TMPDIR)/$(EXECUTABLE).txt
NOHUP_OUT           := nohup.out

# Commands ####################################################################
MIMEOPEN            := mimeopen


include $(ASN1_MAKEFILE)

.PHONY: all most_frequent FORCE
most_frequent: run index
all: $(EXECUTABLE) .syntastic_c_config

$(EXECUTABLE): $(SOURCES) $(LIBNEXOID_PATH)
	$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) $(ASMFLAGS) $(LDFLAGS) $^ $(LDLIBS)

.PHONY: run
run: $(EXECUTABLE)
	./$<

.PHONY: clean
clean: F += $(EXECUTABLE) trace.log tags $(TRACE_LOG) $(NOHUP_OUT)
clean:
	$(if $(strip $(sort $(wildcard $F))),$(RM) -- $F,)
	$(if $(strip $(sort $(wildcard $D))),rmdir -p -- $D,)

.PHONY: ddd
ddd: $(EXECUTABLE)
	ddd --args $<

.PHONY: gdb
gdb: $(EXECUTABLE)
	gdb ./$<

.PHONY: index
index: tags
tags: $(SOURCES)
	ctags -R . /usr/local/include

.PHONY: surf
surf: $(NOHUP_OUT)
$(NOHUP_OUT): $(TRACE_LOG)
	nohup $(MIMEOPEN) $< >$@ &

$(TRACE_LOG): $(EXECUTABLE) FORCE
	-./$< >$@

.PHONY: trace
trace: trace.log
	bash -c 'paste -d "" <(cut -d, -f1 <$<) <(awk -f adjust_addr.awk --non-decimal-data <$< | addr2line -psf -e $(EXECUTABLE))'
trace.log: $(EXECUTABLE) FORCE
	-./$< >/dev/null

.syntastic_c_config:
	echo $(CPPFLAGS) $(CFLAGS) -fdiagnostics-color=never | tr ' ' '\n' > $@

.PHONY: cg
cg: cg.png
cg.png:
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -fdump-rtl-expand $(SOURCES) $(LIBNEXOID_PATH) $(LDLIBS)
	egypt *.expand | sed '8irankdir="LR"' | dot -Tpng > callgraph.png

clean: F += $(libasncodec_la_SOURCES)
clean: F += $(libasncodec_la_OBJECTS)
clean: F += $(ASN1_MAKEFILE)

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

# ASN.1 generator config for asn1c from http://lionet.info/asn1c
ASN1_GENERATED_DIR := asn1c-generated
ASN1_MAKEFILE      := $(ASN1_GENERATED_DIR)/Makefile.am.libasncodec

#TODO: Remove hardcoded prefix of libnexoid.a
LIBNEXOID_PATH      := /usr/local/lib/$(LIBNEXOID_NAME)
CPPFLAGS            += -I/usr/local/include
CPPFLAGS            += -I/usr/local/include/nexoid
CPPFLAGS            += -I$(ASN1_GENERATED_DIR)
CPPFLAGS            += -I/usr/share/asn1c
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
clean: F += $(libasncodec_la_SOURCES)
clean: F += $(libasncodec_la_OBJECTS)
clean: F += $(lib_LTLIBRARIES)
clean: F += $(ASN1_MAKEFILE)
clean: D += $(ASN1_GENERATED_DIR)

$(ASN1_MAKEFILE): $(wildcard asn1/*.asn1)
	mkdir -p $(ASN1_GENERATED_DIR)
	asn1c -funnamed-unions -fincludes-quoted -pdu=auto -D $(ASN1_GENERATED_DIR) -no-gen-example -no-gen-OER -no-gen-PER $^

libasncodec_la_OBJECTS := $(libasncodec_la_SOURCES:.c=.o)
$(lib_LTLIBRARIES)($(libasncodec_la_OBJECTS)): $(libasncodec_la_OBJECTS)
	gcc-ar cr $@ $%
$(lib_LTLIBRARIES): $(libasncodec_la_OBJECTS)
	gcc-ar rcs $@ $^
$(ASN1_GENERATED_DIR)/%.o: $(ASN1_GENERATED_DIR)/%.c $(ASN_MODULE_HDRS)
	$(CC) -c $(libasncodec_la_CPPFLAGS) -I/usr/share/asn1c $(libasncodec_la_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(libasncodec_la_LDFLAGS)

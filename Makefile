# User config #################################################################
OL                  := 0
DL                  := gdb3
WARNINGS            := all extra no-unused-parameter
# Warnings taken from https://gist.github.com/jrelo/f5c976fdc602688a0fd40288fde6d886
WARNINGS            += format format-security
WARNINGS            += conversion sign-conversion
WARNINGS            += stack-protector
GCC_FEATURES        :=
#GCC_FEATURES        += lto=auto use-linker-plugin
GCC_FEATURES        += trapv
GCC_FEATURES        += permissive
#GCC_FEATURES        := instrument-functions
#GCC_FEATURES        += analyzer

# Installation configuration ##################################################
PREFIX          ?= /usr/local
DESTDIR         ?= /
BINDIR          := $(DESTDIR)/$(PREFIX)/bin
INSTALLED_FILES := $(BINDIR)/nexoid-cpp

# Project configuration #######################################################
EXECUTABLE          := nexoid-cpp
SOURCES             := $(filter-out scapi_socket_session.cpp,$(wildcard *.cpp))
LIBNEXOID_NAME      := cygnexoid.a
ASN_SOURCES         := asn1/Scapi.asn1 asn1/ScapiSocketClient.asn1 asn1/ScapiNngClient.asn1
OBJECTS             := $(SOURCES:.cpp=.o) nexoconf.o
DEPENDS             := $(OBJECTS:.o=.d)

# ASN.1 generator config for asn1c from http://lionet.info/asn1c
ASN1_GENERATED_DIR := asn1c-generated
ASN1_MAKEFILE      := $(ASN1_GENERATED_DIR)/Makefile.am.libasncodec

LIBNEXOID_PATH      := nexoid-ed/$(LIBNEXOID_NAME)

CPPFLAGS            += -I/usr/local/include
CPPFLAGS            += -I.
CPPFLAGS            += -Inexoid-ed/src
CPPFLAGS            += -Inexoid-ed/include
CPPFLAGS            += -I$(ASN1_GENERATED_DIR)
CPPFLAGS            += -I/usr/share/asn1c

_CXXFLAGS           := -g$(DL) -O$(OL)
_CXXFLAGS           += $(addprefix -W,$(WARNINGS))
_CXXFLAGS           += $(addprefix -f,$(GCC_FEATURES))
_CXXFLAGS           += $(if $(USE_COLOR),-fdiagnostics-color=always,)
CXXFLAGS            ?= $(_CXXFLAGS)
CXXFLAGS            += -std=c++17
CXXFLAGS            += -fpermissive

_CFLAGS             := -g$(DL) -O$(OL)
_CFLAGS             += $(addprefix -W,$(WARNINGS))
CFLAGS              ?= $(_CFLAGS)
#CFLAGS              += -DASN_EMIT_DEBUG=1
#CFLAGS              += -DASN_THREAD_SAFE

TMPDIR              ?= /tmp
TRACE_LOG           := $(TMPDIR)/$(EXECUTABLE).txt
NOHUP_OUT           := nohup.out
LDLIBS              := -lnng

# Unit tests settings #########################################################
UT_EXECUTABLE  := ut/ut
UT_SOURCES     := $(wildcard ut/*.cpp)
UT_OBJECTS     := $(UT_SOURCES:.cpp=.o)

VALGRIND_FLAGS := --leak-check=full --track-origins=yes --show-error-list=yes

# Commands ####################################################################
MIMEOPEN            := mimeopen

# Targets that do not need *.d dependencies for source files
NOT_DEP      := clean asm pp wipe update

.PHONY: all most_frequent FORCE
most_frequent: run index
all: $(EXECUTABLE) .syntastic_cpp_config

include $(ASN1_MAKEFILE)
include $(if $(filter $(NOT_DEP),$(MAKECMDGOALS)),,$(DEPENDS))

VPATH += . nexoid-ed/src

$(EXECUTABLE): $(OBJECTS) $(LIBNEXOID_PATH) $(lib_LTLIBRARIES)
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(ASMFLAGS) $(word 1,$^)
%.d: %.cpp $(LIBNEXOID_PATH)
	$(CXX) -MM -MG -MF $@ -MT $*.o $(CPPFLAGS) $(CXXFLAGS) $(ASMFLAGS) -o $@ $<
%.d: %.c $(LIBNEXOID_PATH)
	$(CC) -MM -MG -MF $@ -MT $*.o $(CPPFLAGS) $(CFLAGS) $(ASMFLAGS) -o $@ $<

# TODO: Integrate child Makefile better
$(LIBNEXOID_PATH):
	+make -C '$(@D)' static

nexoconf.o: nexoconf.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $(ASMFLAGS) $<

.PHONY: run
run: $(EXECUTABLE)
	./$<

.PHONY: clean
clean: F += $(EXECUTABLE) trace.log tags $(TRACE_LOG) $(NOHUP_OUT)
clean: F += $(OBJECTS)
clean: F += $(DEPENDS)
clean:
	+make -C nexoid-ed clean
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

.syntastic_cpp_config:
	echo $(CPPFLAGS) $(CFLAGS) -fdiagnostics-color=never | tr ' ' '\n' > $@

.PHONY: cg
cg: cg.png
cg.png:
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -fdump-rtl-expand $(SOURCES) $(LIBNEXOID_PATH) $(LDLIBS)
	egypt *.expand | sed '8irankdir="LR"' | dot -Tpng > callgraph.png

%.valgrind: % %.supp
	valgrind $(VALGRIND_FLAGS) --suppressions=$*.supp ./$* 2>$@

.PHONY: valgrind
valgrind: $(EXECUTABLE).valgrind

.PHONY: strace
strace: $(EXECUTABLE).strace
%.strace: %
	strace $(STRACE_FLAGS) ./$* 3>&2 2>&1 1>&3 | ts -i %.S >$@

%.s: %.cpp
	$(CXX) -S $(CPPFLAGS) $(CXXFLAGS) $(ASMFLAGS) -o $@ $<

%.s: %.c
	$(CC) -S $(CPPFLAGS) $(CFLAGS) $(ASMFLAGS) -o $@ $<

clean: F += $(libasncodec_la_SOURCES)
clean: F += $(libasncodec_la_OBJECTS)
clean: F += $(lib_LTLIBRARIES)
clean: F += $(ASN1_MAKEFILE)
clean: F += nexoconf.o
clean: D += $(ASN1_GENERATED_DIR)
clean: F += $(EXECUTABLE).strace
clean: F += $(EXECUTABLE).valgrind
clean: F += $(UT_OBJECTS) $(UT_EXECUTABLE)

$(ASN1_MAKEFILE): $(ASN_SOURCES)
	mkdir -p $(ASN1_GENERATED_DIR)
	asn1c -fincludes-quoted -pdu=auto -D $(ASN1_GENERATED_DIR) -no-gen-example -no-gen-OER -no-gen-PER $^

libasncodec_la_OBJECTS := $(libasncodec_la_SOURCES:.c=.o)
$(lib_LTLIBRARIES)($(libasncodec_la_OBJECTS)): $(libasncodec_la_OBJECTS)
	gcc-ar cr $@ $%
$(lib_LTLIBRARIES): $(libasncodec_la_OBJECTS)
	gcc-ar rcs $@ $^
$(ASN1_GENERATED_DIR)/%.o: $(ASN1_GENERATED_DIR)/%.c $(ASN_MODULE_HDRS)
	$(CC) -c $(libasncodec_la_CPPFLAGS) -I/usr/share/asn1c $(libasncodec_la_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(libasncodec_la_LDFLAGS)

.PHONY: install uninstall
install: $(INSTALLED_FILES)
uninstall: F := $(sort $(wildcard $(INSTALLED_FILES)))
uninstall:
	$(if $(strip $F),$(RM) -- $F,)

$(BINDIR)/nexoid-cpp: nexoid-cpp
	install -D -m755 -t $(@D) $<

.PHONY: test
test: $(UT_EXECUTABLE)
	./$<
$(UT_EXECUTABLE): $(UT_OBJECTS)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

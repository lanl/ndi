######################################################################
#
# Second-level make file for NDI
# 
# This make file does the real work, but probably you should be
# calling this from the make file in the directory above.
#
######################################################################

.PHONY:		all check clean distclean install

SHELL		:= /bin/sh
INSTALL		:= $(shell pwd)/install-sh -c
VERSION 	:= version.h
FMANGLE 	:= fmangle.h
VAR_TYPE	:= ndi_var_type.h
GROUP		:= pdata

######################################################################

# configuration selection is based on the variable CONFIG

include ../conf/$(CONFIG).mk

######################################################################

# GNU conforming installation directory variable names.
# Over-ride with command line arguments, e.g.
#	make prefix=$HOME/usr install

prefix = $(HOME)/usr/$(CONFIG)
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include
infodir = $(prefix)/info

######################################################################

TARGET := libndi.a

SRC := \
	ndi_avail.c \
	ndi_bin_tree.c \
	ndi_check_option.c \
	ndi_close.c \
	ndi_close_lib_file.c \
	ndi_c_values.c \
	ndid_get_size.c \
	ndid_get_string_vec.c \
	ndi_element_index.c \
	ndi_fill_element.c \
	ndi_find_index.c \
	ndi_free.c \
	ndi_get_int_val.c \
	ndi_get_int_val_n.c \
	ndi_get_int_vec.c \
	ndi_get_int_vec_n.c \
	ndi_get_size.c \
	ndi_get_size_n.c \
	ndi_get_size_x.c \
	ndi_get_string_val.c \
	ndi_get_string_val_n.c \
	ndi_init.c \
	ndi_init_iso_state.c \
	ndi_init_state.c \
	ndi_interp_integ.c \
	ndi_malloc.c \
	ndi_misc.c \
	ndi_misc_depl.c \
	ndi_open_gendir.c \
	ndi_open_lib_file.c \
	ndi_read_dir_block.c \
	ndi_read_rx_prod.c \
	ndi_read_string_block.c \
	ndi_set0.c \
	ndi_set_e_grps.c \
	ndi_set_isotope.c \
	ndi_set_option.c \
	ndi_skip_rx_prod.c \
	ndi_state.c \
	ndi_std_zaid.c \
	ndi_zaid_dir.c

SRC_FLOAT := \
	ndi_collapse_float.c \
	ndi_collapse_mat.c \
	ndi_get_float_val.c \
	ndi_get_float_val_n.c \
	ndi_get_float_vec.c \
	ndi_get_float_vec_n.c \
	ndi_get_float_vec_x.c \
	ndi_misc_float.c \
	ndi_read_float_block.c \
	ndi_skip_float_block.c \
	ndid_zaid_float_vec.c

SRC_TESTC := \
        test/kerma_example.c \
        test/5.c \
        test/c.c \
        test/collapse.c \
        test/depl.c \
        test/mgn_c.c \
        test/ndid.c \
        test/tn3.c \
        test/tn4.c \
        test/tn5.c \
        test/tn_c.c \
        test/tn_chain.c \
        test/tn_new.c \
        test/tnreac_c.c \
        test/tn_simple.c

SRC_TESTF := \
	test/ndi_testf2.F \
	test/ndi_testf.F

ifneq (,$(findstring cray, $(CONFIG)))
SRC += ndi_fortran_cray.c
else
SRC += ndi_fortran.c
endif

# determine the object files

OBJ 	:= 
OBJ	+= $(SRC:%.c=%.o)
OBJ	+= $(SRC_FLOAT:%.c=%_float32.o)
OBJ	+= $(SRC_FLOAT:%.c=%_float64.o)

OBJ_TESTC:=  test/all.o $(SRC_TESTC:%.c=%.o)
TESTC	:=  testc_all $(SRC_TESTC:test/%.c=testc_%)
TESTF	:=  $(SRC_TESTF:test/%.F=testf_%)

# determine dependency files

DEPENDENCIES 	:= 
DEPENDENCIES	+= $(SRC:%.c=%.d)
DEPENDENCIES	+= $(SRC_FLOAT:%.c=%_float32.d)
DEPENDENCIES	+= $(SRC_FLOAT:%.c=%_float64.d)

# target rules

all: $(TARGET)

check: $(TARGET) $(TESTC) $(TESTF)
	echo "Insert commands to run the tests here"

clean:
	$(RM) $(OBJ) $(DEPENDENCIES) $(OBJ_TESTC) $(TESTC) $(TESTF)

distclean: clean
	$(RM) $(TARGET) $(VERSION) $(FMANGLE) $(VAR_TYPE)

install: all
	umask 002 ; \
	$(INSTALL) -g $(GROUP) -m 775 -d $(includedir) ; \
	$(INSTALL) -g $(GROUP) -m 775 -d $(libdir) ; \
	$(INSTALL) -g $(GROUP) -m 664 libndi.a $(libdir) ; \
	$(INSTALL) -g $(GROUP) -m 664 ndi.h $(includedir) ; \
	$(INSTALL) -g $(GROUP) -m 664 ndi_keywords.h $(includedir) ; \
	$(INSTALL) -g $(GROUP) -m 664 ndi_var_type.h $(includedir)

libndi.a: $(VERSION) $(FMANGLE) $(VAR_TYPE) $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

$(VERSION): VERSION
	echo '#define NDI_VERSION "'`cat VERSION`'"' > $@

$(VAR_TYPE): ndi_define_float.c
	@echo Making $(VAR_TYPE)
	$(RM) ndi_define_float
	$(CC) $(CFLAGS) $(CPPFLAGS) -o ndi_define_float ndi_define_float.c
	ndi_define_float > $@
	$(RM) ndi_define_float

$(FMANGLE): fcallscc.c fcallscf.f
	@echo Making $(FMANGLE)
	$(RM) f_calls_c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) fcallscc.c
	$(FC) -c $(FFLAGS) $(CPPFLAGS) fcallscf.f
	$(FC) $(FFLAGS) $(CPPFLAGS) -o f_calls_c fcallscf.o fcallscc.o
	f_calls_c > $@
	$(RM) f_calls_c fcallscc.o fcallscf.o

testc_all: $(TARGET) $(OBJ_TESTC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJ_TESTC) -o $@ $(LDLIBS)

# pattern rules

testc_%: $(TARGET)

testc_%: test/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(STANDALONE_FLAG) -o $@ $< $(LDLIBS)

testf_%: $(TARGET)

testf_%: test/%.F
	$(FC) $(FFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

test/%.o: test/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(NO_NAME_SUB_FLAG) -c -o $@ $<

%.o: %.f
	$(FC) $(FFLAGS) -c -o $@ $<

%.o: %.F
	$(FC) $(FFLAGS) $(CPPFLAGS) -c -o $@ $<

%_float32.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(FLOAT32FLAGS) -c -o $@ $<

%_float64.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(FLOAT64FLAGS) -c -o $@ $<


# include dependency files, if we were not invoked as "make clean"
# or "make distclean"

ifneq ($(findstring clean, $(MAKECMDGOALS)),clean)
-include $(DEPENDENCIES)
endif

# rules for calculating dependency files
 
%.d: %.c $(VERSION) $(FMANGLE) $(VAR_TYPE)
	$(CC) -M $(CPPFLAGS) $< | sed  -e 's@^.*\.o:@$*.o $*.d:@' > $@

%_float32.d: %.c $(VERSION) $(FMANGLE) $(VAR_TYPE)
	$(CC) -M $(CPPFLAGS) $(FLOAT32FLAGS) $< | sed  -e 's@^.*\.o:@$*_float32.o $*_float32.d:@' > $@

%_float64.d: %.c $(VERSION) $(FMANGLE) $(VAR_TYPE)
	$(CC) -M $(CPPFLAGS) $(FLOAT64FLAGS) $< | sed  -e 's@^.*\.o:@$*_float64.o $*_float64.d:@' > $@

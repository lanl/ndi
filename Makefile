######################################################################
#
# Top-level makefile for NDI
#
# This make file sets up build directories using $(LNDIR), then does the
# real make in the build directory.
#
######################################################################

.PHONY: all build debug distclean \
	check check-build check-debug \
	clean clean-build clean-debug \
	install install-build install-debug \
	build-dir debug-dir

######################################################################

# configuration selection is based on the variable CONFIG

CONFIG		:= unknown
BUILD_DIR	:= build-$(CONFIG)
DEBUG_DIR	:= debug-$(CONFIG)
SHELL		:= /bin/sh
LNDIR		:= /usr/bin/X11/lndir

######################################################################

all: build debug

build: build-dir
	(cd $(BUILD_DIR); $(MAKE) CONFIG=$(CONFIG) all)

debug: debug-dir
	(cd $(DEBUG_DIR); $(MAKE) CONFIG=$(CONFIG) DEBUG=1 all)

check: check-build check-debug

check-build: build
	(cd $(BUILD_DIR); $(MAKE) CONFIG=$(CONFIG) check)

check-debug: debug
	(cd $(DEBUG_DIR); $(MAKE) CONFIG=$(CONFIG) DEBUG=1 check)

install: install-build

install-build: build
	(cd $(BUILD_DIR); $(MAKE) install)

install-debug: debug
	(cd $(DEBUG_DIR); $(MAKE) install)

clean: clean-build clean-debug

clean-build:
	-[ -d $(BUILD_DIR) ] && ($(LNDIR) ../src $(BUILD_DIR) ; cd $(BUILD_DIR) ; $(MAKE) clean)

clean-debug:
	-[ -d $(DEBUG_DIR) ] && ($(LNDIR) ../src $(DEBUG_DIR) ; cd $(DEBUG_DIR) ; $(MAKE) clean)

distclean:
	$(RM) -r build-* debug-*

build-dir:
	([ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)) ; $(LNDIR) ../src $(BUILD_DIR)

debug-dir:
	([ -d $(DEBUG_DIR) ] || mkdir $(DEBUG_DIR)) ; $(LNDIR) ../src $(DEBUG_DIR)

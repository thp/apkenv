TARGET := apkenv

# Default target, make sure this is always first
all: $(TARGET) modules platform_targets

PKG_CONFIG ?= $(CROSS_COMPILE)pkg-config
ifeq ($(shell which $(PKG_CONFIG) 2>/dev/null),)
  $(error $(PKG_CONFIG) not found)
endif

ifeq ($(origin SDL_CFLAGS) $(origin SDL_LDLIBS), undefined undefined)
  SDL_CONFIG = $(CROSS_COMPILE)sdl2-config
  ifeq ($(shell which $(SDL_CONFIG) 2>/dev/null),)
    SDL_CONFIG = $(CROSS_COMPILE)sdl-config
    ifeq ($(shell which $(SDL_CONFIG) 2>/dev/null),)
      $(error No SDL development libraries found!)
    else
      $(warning Using SDL 1.2 libraries)
    endif
  endif
  SDL_CFLAGS += $(shell $(SDL_CONFIG) --cflags)
  SDL_LDLIBS += $(shell $(SDL_CONFIG) --libs)
endif
CFLAGS += $(SDL_CFLAGS)
LDFLAGS += $(SDL_LDLIBS)

# Compatibility fix for newer Debian/Ubuntu versions with
# 64-bit time_t on armhf. We could wrap all functions and
# translate all structs as part of the wrapping process,
# but this is a little simpler for now.
CFLAGS += -D_TIME_BITS=32

SHELL := bash

HOSTCC ?= cc
PLATFORM_TARGETS :=

# Build configuration
include config.mk

SOURCES := $(TARGET).c
SOURCES += $(wildcard linker/*.c)
SOURCES += $(wildcard compat/*.c)
SOURCES += $(wildcard apklib/*.c)
SOURCES += $(wildcard jni/*.c)
SOURCES += $(wildcard imagelib/*.c)
SOURCES += $(wildcard debug/*.c)
SOURCES += $(wildcard debug/wrappers/*.c)
SOURCES += $(wildcard accelerometer/*.c)
SOURCES += $(wildcard audio/*.c)
SOURCES += $(wildcard mixer/*.c)

GENERATED_FILES += compat/gen/gles_serialize.c
SOURCES += $(GENERATED_FILES)

# Platform-specific targets and configuration
PLATFORM_INSTALL_TARGETS :=
include platform/$(PLATFORM).mk

SOURCES += $(wildcard platform/common/*.c)

# Support modules for specific applications
MODULES_SOURCES := $(wildcard modules/*.c)
MODULES :=

ifeq ($(STATIC_MODULES),1)
CFLAGS += -DAPKENV_STATIC_MODULES
SOURCES += $(MODULES_SOURCES)
else
MODULES += $(patsubst modules/%.c,%.apkenv.so,$(MODULES_SOURCES))
endif

OBJS := $(patsubst %.c,%.o,$(SOURCES))

DEPS := $(patsubst %.c,%.d,$(SOURCES)) $(patsubst %.c,%.d,$(MODULES_SOURCES))

modules: $(MODULES)

platform_targets: $(PLATFORM_TARGETS)

$(TARGET): $(OBJS)
	$(SILENTMSG) -e "\tLINK\t$@"
	$(SILENTCMD)$(CC) $(OBJS) $(LDFLAGS) -o $@

compat/gen/gles_serialize.c: compat/wrapgen2.py compat/gles_vtable.h
	$(SILENTMSG) -e "\tGEN\t$@"
	$(SILENTCMD)mkdir -p $(dir $@)
	$(SILENTCMD)python3 $+ $@

debug/wrappers/%_thumb.o: debug/wrappers/%_thumb.c
	$(SILENTMSG) -e "\tCC (TH)\t$@"
ifeq ($(PLATFORM),osmesa)
	$(SILENTCMD)$(CC) -O0 -c -o $@ $<
else
	$(SILENTCMD)$(CC) -mthumb -march=armv7-a -O0 -c -o $@ $<
endif

debug/wrappers/%_arm.o: debug/wrappers/%_arm.c
	$(SILENTMSG) -e "\tCC\t$@"
	$(SILENTCMD)$(CC) -marm -O0 -c -o $@ $<

%.o: %.c %.d
	$(SILENTMSG) -e "\tCC\t$@"
	$(SILENTCMD)$(CC) $(CFLAGS) -c -o $@ $<

modules/%.d: modules/%.c
	$(SILENTMSG) -e "\tDEP\t$@"
	$(SILENTCMD)$(CC) $(CFLAGS) -MF $@.tmp -MM $<
	$(SILENTCMD)sed -e 's|.*:|$*.o:|' < $@.tmp | sed -e 's|\.o|\.apkenv\.so|' > $@
	$(SILENTCMD)rm -rf modules/$*.d.tmp

%.d: %.c
	$(SILENTMSG) -e "\tDEP\t$@"
	$(SILENTCMD)$(CC) $(CFLAGS) -MF $@.tmp -MM $<
	$(SILENTCMD)sed -e 's|.*:|$*.o:|' < $@.tmp > $@
	$(SILENTCMD)rm -rf $*.d.tmp

%.apkenv.so: modules/%.c
	$(SILENTMSG) -e "\tMOD\t$@"
	$(SILENTCMD)$(CC) -fPIC -shared $(CFLAGS) -o $@ $<

install: $(TARGET) $(MODULES) $(PLATFORM_INSTALL_TARGETS)
	$(SILENTMSG) -e "\tINSTALL\t$(TARGET)"
	$(SILENTCMD)mkdir -p $(DESTDIR)$(PREFIX)/bin
	$(SILENTCMD)install -m755 $(TARGET) $(DESTDIR)$(PREFIX)/bin
	$(SILENTMSG) -e "\tINSTALL\tMODULES"
	$(SILENTCMD)mkdir -p $(DESTDIR)$(PREFIX)/lib/$(TARGET)/modules
	$(SILENTCMD)install -m644 $(MODULES) $(DESTDIR)$(PREFIX)/lib/$(TARGET)/modules

clean:
	$(SILENTMSG) -e "\tCLEAN"
	$(SILENTCMD)rm -rf $(TARGET) $(OBJS) $(MODULES) $(DEPS) hostui $(GENERATED_FILES)

distclean: clean

ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPS)
endif

.DEFAULT: all
.PHONY: all modules install clean distclean platform_targets

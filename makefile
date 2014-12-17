TARGET := apkenv

# Default target, make sure this is always first
all: $(TARGET) modules

PKG_CONFIG ?= $(CROSS_COMPILE)pkg-config
ifeq ($(shell which $(PKG_CONFIG) 2>/dev/null),)
  $(error $(PKG_CONFIG) not found)
endif

LIBPNG_MOD_VERSION := $(shell $(PKG_CONFIG) --modversion libpng | grep -o "[0-9]\.[0-9]" 2>/dev/null)
ifeq ($(shell $(PKG_CONFIG) --modversion libpng 2>/dev/null),)
  $(error No libpng development libraries found!)
else
  LIBPNG_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags libpng)
  LIBPNG_LDLIBS ?=  $(shell $(PKG_CONFIG) --libs libpng)
endif
ifeq ($(LIBPNG_MOD_VERSION),1.2)
  LIBPNG_CFLAGS += -DLIBPNG_LEGACY
endif
CFLAGS += $(LIBPNG_CFLAGS)
LDFLAGS += $(LIBPNG_LDLIBS)

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

# Platform-specific targets and configuration
PLATFORM_INSTALL_TARGETS :=
include platform/$(PLATFORM).mk

OBJS := $(patsubst %.c,%.o,$(SOURCES))

# Support modules for specific applications
MODULES_SOURCES := $(wildcard modules/*.c)
MODULES := $(patsubst modules/%.c,%.apkenv.so,$(MODULES_SOURCES))

DEPS := $(patsubst %.c,%.d,$(SOURCES)) $(patsubst %.c,%.d,$(MODULES_SOURCES))

modules: $(MODULES)

$(TARGET): $(OBJS)
	$(SILENTMSG) -e "\tLINK\t$@"
	$(SILENTCMD)$(CC) $(LDFLAGS) $(OBJS) -o $@

debug/wrappers/%_thumb.o: debug/wrappers/%_thumb.c
	$(SILENTMSG) -e "\tCC (TH)\t$@"
	$(SILENTCMD)$(CC) -mthumb -O0 -c -o $@ $<

debug/wrappers/%_arm.o: debug/wrappers/%_arm.c
	$(SILENTMSG) -e "\tCC\t$@"
	$(SILENTCMD)$(CC) -marm -O0 -c -o $@ $<

%.o: %.c %.d
	$(SILENTMSG) -e "\tCC\t$@"
	$(SILENTCMD)$(CC) $(CFLAGS) -c -o $@ $<

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
	$(SILENTCMD)rm -rf $(TARGET) $(OBJS) $(MODULES) $(DEPS)

ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPS)
endif

.DEFAULT: all
.PHONY: all modules install clean

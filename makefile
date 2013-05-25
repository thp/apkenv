
# Android Gingerbread linker
LINKER_SOURCES=$(wildcard linker/*.c)

# Compatibility wrappers and hooks
COMPAT_SOURCES=$(wildcard compat/*.c)

# Library for getting files out of .apk files
APKLIB_SOURCES=$(wildcard apklib/*.c)

# Our own JNI environment implementation
JNIENV_SOURCES=$(wildcard jni/*.c)

# Support modules for specific applications
MODULES_SOURCES=$(wildcard modules/*.c)

# Maemo/MeeGo-specific stuff
MAEMO_SOURCES=$(wildcard n9xx/*.c)

# Pandora specific stuff
PANDORA_SOURCES=$(wildcard pandora/*.c)

# JPEG and PNG loaders
IMAGELIB_SOURCES=$(wildcard imagelib/*.c)

# segfault catch
DEBUG_SOURCES=$(wildcard debug/*.c)

OBJDUMP = objdump

TARGET = apkenv

DESTDIR ?= /
PREFIX ?= /opt/$(TARGET)/
BIONIC_LIBS = $(wildcard libs/*.so)

SOURCES += $(TARGET).c
SOURCES += $(LINKER_SOURCES)
SOURCES += $(COMPAT_SOURCES)
SOURCES += $(APKLIB_SOURCES)
SOURCES += $(JNIENV_SOURCES)
SOURCES += $(IMAGELIB_SOURCES)
SOURCES += $(DEBUG_SOURCES)

PANDORA ?= 0
ifeq ($(PANDORA),1)
SOURCES += $(PANDORA_SOURCES)
else
SOURCES += $(MAEMO_SOURCES)
endif

OBJS = $(patsubst %.c,%.o,$(SOURCES))
MODULES = $(patsubst modules/%.c,%.apkenv.so,$(MODULES_SOURCES))

LDFLAGS = -fPIC -ldl -lz -lSDL -lSDL_mixer -pthread -lpng -ljpeg

ifeq ($(PANDORA),1)
CFLAGS += -DPANDORA
LDFLAGS += -lrt
endif

# Selection of OpenGL ES version support (if any) to include
GLES ?= 1
ifeq ($(GLES),2)
    CFLAGS += -DAPKENV_GLES2
    LDFLAGS += -lGLESv2
else
    CFLAGS += -DAPKENV_GLES
    LDFLAGS += -lGLES_CM
endif

FREMANTLE ?= 0
ifeq ($(FREMANTLE),1)
    CFLAGS += -DFREMANTLE
    LDFLAGS += -lSDL_gles
endif

INJECTION_WRAPPER_SOURCE_ARM=linker/wrapper_ARM.c
INJECTION_WRAPPER_SOURCE_THUMB=linker/wrapper_THUMB.c
INJECTION_WRAPPER_SOURCE_HOOK=compat/wrapper.c
INJECTION_WRAPPER_SOURCES=$(INJECTION_WRAPPER_SOURCE_HOOK) $(INJECTION_WRAPPER_SOURCE_ARM) $(INJECTION_WRAPPER_SOURCE_THUMB)

# specify if methods should be traced
DEBUG_TRACE_METHODS ?= 0
# specify which methods should be traced
DEBUG_TRACE_DYNHOOKS ?= 1
DEBUG_TRACE_LATEHOOKS ?= 1
DEBUG_TRACE_UNHOOKED ?= 1
# the following are experimental and use code injection
# in order to be able to trace internal function calls
# this one works, but not in every case so
# adjust is_blacklisted in compat/hooks.c if you have
# problems with some methods
DEBUG_TRACE_INJ_ARM ?= 0
# unfortunately this does not work yet
DEBUG_TRACE_INJ_THUMB ?= 0

INJECTION_WRAPPER_OBJ_ARM = $(patsubst %.c,%.o,$(INJECTION_WRAPPER_SOURCE_ARM))
INJECTION_WRAPPER_OBJ_THUMB = $(patsubst %.c,%.o,$(INJECTION_WRAPPER_SOURCE_THUMB))
INJECTION_WRAPPER_OBJ_HOOK = $(patsubst %.c,%.o,$(INJECTION_WRAPPER_SOURCE_HOOK))

INJECTION_WRAPPER_TARGET_ARM = $(patsubst %.c,%.instructions,$(INJECTION_WRAPPER_SOURCE_ARM))
INJECTION_WRAPPER_TARGET_THUMB = $(patsubst %.c,%.instructions,$(INJECTION_WRAPPER_SOURCE_THUMB))
INJECTION_WRAPPER_TARGET_HOOK = $(patsubst %.c,%.instructions,$(INJECTION_WRAPPER_SOURCE_HOOK))

ifeq ($(DEBUG_TRACE_METHODS),1)
    h=0
    ifeq ($(DEBUG_TRACE_DYNHOOKS),1)
        CFLAGS += -DDEBUG_TRACE_DYNHOOKS
        I_CFLAGS += -DDEBUG_TRACE_METHODS
        h=1
    endif
    ifeq ($(DEBUG_TRACE_LATEHOOKS),1)
        CFLAGS += -DDEBUG_TRACE_LATEHOOKS
        I_CFLAGS += -DDEBUG_TRACE_METHODS
        h=1
    endif
    ifeq ($(DEBUG_TRACE_UNHOOKED),1)
        CFLAGS += -DDEBUG_TRACE_UNHOOKED
        I_CFLAGS += -DDEBUG_TRACE_METHODS
        h=1
    endif
    ifeq ($h,0)
        INJECTION_WRAPPER_TARGET_HOOK=
	else
		CFLAGS += -DDEBUG_TRACE_METHODS
    endif
    ifeq ($(DEBUG_TRACE_INJ_ARM),1)
        CFLAGS += -DDEBUG_TRACE_INJ_ARM -DDEBUG_TRACE_METHODS
        I_CFLAGS += -DDEBUG_TRACE_INJ_ARM
    else
        INJECTION_WRAPPER_TARGET_ARM=
    endif
    ifeq ($(DEBUG_TRACE_INJ_THUMB),1)
        CFLAGS += -DDEBUG_TRACE_INJ_THUMB -DDEBUG_TRACE_METHODS
        I_CFLAGS += -DDEBUG_TRACE_INJ_THUMB
    else
        INJECTION_WRAPPER_TARGET_THUMB=
    endif
endif

INJECTION_WRAPPER_TARGETS = $(INJECTION_WRAPPER_TARGET_ARM) $(INJECTION_WRAPPER_TARGET_THUMB) $(INJECTION_WRAPPER_TARGET_HOOK)

DEBUG ?= 0
ifeq ($(DEBUG),1)
    CFLAGS += -g -Wall -DLINKER_DEBUG=1 -DAPKENV_DEBUG -Wformat=0
else
    CFLAGS += -O2 -DLINKER_DEBUG=0
endif

all: $(INJECTION_WRAPPER_TARGETS) $(TARGET) $(MODULES)

compat/wrapper.instructions: compat/wrapper.c
	@echo -e "\tCC_I_W\t"$@
	@$(CC) -marm -Wno-unused-function $(I_CFLAGS) -O0 -c -o $(patsubst %.c,%.o,$<) $<
	@objdump -d $(patsubst %.c,%.o,$<) | ./tools/extract_wrapper_code.sh | ./tools/to_code_32.sh > $(patsubst %.c,%.instructions,$<)

linker/wrapper_THUMB.instructions: linker/wrapper_THUMB.c
	@echo -e "\tCC_I_W\t"$@
	@$(CC) -mthumb -Wno-unused-function $(I_CFLAGS) -O0 -c -o $(patsubst %.c,%.o,$<) $<
	@objdump -d $(patsubst %.c,%.o,$<) | ./tools/extract_wrapper_code.sh | ./tools/to_code_16.sh > $(patsubst %.c,%.instructions,$<)

linker/wrapper_ARM.instructions: linker/wrapper_ARM.c
	@echo -e "\tCC_I_W\t"$@
	@$(CC) -marm -Wno-unused-function $(I_CFLAGS) -O0 -c -o $(patsubst %.c,%.o,$<) $<
	@objdump -d $(patsubst %.c,%.o,$<) | ./tools/extract_wrapper_code.sh | ./tools/to_code_32.sh > $(patsubst %.c,%.instructions,$<)

%.o: %.c $(INJECTION_WRAPPER_TARGETS)
	@echo -e "\tCC\t$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	@echo -e "\tLINK\t$@"
	@$(CC) $(LDFLAGS) $(OBJS) -o $@

%.apkenv.so: modules/%.c
	@echo -e "\tMOD\t$@"
	@$(CC) -fPIC -shared $(CFLAGS) -o $@ $<

strip:
	@echo -e "\tSTRIP"
	@strip $(TARGET) $(MODULES)

install: $(TARGET) $(MODULES)
	@echo -e "\tMKDIR"
	@mkdir -p $(DESTDIR)$(PREFIX)/{modules,bionic}
	@mkdir -p $(DESTDIR)/usr/bin
	@echo -e "\tINSTALL\t$(TARGET)"
	@install -m755 $(TARGET) $(DESTDIR)/usr/bin
	@echo -e "\tINSTALL\tMODULES"
	@install -m644 $(MODULES) $(DESTDIR)$(PREFIX)/modules
ifneq ($(PANDORA),1)
	@echo -e "\tINSTALL\tBIONIC"
	@install -m644 $(BIONIC_LIBS) $(DESTDIR)$(PREFIX)/bionic
ifneq ($(FREMANTLE),1)
	@echo -e "\tINSTALL\tHarmattan Resource Policy"
	@install -d -m755 $(DESTDIR)/usr/share/policy/etc/syspart.conf.d
	@install -m644 $(TARGET).conf $(DESTDIR)/usr/share/policy/etc/syspart.conf.d
endif
else
	@echo -e "\tINSTALL\tlibs"
	@mkdir -p $(DESTDIR)$(PREFIX)/system/lib
	@install -m644 libs/pandora/system/lib/*.so $(DESTDIR)$(PREFIX)/system/lib/
endif

clean:
	@echo -e "\tCLEAN"
	@rm -rf $(TARGET) $(OBJS) $(MODULES) $(INJECTION_WRAPPER_TARGETS)


.DEFAULT: all
.PHONY: strip install clean


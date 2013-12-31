
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

# JPEG and PNG loaders
IMAGELIB_SOURCES=$(wildcard imagelib/*.c)

# segfault catch
DEBUG_SOURCES=$(wildcard debug/*.c)

# debug wrappers
WRAPPER_INJECT_ARM_SOURCE=debug/wrappers/wrapper_ARM.c
WRAPPER_INJECT_THUMB_SOURCE=debug/wrappers/wrapper_THUMB.c
WRAPPER_INJECT_GENERIC_SOURCE=debug/wrappers/wrapper_GENERIC.c
WRAPPER_INJECT_SOURCES=$(WRAPPER_INJECT_ARM_SOURCE) $(WRAPPER_INJECT_THUMB_SOURCE) $(WRAPPER_INJECT_GENERIC_SOURCE)
WRAPPER_INJECT_TARGETS=$(patsubst %.c,%.instructions,$(WRAPPER_INJECT_SOURCES))

TARGET = apkenv

# we need objdump for getting a disassembly of the debug-wrappers
OBJDUMP ?= objdump

# enable latehooks
LATEHOOKS ?= 1

ifeq ($(LATEHOOKS),1)
	CFLAGS += -DLATEHOOKS
endif

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
SOURCES += $(WRAPPER_SOURCES)

PANDORA ?= 0
ifeq ($(PANDORA),1)
SOURCES += platform/pandora.c
else
SOURCES += platform/maemo.c
endif

OBJS = $(patsubst %.c,%.o,$(SOURCES))
MODULES = $(patsubst modules/%.c,%.apkenv.so,$(MODULES_SOURCES))

LDFLAGS = -fPIC -ldl -lz -lSDL -lSDL_mixer -pthread -lpng -ljpeg

ifeq ($(PANDORA),1)
CFLAGS += -DPANDORA
LDFLAGS += -lrt
endif

# Selection of OpenGL ES version support (if any) to include
CFLAGS += -DAPKENV_GLES
LDFLAGS += -lGLES_CM
CFLAGS += -DAPKENV_GLES2
LDFLAGS += -lGLESv2 -lEGL

FREMANTLE ?= 0
ifeq ($(FREMANTLE),1)
    CFLAGS += -DFREMANTLE
    LDFLAGS += -lSDL_gles
endif

DEBUG ?= 0
ifeq ($(DEBUG),1)
    CFLAGS += -g -Wall -DLINKER_DEBUG=1 -DAPKENV_DEBUG -Wformat=0
else
    CFLAGS += -O2 -DLINKER_DEBUG=0
endif

all: $(WRAPPER_INJECT_TARGETS) $(TARGET) $(MODULES)

%.o: %.c
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

$(patsubst %.c,%.instructions,$(WRAPPER_INJECT_ARM_SOURCE)): $(WRAPPER_INJECT_ARM_SOURCE)
	@echo -e "\tCC_W\t$<"
	@$(CC) -marm -Wno-unused-function -O0 -c -o $(patsubst %.c,%.o,$<) $<
	@echo -e "\tDEC\t$(patsubst %.c,%.o,$<)"
	@$(OBJDUMP) -d $(patsubst %.c,%.o,$<) | ./tools/extract_wrapper_code.sh | ./tools/to_code_arm.sh > $(patsubst %.c,%.instructions,$<)
	@echo -e "\tSIZE\t$(patsubst %.c,%.instructions,$<)"
	@wc -l $(patsubst %.c,%.instructions,$<) | awk '{print $$1}' > $(patsubst %.c,%.size,$<)

$(patsubst %.c,%.instructions,$(WRAPPER_INJECT_THUMB_SOURCE)): $(WRAPPER_INJECT_THUMB_SOURCE)
	@echo -e "\tCC_W\t$<"
	@$(CC) -mthumb -Wno-unused-function -O0 -c -o $(patsubst %.c,%.o,$<) $<
	@echo -e "\tDEC\t$(patsubst %.c,%.o,$<)"
	@$(OBJDUMP) -d $(patsubst %.c,%.o,$<) | ./tools/extract_wrapper_code.sh | ./tools/to_code_thumb.sh > $(patsubst %.c,%.instructions,$<)
	@echo -e "\tSIZE\t$(patsubst %.c,%.instructions,$<)"
	@wc -l $(patsubst %.c,%.instructions,$<) | awk '{print $$1}' > $(patsubst %.c,%.size,$<)

$(patsubst %.c,%.instructions,$(WRAPPER_INJECT_GENERIC_SOURCE)): $(WRAPPER_INJECT_GENERIC_SOURCE)
	@echo -e "\tCC_W\t$<"
	@$(CC) -marm -Wno-unused-function -O0 -c -o $(patsubst %.c,%.o,$<) $<
	@echo -e "\tDEC\t$(patsubst %.c,%.o,$<)"
	@$(OBJDUMP) -d $(patsubst %.c,%.o,$<) | ./tools/extract_wrapper_code.sh | ./tools/to_code_arm.sh > $(patsubst %.c,%.instructions,$<)
	@echo -e "\tSIZE\t$(patsubst %.c,%.instructions,$<)"
	@wc -l $(patsubst %.c,%.instructions,$<) | awk '{print $$1}' > $(patsubst %.c,%.size,$<)

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
	@rm -rf $(TARGET) $(OBJS) $(MODULES) $(WRAPPER_INJECT_TARGETS) $(patsubst %.c,%.o, $(WRAPPER_INJECT_SOURCES)) $(patsubst %.c,%.size, $(WRAPPER_INJECT_SOURCES))

.DEFAULT: all
.PHONY: strip install clean


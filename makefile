TARGET := apkenv

# Default target, make sure this is always first
all: $(TARGET) modules

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

ifeq ($(EVDEV),1)
SOURCES += touch/evdev.c
CFLAGS += -DEVDEV
endif
# Platform-specific targets and configuration
PLATFORM_INSTALL_TARGETS :=
include platform/$(PLATFORM).mk

OBJS := $(patsubst %.c,%.o,$(SOURCES))

# Support modules for specific applications
MODULES_SOURCES := $(wildcard modules/*.c)
MODULES := $(patsubst modules/%.c,%.apkenv.so,$(MODULES_SOURCES))

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

%.o: %.c
	$(SILENTMSG) -e "\tCC\t$@"
	$(SILENTCMD)$(CC) $(CFLAGS) -c -o $@ $<

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
	$(SILENTCMD)rm -rf $(TARGET) $(OBJS) $(MODULES)

.DEFAULT: all
.PHONY: all modules install clean

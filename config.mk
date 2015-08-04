# Latehooks (experimental)
LATEHOOKS ?= 1

NO_THUMB ?= 0

# Debug build (logging + debug symbols)
DEBUG ?= 0

# Destination directory when installing (used for packaging)
DESTDIR ?= /

# Destination prefix when installing
PREFIX ?= /usr

# Target platform
PLATFORM ?= harmattan

# Set to 1 for verbose compile output
V ?= 0

# Default linker flags
LDFLAGS += -fPIC -rdynamic -pthread -ldl -lz -lpng -ljpeg

ifeq ($(LATEHOOKS),1)
    CFLAGS += -DAPKENV_LATEHOOKS
endif

ifeq ($(DEBUG),1)
    CFLAGS += -g -Wall -DLINKER_DEBUG=1 -DAPKENV_DEBUG -Wformat=0
else
    CFLAGS += -O2 -DLINKER_DEBUG=0
endif

ifeq ($(V),1)
    SILENTMSG := @true
    SILENTCMD :=
else
    SILENTMSG := @echo
    SILENTCMD := @
endif


# Expose some makefile variables as C macros
CFLAGS += -DAPKENV_PREFIX=\"$(PREFIX)\"
CFLAGS += -DAPKENV_TARGET=\"$(TARGET)\"
CFLAGS += -DAPKENV_PLATFORM=\"$(PLATFORM)\"

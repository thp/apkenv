OSMESA_PATH ?= deps/mesa-22.3.4/builddir/install/
ZLIB_PATH ?= deps/zlib-1.2.13/install/

CC := arm-linux-gnueabihf-gcc-12

SOURCES += platform/osmesa.c

CFLAGS += -DAPKENV_OSMESA -DAPKENV_GLES -DAPKENV_GLES2 -Iplatform/osmesa

LDFLAGS += -lm
LDFLAGS += -lrt -Wl,-rpath,.
LDFLAGS += -Wl,-rpath,/usr/arm-linux-gnueabihf/lib

# OSMesa
LDFLAGS += -lOSMesa -L$(OSMESA_PATH)/lib
LDFLAGS += -Wl,-rpath,$(OSMESA_PATH)/lib
CFLAGS += -I$(OSMESA_PATH)/include

# zlib
LDFLAGS += -L$(ZLIB_PATH)/lib -lz
LDFLAGS += -Wl,-rpath,$(ZLIB_PATH)/lib
CFLAGS += -I$(ZLIB_PATH)/include

BIONIC_LIBS := $(wildcard libs/harmattan/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/harmattan/\"

hostui: hostui.c
	$(HOSTCC) -o $@ -D_GNU_SOURCE $< $(shell sdl2-config --libs --cflags) -lSDL2_mixer -lGL

PLATFORM_TARGETS += hostui

SDL_CFLAGS :=
SDL_LDLIBS :=

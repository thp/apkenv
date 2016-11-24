SOURCES += platform/pocketchip.c
LDFLAGS += -leglo -L../eglo -lm -lrt -lSDL -lSDL_mixer -Wl,-rpath,. -Wl,-rpath,../eglo
CFLAGS += -DAPKENV_POCKETCHIP -DAPKENV_GLES -DAPKENV_GLES2 -DNO_IMGLIB_JPEG_MEM_SRC -I../eglo -I/usr/include/SDL
LDFLAGS += -lGLESv1_CM -lGLESv2

BIONIC_LIBS := $(wildcard libs/harmattan/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/harmattan/\"

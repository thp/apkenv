SOURCES += platform/rpi3.c
CFLAGS += -DAPKENV_RPI3 -DAPKENV_GLES -DAPKENV_GLES2
LDFLAGS += -lGLESv1_CM -lGLESv2 -lSDL_mixer -lEGL -lm -lX11 -lrt -lGL

BIONIC_LIBS := $(wildcard libs/harmattan/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/harmattan/\"

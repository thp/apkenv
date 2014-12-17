SOURCES += platform/pandora.c
CFLAGS += -DPANDORA
LDFLAGS += -lSDL_mixer -lrt
CFLAGS += -DAPKENV_GLES -DAPKENV_GLES2
LDFLAGS += -lGLES_CM -lGLESv2 -lEGL

BIONIC_LIBS := $(wildcard libs/pandora/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/pandora/\"

install_pandora:
	$(SILENTMSG) -e "\tINSTALL\tBIONIC"
	$(SILENTCMD)mkdir -p $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic
	$(SILENTCMD)install -m644 $(BIONIC_LIBS) $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic

PLATFORM_INSTALL_TARGETS += install_pandora
.PHONY: install_pandora

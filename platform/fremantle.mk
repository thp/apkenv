SOURCES += platform/fremantle.c
CFLAGS += -DFREMANTLE
LDFLAGS += -lSDL_mixer -lSDL_gles
CFLAGS += -DAPKENV_GLES -DAPKENV_GLES2
LDFLAGS += -lGLES_CM -lGLESv2 -lEGL

# We can re-use the Bionic libs from Harmattan
BIONIC_LIBS := $(wildcard libs/harmattan/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/harmattan/\"

install_fremantle:
	$(SILENTMSG) -e "\tINSTALL\tBIONIC"
	$(SILENTCMD)mkdir -p $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic
	$(SILENTCMD)install -m644 $(BIONIC_LIBS) $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic

PLATFORM_INSTALL_TARGETS += install_fremantle
.PHONY: install_fremantle

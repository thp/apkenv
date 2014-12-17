SOURCES += platform/sailfish.c
LDFLAGS += -lSDL2_mixer -lm -lrt
CFLAGS += -DAPKENV_GLES -DAPKENV_GLES2 -DMESA_EGL_NO_X11_HEADERS -DNO_IMGLIB_JPEG_MEM_SRC
LDFLAGS += -lGLESv1_CM -lGLESv2 -lEGL

BIONIC_LIBS := $(wildcard libs/sailfish/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/sailfish/\"

install_sailfish:
	$(SILENTMSG) -e "\tINSTALL\tBIONIC"
	$(SILENTCMD)mkdir -p $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic
	$(SILENTCMD)install -m644 $(BIONIC_LIBS) $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic

PLATFORM_INSTALL_TARGETS += install_sailfish
.PHONY: install_sailfish

rpm:
	echo "rpm TARGET TODO"

.PHONY: rpm

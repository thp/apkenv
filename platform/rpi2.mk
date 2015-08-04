SOURCES += platform/rpi2.c
LDFLAGS += -lSDL2_mixer -lm -lrt
CFLAGS += -DAPKENV_GLES -DAPKENV_GLES2 -DMESA_EGL_NO_X11_HEADERS -DNO_IMGLIB_JPEG_MEM_SRC -DNO_THUMB
LDFLAGS += -lGLESv1_CM -lGLESv2 -lEGL

NO_THUMB := 1

BIONIC_LIBS := $(wildcard libs/harmattan/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/harmattan/\"

# only install bionic libs if we actually have them
ifneq ($(BIONIC_LIBS),)
install_rpi2:
	$(SILENTMSG) -e "\tINSTALL\tBIONIC"
	$(SILENTCMD)mkdir -p $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic
	$(SILENTCMD)install -m644 $(BIONIC_LIBS) $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic

PLATFORM_INSTALL_TARGETS += install_rpi2
.PHONY: install_rpi2
endif


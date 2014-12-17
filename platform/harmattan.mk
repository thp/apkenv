SOURCES += platform/harmattan.c
LDFLAGS += -lSDL_mixer
CFLAGS += -DAPKENV_GLES -DAPKENV_GLES2
LDFLAGS += -lGLES_CM -lGLESv2 -lEGL

BIONIC_LIBS := $(wildcard libs/harmattan/*.so)

CFLAGS += -DAPKENV_LOCAL_BIONIC_PATH=\"./libs/harmattan/\"

install_harmattan:
	$(SILENTMSG) -e "\tINSTALL\tBIONIC"
	$(SILENTCMD)mkdir -p $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic
	$(SILENTCMD)install -m644 $(BIONIC_LIBS) $(DESTDIR)$(PREFIX)/lib/$(TARGET)/bionic
	$(SILENTMSG) -e "\tINSTALL\tHarmattan Resource Policy"
	$(SILENTCMD)install -d -m755 $(DESTDIR)/usr/share/policy/etc/syspart.conf.d
	$(SILENTCMD)install -m644 platform/harmattan/$(TARGET).conf $(DESTDIR)/usr/share/policy/etc/syspart.conf.d

PLATFORM_INSTALL_TARGETS += install_harmattan
.PHONY: install_harmattan

deb:
	ln -sf platform/harmattan/debian .
	dpkg-buildpackage -rfakeroot -b
	rm -f debian

.PHONY: deb

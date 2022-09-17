TARGET := egloapp
TARGET_OBJ := egloapp.o
TARGET_LDFLAGS += -lGLESv2 -leglo -L. -Wl,-rpath .

SHARED := libeglo.so
SHARED_OBJ := eglo.o
SHARED_LDFLAGS += -lEGL -lGLESv2 -lGLESv1_CM -lX11

DEB := eglo.deb

CXXFLAGS += -O2 -std=c++14 -fPIC

PREFIX ?= /usr/local
DESTDIR ?=

all: $(TARGET) $(SHARED)

$(TARGET): $(TARGET_OBJ) $(SHARED)
	$(CXX) -o $@ $< $(TARGET_LDFLAGS)

$(SHARED): $(SHARED_OBJ)
	$(CXX) -shared $(CXXFLAGS) -o $@ $< $(SHARED_LDFLAGS)

install: $(SHARED)
	mkdir -p $(DESTDIR)$(PREFIX)/lib $(DESTDIR)$(PREFIX)/include
	install -m 644 $(SHARED) $(DESTDIR)$(PREFIX)/lib/
	install -m 644 eglo.h $(DESTDIR)$(PREFIX)/include/

deb: $(DEB)

$(DEB): $(SHARED)
	rm -rf tmp data.tar.gz control.tar.gz control debian-binary $(DEB)
	$(MAKE) install DESTDIR=$(shell pwd)/tmp/ PREFIX=/usr
	strip -s tmp/usr/lib/$(SHARED)
	mkdir -p tmp/usr/share/doc/eglo/
	cp README LICENSE tmp/usr/share/doc/eglo/
	tar cJvf data.tar.xz -C tmp usr
	sed -e "s/--SIZE--/`du -sk tmp | awk '{ print $$1 }'`/" \
	    -e "s/--VERSION--/`awk '/@\(#\)/ { print $$3 }' eglo.cpp`/" control.in > control
	tar cJvf control.tar.xz control
	echo 2.0 > debian-binary
	ar r $(DEB) debian-binary control.tar.xz data.tar.xz
	rm -rf debian-binary control.tar.xz data.tar.xz control tmp

clean:
	$(RM) $(TARGET) $(TARGET_OBJ) $(SHARED) $(SHARED_OBJ) $(DEB)

distclean: clean

.PHONY: all install deb clean distclean

Name: apkenv
Version: 42.23.0
Release: 1
Summary: A compatibility layer for applications packaged as ".apk".
Group: System/Base
License: BSD
Source0: %{name}-%{version}.tar.gz
URL: https://thp.io/2012/apkenv/
BuildRequires: pkgconfig(egl)
BuildRequires: pkgconfig(glesv1_cm)
BuildRequires: pkgconfig(glesv2)
BuildRequires: pkgconfig(sdl2)
BuildRequires: pkgconfig(libpng)
BuildRequires: libjpeg-turbo-devel
BuildRequires: SDL2_mixer-devel
BuildRequires: pkgconfig(audioresource)
BuildRequires: pkgconfig(glib-2.0)

%description
%{summary}.

%prep
%setup -q -n %{name}-%{version}

%build
make PLATFORM=sailfish

%install
rm -rf %{buildroot}
make PLATFORM=sailfish DESTDIR=%{buildroot} install

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_libdir}/%{name}/modules/*.%{name}.so
%{_libdir}/%{name}/bionic/libdvm_compat.so

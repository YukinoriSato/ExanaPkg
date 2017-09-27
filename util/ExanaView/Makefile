# =========================================================================
#    wxWidgets demos and samples are used as reference for this makefile
# =========================================================================


prefix = /usr/local
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
INSTALL = /usr/bin/install -c
EXEEXT = 
WINDRES = 
SETFILE = 
NM = nm
BK_DEPS = /home/ysato/wxWidgets/gtk_build/bk-deps
#BK_DEPS = /work/wxWidgets/wxWidgets/build_gtk/bk-deps
srcdir = .
top_srcdir = /home/ysato/wxWidgets/
#top_srcdir = /work/wxWidgets/wxWidgets
LIBS = -lz -ldl -lm
LDFLAGS_GUI = 
CXX = g++
#CXXFLAGS = -DWX_PRECOMP -pthread -O2 -fno-strict-aliasing -I/usr/include/gtk-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include -I/usr/lib64/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gtk-unix-print-2.0 -I/usr/include/gtk-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/lib64/gtk-2.0/include -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -fvisibility=hidden -fvisibility-inlines-hidden
CXXFLAGS = -DWX_PRECOMP -pthread -O2 -pthread -I/usr/include/gtk-unix-print-2.0 -I/usr/include/gtk-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/lib64/gtk-2.0/include -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/libdrm -I/usr/include/harfbuzz -fvisibility=hidden -fvisibility-inlines-hidden
#CPPFLAGS = -D_FILE_OFFSET_BITS=64 -I${wx_top_builddir}/lib/wx/include/gtk2-unicode-3.1 -I${top_srcdir}/include -pthread -I/usr/include/gtk-2.0 -I/usr/lib64/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -pthread -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/libxml2
CPPFLAGS = -D_FILE_OFFSET_BITS=64 -I${wx_top_builddir}/lib/wx/include/gtk2-unicode-3.1 -I${top_srcdir}/include -pthread -I/usr/include/gtk-2.0 -I/usr/lib64/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/libdrm -I/usr/include/harfbuzz

LDFLAGS = -pthread   
WX_LIB_FLAVOUR = 
TOOLKIT = GTK
TOOLKIT_LOWERCASE = gtk
TOOLKIT_VERSION = 2
TOOLCHAIN_FULLNAME = gtk2-unicode-3.1
EXTRALIBS = -pthread   -Wl,--version-script,$(wx_top_builddir)/version-script -lz -ldl -lm 
EXTRALIBS_HTML = 
#EXTRALIBS_XML =  -lexpat
#EXTRALIBS_GUI = -pthread -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lz -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 -lX11 -lXxf86vm -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lz -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -L/lib64 -lnotify -lgtk-x11-2.0 -ldbus-glib-1 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lz -lfontconfig -lgmodule-2.0 -ldbus-1 -lgobject-2.0 -lglib-2.0 -lpng -lz -ljpeg
EXTRALIBS_GUI = -pthread -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lgobject-2.0 -lfreetype -lgthread-2.0 -lglib-2.0 -lX11 -lXxf86vm -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lgobject-2.0 -lglib-2.0 -lfreetype -lpng -lz

# -ltiff
EXTRALIBS_SDL = 
CXXWARNINGS = -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual
HOST_SUFFIX = 
SAMPLES_RPATH_FLAG = -Wl,-rpath,$(wx_top_builddir)/lib
SAMPLES_CXXFLAGS = 
wx_top_builddir = /home/ysato/wxWidgets/gtk_build
#wx_top_builddir = /work/wxWidgets/wxWidgets/build_gtk

### Variables: ###

DESTDIR = 
WX_RELEASE = 3.1
#WX_VERSION = $(WX_RELEASE).0
WX_VERSION = $(WX_RELEASE).1
LIBDIRNAME = $(wx_top_builddir)/lib
#	-I$(srcdir) $(__DLLFLAG_p) -I/home/ysato/wxWidgets/samples $(CXXWARNINGS) 
EXANAV_CXXFLAGS = -D__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) \
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) \
	-I$(srcdir) $(__DLLFLAG_p) -I/home/ysato/wxWidgets/gtk_build/samples $(CXXWARNINGS) \
	$(SAMPLES_CXXFLAGS) $(CPPFLAGS) $(CXXFLAGS)
EXANAV_OBJECTS =  \
	exanav_exanav.o \
	exanav_exanavadd.o \
	$(__exanav___win32rc)

### Conditionally set variables: ###

#CXXC = $(CXX)
CXXC = $(BK_DEPS) $(CXX)
PLUGIN_ADV_EXTRALIBS = $(EXTRALIBS_SDL)
#PORTNAME = base
PORTNAME = $(TOOLKIT_LOWERCASE)$(TOOLKIT_VERSION)
#WXBASEPORT = _carbon
#WXDEBUGFLAG = d
WXUNICODEFLAG = u
#WXUNIVNAME = univ
EXTRALIBS_FOR_BASE = $(EXTRALIBS)
#EXTRALIBS_FOR_BASE = $(EXTRALIBS) \
#	$(EXTRALIBS_XML) $(EXTRALIBS_GUI)
EXTRALIBS_FOR_GUI = $(EXTRALIBS_GUI)
#EXTRALIBS_FOR_GUI = 
__exanav___mac_setfilecmd = @true
#__exanav___mac_setfilecmd = \
#	$(SETFILE) -t APPL exanav$(EXEEXT)
#__WXUNIV_DEFINE_p = -D__WXUNIVERSAL__
#__WXUNIV_DEFINE_p_2 = --define __WXUNIVERSAL__
#__DEBUG_DEFINE_p = -DwxDEBUG_LEVEL=0
#__DEBUG_DEFINE_p_2 = --define wxDEBUG_LEVEL=0
#__EXCEPTIONS_DEFINE_p = -DwxNO_EXCEPTIONS
#__EXCEPTIONS_DEFINE_p_2 = --define wxNO_EXCEPTIONS
#__RTTI_DEFINE_p = -DwxNO_RTTI
#__RTTI_DEFINE_p_2 = --define wxNO_RTTI
#__THREAD_DEFINE_p = -DwxNO_THREADS
#__THREAD_DEFINE_p_2 = --define wxNO_THREADS
__DLLFLAG_p = -DWXUSINGDLL
__DLLFLAG_p_2 = --define WXUSINGDLL
COND_PLATFORM_OS2_1___exanav___os2_emxbindcmd = $(NM) exanav$(EXEEXT) | if grep -q \
	pmwin.763 ; then emxbind -ep exanav$(EXEEXT) ; fi
#__exanav___os2_emxbindcmd = $(COND_PLATFORM_OS2_1___exanav___os2_emxbindcmd)
#__RCDEFDIR_p_1 = --include-dir \
#	$(LIBDIRNAME)/wx/include/$(TOOLCHAIN_FULLNAME)
#__exanav_app_Contents_PkgInfo___depname \
#	= exanav.app/Contents/PkgInfo
#__exanav_bundle___depname = exanav_bundle
#____exanav_BUNDLE_TGT_REF_DEP = \
#	$(__exanav_app_Contents_PkgInfo___depname)
#____exanav_BUNDLE_TGT_REF_DEP \
#	= $(__exanav_app_Contents_PkgInfo___depname)
#____exanav_BUNDLE_TGT_REF_DEP \
#	= $(__exanav_app_Contents_PkgInfo___depname)
#____exanav_BUNDLE_TGT_REF_DEP \
#	= $(__exanav_app_Contents_PkgInfo___depname)
#____exanav_BUNDLE_TGT_REF_DEP = \
#	$(__exanav_app_Contents_PkgInfo___depname)
COND_MONOLITHIC_0___WXLIB_AUI_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_aui-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_AUI_p = $(COND_MONOLITHIC_0___WXLIB_AUI_p)
COND_MONOLITHIC_0___WXLIB_HTML_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_html-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_HTML_p = $(COND_MONOLITHIC_0___WXLIB_HTML_p)
COND_MONOLITHIC_0___WXLIB_ADV_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_adv-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_ADV_p = $(COND_MONOLITHIC_0___WXLIB_ADV_p)
COND_MONOLITHIC_0___WXLIB_XML_p = \
	-lwx_base$(WXBASEPORT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_xml-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_XML_p = $(COND_MONOLITHIC_0___WXLIB_XML_p)
COND_MONOLITHIC_0___WXLIB_STC_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_stc-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_STC_p = $(COND_MONOLITHIC_0___WXLIB_STC_p)
COND_MONOLITHIC_0___WXLIB_CORE_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_CORE_p = $(COND_MONOLITHIC_0___WXLIB_CORE_p)
COND_MONOLITHIC_0___WXLIB_BASE_p = \
	-lwx_base$(WXBASEPORT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_BASE_p = $(COND_MONOLITHIC_0___WXLIB_BASE_p)
#__exanav___win32rc = exanav_exanav_rc.o
COND_MONOLITHIC_1___WXLIB_MONO_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
#__WXLIB_MONO_p = $(COND_MONOLITHIC_1___WXLIB_MONO_p)
#__LIB_SCINTILLA_IF_MONO_p \
#	= \
#	-lwxscintilla$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)

__LIB_TIFF_p \
	= \
	-lwxtiff$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_JPEG_p \
	= \
	-lwxjpeg$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)

#__LIB_PNG_p \
#	= \
#	-lwxpng$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
#__LIB_ZLIB_p = \
#	-lwxzlib$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
COND_wxUSE_REGEX_builtin___LIB_REGEX_p = \
	-lwxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_REGEX_p = $(COND_wxUSE_REGEX_builtin___LIB_REGEX_p)
#__LIB_EXPAT_p = \
#	-lwxexpat$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)

### Targets: ###

all: exanav$(EXEEXT) $(__exanav_bundle___depname)

install: 

uninstall: 

install-strip: install

clean: 
	rm -rf ./.deps ./.pch
	rm -f ./*.o
	rm -f exanav$(EXEEXT)
	rm -rf exanav.app

distclean: clean
	rm -f config.cache config.log config.status bk-deps bk-make-pch shared-ld-sh Makefile

exanav$(EXEEXT): $(EXANAV_OBJECTS) $(__exanav___win32rc)
#	$(CXX) -o $@ $(EXANAV_OBJECTS)    -L$(LIBDIRNAME) $(LDFLAGS_GUI) $(SAMPLES_RPATH_FLAG) $(LDFLAGS)  $(__WXLIB_AUI_p)  $(__WXLIB_HTML_p) $(EXTRALIBS_HTML) $(__WXLIB_ADV_p) $(PLUGIN_ADV_EXTRALIBS)  $(__WXLIB_XML_p) $(EXTRALIBS_XML)  $(__WXLIB_STC_p)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  -lwxscintilla$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)  $(__WXLIB_MONO_p) $(__LIB_SCINTILLA_IF_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  $(EXTRALIBS_FOR_GUI) $(__LIB_ZLIB_p) $(__LIB_REGEX_p) $(__LIB_EXPAT_p) $(EXTRALIBS_FOR_BASE) $(LIBS)
	$(CXX) -o $@ $(EXANAV_OBJECTS)    -L$(LIBDIRNAME) $(LDFLAGS_GUI) $(SAMPLES_RPATH_FLAG) $(LDFLAGS)  $(__WXLIB_AUI_p)  $(__WXLIB_HTML_p) $(EXTRALIBS_HTML) $(__WXLIB_ADV_p) $(PLUGIN_ADV_EXTRALIBS)  $(__WXLIB_XML_p) $(EXTRALIBS_XML)  $(__WXLIB_STC_p)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  -lwxscintilla$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)  $(__WXLIB_MONO_p) $(__LIB_SCINTILLA_IF_MONO_p) $(__LIB_TIFF_p) $(__LIB_PNG_p)  $(EXTRALIBS_FOR_GUI) $(__LIB_ZLIB_p) $(__LIB_REGEX_p) $(__LIB_EXPAT_p) $(EXTRALIBS_FOR_BASE) $(LIBS)


	$(__exanav___mac_setfilecmd)
	$(__exanav___os2_emxbindcmd)

#exanav.app/Contents/PkgInfo: exanav$(EXEEXT) $(top_srcdir)/src/osx/carbon/Info.plist.in $(top_srcdir)/src/osx/carbon/wxmac.icns
#	mkdir -p exanav.app/Contents
#	mkdir -p exanav.app/Contents/MacOS
#	mkdir -p exanav.app/Contents/Resources
#	
#	
#	sed -e "s/IDENTIFIER/`echo $(srcdir) | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
#	-e "s/EXECUTABLE/exanav/" \
#	-e "s/VERSION/$(WX_VERSION)/" \
#	$(top_srcdir)/src/osx/carbon/Info.plist.in >exanav.app/Contents/Info.plist
#	
#	
#	/bin/echo "APPL????" >exanav.app/Contents/PkgInfo
#	
#	
#	ln -f exanav$(EXEEXT) exanav.app/Contents/MacOS/exanav
#	
#	
#	cp -f $(top_srcdir)/src/osx/carbon/wxmac.icns exanav.app/Contents/Resources/wxmac.icns

#exanav_bundle: $(____exanav_BUNDLE_TGT_REF_DEP)

exanav_exanav.o: $(srcdir)/exanav.cpp
	$(CXXC) -c -o $@ $(EXANAV_CXXFLAGS) $(srcdir)/exanav.cpp

exanav_exanavadd.o: $(srcdir)/exanavadd.cpp
	$(CXXC) -c -o $@ $(EXANAV_CXXFLAGS) $(srcdir)/exanavadd.cpp

exanav_exanav_rc.o: $(srcdir)/exanav.rc
	#$(WINDRES) -i$< -o$@    --define __WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_2) $(__DEBUG_DEFINE_p_2)  $(__EXCEPTIONS_DEFINE_p_2) $(__RTTI_DEFINE_p_2) $(__THREAD_DEFINE_p_2)  --include-dir $(srcdir) $(__DLLFLAG_p_2) --include-dir  /home/ysato/wxWidgets/samples $(__RCDEFDIR_p_1) --include-dir $(top_srcdir)/include
#	$(WINDRES) -i$< -o$@    --define __WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_2) $(__DEBUG_DEFINE_p_2)  $(__EXCEPTIONS_DEFINE_p_2) $(__RTTI_DEFINE_p_2) $(__THREAD_DEFINE_p_2)  --include-dir $(srcdir) $(__DLLFLAG_p_2) --include-dir  /work/wxWidgets/build_gtk/samples $(__RCDEFDIR_p_1) --include-dir $(top_srcdir)/include
	$(WINDRES) -i$< -o$@    --define __WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_2) $(__DEBUG_DEFINE_p_2)  $(__EXCEPTIONS_DEFINE_p_2) $(__RTTI_DEFINE_p_2) $(__THREAD_DEFINE_p_2)  --include-dir $(srcdir) $(__DLLFLAG_p_2) --include-dir  /home/ysato/wxWidgets/gtk_build/samples $(__RCDEFDIR_p_1) --include-dir $(top_srcdir)/include



# Include dependency info, if present:
-include ./.deps/*.d

.PHONY: all install uninstall clean distclean exanav_bundle

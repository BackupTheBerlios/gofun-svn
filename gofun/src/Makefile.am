bin_PROGRAMS = gofun gosu
MOCFILES = gofun.cpp gofun_cat_settings.cpp gofun_cat_button.cpp \
		gofun_item.cpp gofun_settings.cpp gofun_settings_dlg.cpp gofun_widget.cpp \
		gofun_item_settings.cpp gofun_cstart.cpp gofun_help.cpp
gofun_SOURCES = gofun.cpp gofun_cat_settings.cpp gofun_cat_button.cpp \
					gofun_item.cpp gofun_settings.cpp gofun_settings_dlg.cpp gofun_widget.cpp \
					gofun_item_settings.cpp gofun_cstart.cpp $(MOCFILES:.cpp=_moc.cpp) gofun_data.cpp \
				gofun_misc.cpp gofun_iconview.cpp gofun_help.cpp gofun_about.cpp
# set the include path found by configure
INCLUDES= -I$(top_srcdir)/include $(QT_INCLUDES) $(all_includes)

METASOURCES = AUTO
# the library search path.
gofun_LDFLAGS = $(all_libraries) 

gofun_LDADD = $(QT_LDFLAGS) $(QT_LIB)
noinst_HEADERS = gofun_widget.h gofun_item.h gofun_item_settings.h \
				gofun_settings_dlg.h gofun_cat_button.h gofun_iconview.h gofun_cat_settings.h gofun_cstart.h \
				gofun_settings.h gofun.h gofun_data.h gofun_misc.h gofun_help.h gofun_about.h
	
gofun_MOC = gofun_widget_moc.cpp
	
%.h: %.ui
	$(UIC) $< -o $@
%.cpp: %.h %.ui
	$(UIC) -impl $^ -o $@
%_moc.cpp: %.h
	$(MOC) -o $@ $<
gosu_SOURCES = gosu.cpp
gosu_LDADD = $(QT_LIB) $(QT_LDFLAGS) -lcrypt

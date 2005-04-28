bin_PROGRAMS = gofun gosu golauncher
MOCFILES = gofun.cpp gofun_cat_settings.cpp gofun_cat_button.cpp \
		gofun_item.cpp gofun_settings.cpp gofun_settings_dlg.cpp gofun_widget.cpp \
		gofun_item_settings.cpp gofun_custom_start.cpp gofun_help.cpp gofun_list_dialog.cpp \
		gofun_command_editor.cpp gofun_item_wizard.cpp gofun_iconview.cpp \
		gofun_settings_wizard.cpp gofun_application_item.cpp \
		gofun_application_item_settings.cpp gofun_directory_browser.cpp gofun_icon_dialog.cpp gofun_fsdevice_item.cpp gofun_link_item.cpp gofun_fsdevice_item_settings.cpp gofun_link_item_settings.cpp gofun_url_composer.cpp gofun_parameter_edit.cpp gofun_list_popup.cpp gofun_desktop_entry_settings_widget.cpp gofun_executable_browser.cpp gofun_process_logger.cpp gofun_parameter_decimal_edit_widget.cpp gofun_parameter_integer_edit_widget.cpp gofun_parameter_string_edit_widget.cpp gofun_envvar_edit.cpp
gofun_SOURCES = gofun.cpp gofun_cat_settings.cpp gofun_cat_button.cpp \
			gofun_item.cpp gofun_settings.cpp gofun_settings_dlg.cpp gofun_widget.cpp \
			gofun_item_settings.cpp gofun_custom_start.cpp $(MOCFILES:.cpp=_moc.cpp) gofun_data.cpp \
			gofun_iconview.cpp gofun_help.cpp gofun_about.cpp gofun_desktop_object.cpp \
			gofun_list_dialog.cpp gofun_command_editor.cpp gofun_item_wizard.cpp \
			gofun_settings_wizard.cpp gofun_application_item.cpp gofun_application_item_settings.cpp \
			gofun_icon_dialog.cpp gofun_directory_browser.cpp gofun_fsdevice_item.cpp gofun_link_item.cpp \
			gofun_fsdevice_item_settings.cpp gofun_link_item_settings.cpp gofun_desktop_entry_settings_widget.cpp \
			gofun_url_composer.cpp gofun_parameter_edit.cpp gofun_list_popup.cpp \
			gofun_executable_browser.cpp gofun_file_dialog_preview.cpp gofun_file_dialog.cpp \
			gofun_parameter_edit_widget.cpp gofun_parameter_integer_edit_widget.cpp \
			gofun_parameter_string_edit_widget.cpp gofun_parameter_decimal_edit_widget.cpp \
		gofun_command_editor_options.cpp gofun_envvar_edit.cpp
# set the include path found by configure
INCLUDES = -I$(top_srcdir)/include $(QT_INCLUDES) $(all_includes)

METASOURCES = AUTO
# the library search path.
gofun_LDFLAGS = $(all_libraries)
gofun_CPPFLAGS = -DQT_THREAD_SUPPORT

gofun_LDADD = $(top_builddir)/src/libgofun.a $(QT_LDFLAGS) $(QT_LIB)
noinst_HEADERS = gofun_widget.h gofun_item.h gofun_item_settings.h \
		gofun_settings_dlg.h gofun_cat_button.h gofun_iconview.h gofun_cat_settings.h \
		gofun_custom_start.h gofun_settings.h gofun.h gofun_data.h gofun_misc.h gofun_help.h \
		gofun_about.h gofun_desktop_object.h gofun_list_dialog.h gofun_command_editor.h \
		gofun_item_wizard.h gofun_parameter_prompt.h gofun_settings_wizard.h \
		gofun_application_item.h gofun_application_item_settings.h gofun_icon_dialog.h \
		gofun_directory_browser.h gofun_fsdevice_item.h gofun_link_item.h gofun_fsdevice_item_settings.h \
		gofun_link_item_settings.h gofun_locale_string.h gofun_desktop_entry_settings_widget.h \
		gofun_url_composer.h gofun_parameter_edit.h gofun_password_dialog.h gofun_list_popup.h \
		gofun_executable_browser.h gofun_process_logger.h gofun_file_dialog_preview.h gofun_file_dialog.h \
		gofun_process_problem_feedback.h gofun_desktop_entry_data.h gofun_application_entry_data.h \
		gofun_fsdevice_entry_data.h gofun_link_entry_data.h gofun_cat_entry_data.h \
		gofun_parameter_prompt_widget.h gofun_parameter_string_prompt_widget.h \
		gofun_parameter_integer_prompt_widget.h gofun_parameter_decimal_prompt_widget.h gofun_decimal_spinbox.h \
		gofun_parameter_edit_widget.h gofun_parameter_string_edit_widget.h \
		gofun_parameter_integer_edit_widget.h gofun_parameter_decimal_edit_widget.h gofun_settings_container.h \
		gofun_command_editor_options.h gofun_process.h golauncher_widget.h gofun_envvar_edit.h
	
gofun_MOC = gofun_widget_moc.cpp

install-exec-local: gosu
	chmod a=rx,u+s $(DESTDIR)$(bindir)/gosu
	
%.h: %.ui
	$(UIC) $< -o $@
%.cpp: %.h %.ui
	$(UIC) -impl $^ -o $@
%_moc.cpp: %.h
	$(MOC) -o $@ $<
gosu_SOURCES = gosu.cpp
gosu_LDADD = $(top_builddir)/src/libgofun.a $(QT_LIB) $(QT_LDFLAGS) -lcrypt
golauncher_MOCFILES = golauncher_widget.cpp
golauncher_SOURCES = golauncher.cpp gofun_process_problem_feedback.cpp \
	gofun_process.cpp golauncher_widget.cpp $(golauncher_MOCFILES:.cpp=_moc.cpp)
golauncher_LDADD = $(top_builddir)/src/libgofun.a $(QT_LDFLAGS) $(QT_LIB)

lib_LIBRARIES = libgofun.a
libgofun_a_MOCFILES = gofun_password_dialog.cpp gofun_process_logger.cpp gofun_parameter_prompt.cpp 
libgofun_a_SOURCES = gofun_misc.cpp \
		gofun_process_logger.cpp	gofun_password_dialog.cpp $(libgofun_a_MOCFILES:.cpp=_moc.cpp) gofun_application_entry_data.cpp \
		gofun_cat_entry_data.cpp gofun_data.cpp gofun_desktop_entry_data.cpp \
		gofun_fsdevice_entry_data.cpp gofun_link_entry_data.cpp gofun_parameter_decimal_prompt_widget.cpp \
		gofun_parameter_integer_prompt_widget.cpp gofun_parameter_prompt.cpp gofun_parameter_prompt_widget.cpp \
		gofun_parameter_string_prompt_widget.cpp gofun_locale_string.cpp gofun_decimal_spinbox.cpp \
	gofun_settings_container.cpp
bin_SCRIPTS = golauncherterm

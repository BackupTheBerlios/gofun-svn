/***************************************************************************
 *   Copyright (C) 2005 by Tobias Glaesser                                 *
 *   tobi.web@gmx.de                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <qstring.h>
 
#ifndef GOFUN_SETTINGS_CONTAINER
#define GOFUN_SETTINGS_CONTAINER

class QSettings;

///Settings container
/** Singleton that loads/saves application settings.
    To get a settings you can use GSC::get()->_setting_. */
class GofunSettingsContainer
{
public:
	~GofunSettingsContainer();
	///Get the container
	static GofunSettingsContainer* get() { _instance ? _instance : _instance = new GofunSettingsContainer(); return _instance; }
	///Save the settings
	static void save() { delete _instance; _instance = 0; }
	
	//the different settings
	QString terminal_cmd;
	QString filemanager_cmd;
	QString browser_cmd;
	QString gofun_dir;
	QString style;
	QString color_source;
	QString custom_color;
	QString main_width;
	QString main_height;
	QString main_x;
	QString main_y;
	QString save_main_geom;
	QString ce_completion_popup;
private:
	//private constructor (singleton)
	GofunSettingsContainer();
	
	QSettings* m_settings;
	static GofunSettingsContainer* _instance;
};

typedef GofunSettingsContainer GSC;

#endif

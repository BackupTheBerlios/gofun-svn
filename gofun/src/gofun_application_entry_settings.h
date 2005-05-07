/***************************************************************************
 *   Copyright (C) 2004 by Tobias Glaesser                                 *
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

#include <qlineedit.h>
 
#include "gofun_entry_settings.h"
#include "gofun_application_entry_data.h"

#ifndef GOFUN_APPLICATION_ITEM_SETTINGS
#define GOFUN_APPLICATION_ITEM_SETTINGS

class GofunApplicationEntrySettingsParameter;
class GofunApplicationEntrySettingsAdvanced;
class GofunApplicationEntrySettingsEnvironment;

///Settings-dialog for application entries
class GofunApplicationEntrySettings : public GofunDesktopEntrySettings
{
	Q_OBJECT
public:
	GofunApplicationEntrySettings();
	void load(GofunApplicationEntryData*);
	void setDefaults();
	
public slots:
	void iconDialog();
	void dirDialog();
	void commandEditor();
private:
	void save();
	void apply();
	void apply(GofunApplicationEntryData*);
	bool inputValid();
	
	GofunApplicationEntryData* data();

	QLineEdit* command;
	QToolButton* command_button;
	QLineEdit* directory;
	QToolButton* dir_button;
	GofunApplicationEntrySettingsAdvanced* widget_adv;
	GofunApplicationEntrySettingsParameter* widget_par;
	GofunApplicationEntrySettingsEnvironment* widget_env;

friend class GofunCommandEditor;
};

#endif


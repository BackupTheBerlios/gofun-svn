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
#include <qtoolbutton.h>
#include <qdir.h>

#ifndef GOFUN_CAT_SETTINGS
#define GOFUN_CAT_SETTINGS

#include "gofun_settings_dlg.h" 

class GofunCatButton;
class GofunDesktopEntrySettingsWidget;

///Settings-dialog for categories
class GofunCatSettings : public GofunSettingsDlg
{
	Q_OBJECT
public:
	GofunCatSettings();
	virtual void load(GofunCatButton*);
	void setDefaults();

public slots:
	void iconDialog();
	void backgroundDialog();
private:
	virtual void save();
	virtual void apply();
	virtual bool inputValid();

	GofunDesktopEntrySettingsWidget* desw;
	QLineEdit* background;
	QToolButton* background_button;
	QDir directory;
	GofunCatButton* item;
};

#endif

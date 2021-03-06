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

#include "gofun_desktop_entry_settings.h"
#include "gofun_link_entry_data.h"

#ifndef GOFUN_LINK_ITEM_SETTINGS
#define GOFUN_LINK_ITEM_SETTINGS

///Settings-dialog for link entries
class GofunLinkEntrySettings : public GofunDesktopEntrySettings
{
	Q_OBJECT
public:
	GofunLinkEntrySettings();
	void load(GofunLinkEntryData*);
	void setDefaults();
	
public slots:
	void dirDialog();
	void urlComposer();
private:
	void save();
	void apply();
	bool inputValid();

	GofunLinkEntryData* data();
	
	QLineEdit* url;
	QToolButton* url_button;
};

#endif

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

#include "gofun_item_settings.h"
#include "gofun_cat_entry_data.h"

#ifndef GOFUN_CAT_SETTINGS
#define GOFUN_CAT_SETTINGS

class QLineEdit;
class QToolButton;
class QRadioButton;

///Settings-dialog for directory entries
class GofunDirectoryEntrySettings : public GofunDesktopEntrySettings
{
	Q_OBJECT
public:
	GofunDirectoryEntrySettings();
	void load(GofunDirectoryEntryData*);
	void setDefaults();

public slots:
	void backgroundDialog();
	void backgroundColorDialog();
private:
	void save();
	void apply();
	bool inputValid();

	GofunDirectoryEntryData* data();

	QLineEdit* background_image;
	QToolButton* background_image_button;
	QToolButton* background_color_button;
	QRadioButton* rb_bg_image;
	QRadioButton* rb_bg_color;
	QRadioButton* rb_bg_none;
};

#endif

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

#include <qpoint.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qtable.h>
 
#include "gofun_settings_dlg.h"
#include "gofun_data.h"

class GofunCatButton;
class GofunItem;

#ifndef GOFUN_ITEM_SETTINGS 
#define GOFUN_ITEM_SETTINGS

///Settings-dialog for items
class GofunItemSettings : public GofunSettingsDlg
{
	Q_OBJECT
public:
	GofunItemSettings();
	void setCategory(GofunCatButton*);
	virtual void load(GofunItem*);
	
public slots:
	virtual void iconDialog();
private:
	virtual GofunItemData* data();
	
protected:
	virtual void apply();
	virtual bool inputValid();
	virtual void save();

	QLineEdit* caption;
	QLineEdit* icon;
	QToolButton* icon_button;
	QLineEdit* comment;
	
	GofunCatButton* category;
	GofunItem* item;
};

#endif

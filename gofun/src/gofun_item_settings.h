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
	
	QString file;
public slots:
	void iconDialog();
	void dirDialog();
	void addEnvVar();
	void remEnvVar();
	void envItemEdit(QListViewItem*,const QPoint&,int);
	void userChkToggled(bool);
	void addParRow();
	void remParRow();
	void parValEditDialog();
	void commandEditor();
private:
	virtual void save();
	virtual void apply();
	bool inputValid();

	QLineEdit* command;
	QToolButton* command_button;
	QLineEdit* caption;
	QLineEdit* directory;
	QToolButton* dir_button;
	QLineEdit* icon;
	QToolButton* icon_button;
	GofunItem* item;
	GofunCatButton* category;
	QLineEdit* comment;
	QListView* envvars;
	QPushButton* envadd;
	QPushButton* envrem;
	QPushButton* envpre;
	QTable* tb_par;
	QPushButton* paradd;
	QPushButton* parrem;
	QCheckBox* terminal_chk;
	QCheckBox* user_chk;
	QComboBox* user_combo;
	QCheckBox* newx_chk;
};

#endif

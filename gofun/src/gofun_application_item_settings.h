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
 
#include "gofun_item_settings.h"

class GofunApplicationItem;

#ifndef GOFUN_APPLICATION_ITEM_SETTINGS
#define GOFUN_APPLICATION_ITEM_SETTINGS

class GofunInterpretedLineEdit : public QLineEdit
{
	Q_OBJECT
	
	public:
	GofunInterpretedLineEdit(const QString&, QWidget*);
	
	public slots:
	void setText(const QString&);
};

///Settings-dialog for items
class GofunApplicationItemSettings : public GofunItemSettings
{
	Q_OBJECT
public:
	GofunApplicationItemSettings();
	void load(GofunApplicationItem*);
	
public slots:
	void iconDialog();
	void dirDialog();
	void addEnvVar();
	void addEnvVar(const QString&, const QString&);
	void remEnvVar();
	void envItemEdit(QListViewItem*,const QPoint&,int);
	void envItemEdit();
	void envItemEditInterpreted(const QString&);
	void envPredefinedPopup();
	void envPredefinedPopupActivated(QListViewItem*);
	void userChkToggled(bool);
	void addParRow();
	void remParRow();
	void parEditDialog();
	void commandEditor();
	void editableEnvVar();
private:
	void save();
	void apply();
	void apply(GofunApplicationEntryData*);
	bool inputValid();
	
	GofunApplicationEntryData* data();

	QValueList<GofunParameterData> par_data;
	QLineEdit* command;
	QToolButton* command_button;
	QLineEdit* directory;
	QToolButton* dir_button;
	QListView* envvars;
	QPushButton* envadd;
	QPushButton* envrem;
	QPushButton* envpre;
	QPushButton* envedit;
	QTable* tb_par;
	QPushButton* paradd;
	QPushButton* parrem;
	QCheckBox* terminal_chk;
	QCheckBox* user_chk;
	QComboBox* user_combo;
	QCheckBox* newx_chk;

friend class GofunCommandEditor;
};

#endif


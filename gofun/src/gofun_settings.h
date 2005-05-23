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
 
#include "gofun_settings_dlg.h"

#ifndef GOFUN_SETTINGS
#define GOFUN_SETTINGS

class QComboBox;
class QLineEdit;
class QCheckBox;
class QRadioButton;
class QToolButton;
class GofunShortcutView;

///Main settings dialog
class GofunSettings : public GofunSettingsDlg
{
	Q_OBJECT
public:
	GofunSettings();
	virtual void load();
public slots:

private slots:
	void customColorDialog();
	void directoryDialog();
private:
	virtual void save();
	virtual void apply();
	virtual bool inputValid();
	virtual void accept();

	QLineEdit* terminal;
	QLineEdit* directory;
	QLineEdit* filemanager;
	QLineEdit* browser;
	QComboBox* styles;
	QRadioButton* col_system;
	QRadioButton* col_random;
	QRadioButton* col_custom;
	QToolButton* custom_col_bt;
	QCheckBox* save_main_geom;
	GofunShortcutView* shortcut_view;
	QString file;
};

#endif




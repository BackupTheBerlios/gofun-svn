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

#include <iostream>
 
#include <qdialog.h>
#include <qtabwidget.h>
#include <qlayout.h>

#ifndef GOFUN_SETTINGS_DLG
#define GOFUN_SETTINGS_DLG
 
class GofunSettingsDlg : public QDialog
{
	Q_OBJECT
public:
	GofunSettingsDlg();
public slots:
	virtual void accept();
	virtual void reject();
	void iconDialog();
	void dirDialog();
protected:
	QTabWidget* tabwidget;
private:
	virtual void save();
	virtual void apply();
	virtual bool inputValid();
};

#endif

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

#include <qwidget.h>

#ifndef GOFUN_APPLICATION_ITEM_SETTINGS_ENVIRONMENT
#define GOFUN_APPLICATION_ITEM_SETTINGS_ENVIRONMENT

class QListView;
class QPushButton;
class QListViewItem;
class GofunApplicationEntryData;

class GofunApplicationEntrySettingsEnvironment : public QWidget
{
	Q_OBJECT

	public:
	GofunApplicationEntrySettingsEnvironment(QWidget*);
	void apply(GofunApplicationEntryData*);
	void load(GofunApplicationEntryData*);

	private slots:
	void addEnvVar();
	void addEnvVar(const QString&, const QString&);
	void remEnvVar();
	void envItemEdit(QListViewItem*,const QPoint&,int);
	void envItemEdit();
	void envItemEditInterpreted(const QString&);
	void envPredefinedPopup();
	void envPredefinedPopupActivated(QListViewItem*);
	void editableEnvVar();

	private:
	QListView* envvars;
	QPushButton* envadd;
	QPushButton* envrem;
	QPushButton* envpre;
	QPushButton* envedit;
};

#endif

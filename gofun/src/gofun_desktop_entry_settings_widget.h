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

#include <qframe.h>
#include <qdialog.h>

class QToolButton;
class QLineEdit; 

#ifndef GOFUN_DESKTOP_ENTRY_SETTINGS_WIDGET
#define GOFUN_DESKTOP_ENTRY_SETTINGS_WIDGET

class GofunDesktopEntrySettingsWidget : public QFrame
{
	Q_OBJECT

	public:
	GofunDesktopEntrySettingsWidget(QWidget*);
	
	QToolButton* icon_button;
	QString icon;
	QLineEdit* caption;
	QLineEdit* comment;
	QString generic_name;
	void setIcon(const QString&);
	
	public slots:
	void showMoreSettings();
	void guessIcon(const QString&);
};

class GofunDesktopEntrySettingsMore : public QDialog
{
	public:
	GofunDesktopEntrySettingsMore(QWidget*);
	
	private:
	QLineEdit* generic_name;	
	
friend class GofunDesktopEntrySettingsWidget;
};


#endif

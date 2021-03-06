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

#include <qdialog.h>
#include <qtextedit.h>
#include <qlistview.h>

#ifndef GOFUN_COMMAND_EDITOR
#define GOFUN_COMMAND_EDITOR

class GofunApplicationEntrySettings;
class GofunListPopup;

///Command editor
class GofunCommandEditor : public QDialog
{
	Q_OBJECT
	public:
	GofunCommandEditor();
	void setCommand(const QString&);
	QString command();
	void setSettingsWidget(GofunApplicationEntrySettings*);
	
	private slots:
	void optionsDialog();
	void commandExpand();
	void commandCompletion(QListViewItem*);
	void test();
	void browseForDirectory();
	void browseForExecutable();
	void browseForFile();
	void importFromScript();
	void importFromEntry();
	void expandListKey(const QString&);
		
	private:
	QTextEdit* text;
	QString cmd;
	GofunListPopup* expand_list;
	
	GofunApplicationEntrySettings* settings_widget;
};

#endif


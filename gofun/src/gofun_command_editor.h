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

class GofunApplicationItem;

class GofunExecutableBrowser : public QDialog
{
	Q_OBJECT
	public:
	GofunExecutableBrowser();
	QString getExecutable();
	
};

class GofunCommandEditor : public QDialog
{
	Q_OBJECT
	public:
	GofunCommandEditor();
	void setCommand(const QString&);
	QString command();
	void setApplicationItem(GofunApplicationItem*);
	
	private slots:
	void commandExpand();
	void commandCompletion(QListViewItem*);
	void test();
	void browseForDirectory();
	void browseForExecutable();
	void browseForFile();
	
	private:
	QTextEdit* text;
	QString cmd;
	
	GofunApplicationItem* app_item;
};

#endif


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
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
 
#ifndef GOFUN_LIST_DIALOG
#define GOFUN_LIST_DIALOG

class GofunListDialog : public QDialog
{
	Q_OBJECT
public:
	GofunListDialog();
	QStringList returnList();
	void fillList(const QStringList&);

public slots:
	void add();
	void remove();
	void up();
	void down();
	void updateValue(const QString&);
	void updateEdit();
	
private:
	QListView* list;
	QLineEdit* edit;
	QPushButton* add_button;
	QPushButton* rem_button;
	QPushButton* apply_button;
	QPushButton* cancel_button;
	QPushButton* up_button;
	QPushButton* down_button;
};

#endif


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

#include <string>
#include <iostream>
#include <vector>
 
#include <qwidget.h>
#include <qpixmap.h>
#include <qiconview.h>
#include <qbuttongroup.h>
#include <qvbuttongroup.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qsound.h>
#include <qprocess.h>
#include <qmessagebox.h>

class GofunItem;
class GofunCatButton;
class GofunSettings;
class GofunItemSettings;
struct GofunCatData;

#ifndef GOFUN_WIDGET
#define GOFUN_WIDGET

enum
{
  PID_Execute,
  PID_Execute_in_terminal,
  PID_Execute_with_xinit,
  PID_Open_directory,
  PID_Edit,
  PID_Delete,
  PID_Costumized_start,
  PID_Add,
  PID_Add_Wizard,
  PID_Add_Category
};

///Is being the main window of GoFun
class GofunWidget : public QWidget
{
	Q_OBJECT
public:
	GofunWidget();
	void reloadData();
public slots:
	void changeCategory(int);
	void executeItem(QIconViewItem*,const QString& = QString::null);
	void rightClickedItem(QIconViewItem*,const QPoint&);
	void popupMenuItem(int);
	void popupMenuSpace(int);
	void popupCatAdd();
	void popupCAActivated(int);
	void openSettingsDlg();
	void addCategory();
	void showHelp();
	void showAbout();
	QVButtonGroup* categoryButtons() { return cats_bg; };
private:
	void costumizedStart(GofunItem*);
	void addEntry();
	void editEntry(GofunItem*);
	void deleteEntry(GofunItem*);
	void openDirectoryItem(GofunItem*);
	void loadData();
	void unloadData();

	QWidgetStack * view_ws;
	GofunCatButton* current_cat;
	QVButtonGroup* cats_bg;
	
friend class GofunCatSettings;
friend class GofunCatButton;
};

#endif
                      


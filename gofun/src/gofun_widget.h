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
struct GofunCatEntryData;

#ifndef GOFUN_WIDGET
#define GOFUN_WIDGET

enum
{
  PID_Add,
  PID_Add_Wizard,
  PID_Add_Category,
  PID_Add_Application,
  PID_Add_Device,
  PID_Add_Link
};

///Is being the main window of GoFun
class GofunWidget : public QWidget
{
	Q_OBJECT
public:
	GofunWidget(WFlags f = 0);
	void reloadData();
public slots:
	void changeCategory(int);
	void rightClickedItem(QIconViewItem*,const QPoint&);
	void popupMenuSpace(int);
	void popupCatAdd();
	void popupCAActivated(int);
	void openSettingsDlg();
	void addCategory();
	void showHelp();
	void showAbout();
	QVButtonGroup* categoryButtons() { return cats_bg; };
	void insertCategory(GofunCatButton*);
	void changeToTools();
	void performDefaultActionOnItem(QIconViewItem*);
	void toggleFullscreen();

public:
	static void applyColorSettings();
	static void applyStyleSettings();
	static QPalette system_palette;
	static QString system_style;
private:
	void costumizedStart(GofunItem*);
	void editEntry(GofunItem*);
	void deleteEntry(GofunItem*);
	void loadData();
	void unloadData();
	void connectCatIconview(GofunCatButton*);

	GofunCatButton* current_cat;
	GofunCatButton* tools_cat;
	QWidgetStack * view_ws;
	QVButtonGroup* cats_bg;
	
friend class GofunCatSettings;
friend class GofunCatButton;
};

#endif
                      


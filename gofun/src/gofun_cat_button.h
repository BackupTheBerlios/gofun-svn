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

#include <qtoolbutton.h>
#include <qpushbutton.h> 
#include <qiconview.h>

class GofunItem;
class GofunCatData;
class GofunIconView;

#ifndef GOFUN_CAT_BUTTON
#define GOFUN_CAT_BUTTON

enum {
	PID_MOVE_ITEM,
	PID_COPY_ITEM
	};

///Represents a directory and a category
class GofunCatButton : public QPushButton
{
	Q_OBJECT
public slots:
	void popupItemDnD(int);
	void catSettings();
	void resizeEvent(QResizeEvent*);
public:
	GofunCatButton(const QString&, QWidget*);
	//~GofunCatButton();
	void setIconView(GofunIconView*);
	void setData(GofunCatData*);
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);
	void save();
	
	GofunIconView* iconview;
	GofunCatData* data;
	GofunItem* current_item;
	QString directory;
	QToolButton* conf_button;
};

#endif

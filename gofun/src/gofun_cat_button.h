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

#include "gofun_desktop_object.h"

class GofunItem;
class GofunCatEntryData;
class GofunIconView;

#ifndef GOFUN_CAT_BUTTON
#define GOFUN_CAT_BUTTON

enum {
	PID_MOVE_ITEM = 5, //@TODO FIXME: PID collisions
	PID_COPY_ITEM,
	PID_ADD_ENTRY,
	PID_ADD_ENTRY_WIZARD,
	PID_SETTINGS,
	PID_DELETE
	};

///Represents a directory and a category
class GofunCatButton : public QPushButton , public GofunDesktopObject
{
	Q_OBJECT
public slots:
	void popupItemDnD(int);
	void catSettings();
	void resizeEvent(QResizeEvent*);
	void popupConfButton();
	void popupCBActivated(int);
public:
	GofunCatButton(const QString&, QWidget*);
	~GofunCatButton();
	void setIconView(GofunIconView*);
	void setData(GofunCatEntryData*);
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	void save();
	void loadIcon();
	void refreshBackground();
	virtual GofunCatEntryData* data() { return m_data; }
	const QPixmap* confButtonPixmap() { return conf_button->pixmap(); }
	void setupToolTip();
	void runNewItemWizard();
	bool isReadOnly() { return readonly;}
	void deleteEntry();
	void deleteEntryFile();
	void makeCurrent();

	GofunIconView* iconview;
	
private:
	GofunCatEntryData* m_data;
	GofunItem* current_item;
	QString directory;
	QToolButton* conf_button;
	
	bool readonly;	
};

#endif

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

#include <vector>
 
#include <qdialog.h>
#include <qthread.h>
#include <qiconview.h>

class QLabel;
class QIconView;
class QProgressBar;
class QGridLayout;
class GofunIconItem;
class GofunIconLoadThread;

#ifndef GOFUN_ICON_DIALOG
#define GOFUN_ICON_DIALOG

class GofunAdjustAbleIconView : public QIconView
{
	Q_OBJECT
	public:
	GofunAdjustAbleIconView(QWidget*);
	void adjustMe();
};

class GofunIconItemData;

class GofunIconDialog : public QDialog
{
	Q_OBJECT

	public:
	GofunIconDialog();
	~GofunIconDialog();
	QString selected();
	void setStartIcon(const QString&);
	void setStartDir(const QString&);
		
	private slots:
	void setSelectedIcon(QIconViewItem*);
	void browseForIcon();
	void updateFilterView(const QString&);
	
	private:
	void loadIcons();
	void customEvent(QCustomEvent*);
	
	QString start_dir;
	QLabel* icon_preview;
	QLabel* icon_file;
	GofunAdjustAbleIconView* filter_view;
	QProgressBar* load_progress;
	QGridLayout* grid;
	std::vector<GofunIconItem*> taken_icons;
	
	static std::vector<GofunIconItemData*> icon_pool;
	GofunIconLoadThread* icon_loader;
	
friend class GofunIconLoadThread;
};

class GofunIconLoadThread : public QThread
{
	public:
	GofunIconLoadThread(GofunIconDialog* id) { icon_dialog = id; };
	void setDead();
	virtual void run();
	
	private:
	GofunIconDialog* icon_dialog;
	bool dead;
};

struct GofunIconItemData
{
	GofunIconItemData(const QImage _pixmap, const QString& _text, const QString& _file) : pixmap(_pixmap), text(_text), file(_file) {}

	QImage pixmap;
	QString text;
	QString file;
};

class GofunIconItem : public QIconViewItem
{
	public:
	GofunIconItem(QIconView*, const QString& = 0, const QPixmap& = 0, const QString& = 0);
	
	QString file;
};

enum
{
	IconItemEventID = 5555,
	IconsLoadedEventID = 6666
};

class GofunIconItemDataEvent : public QCustomEvent
{
	public:
	GofunIconItemDataEvent(const QString _text, const QImage _pixmap, const QString _file)
	: QCustomEvent(IconItemEventID) , data(_pixmap,_text,_file) {}
	
	GofunIconItemData data;
};

class GofunIconsLoadedEvent : public QCustomEvent
{
	public:
	GofunIconsLoadedEvent() : QCustomEvent(IconsLoadedEventID) {}
};

#endif


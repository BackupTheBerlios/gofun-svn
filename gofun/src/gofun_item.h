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

#include <qfile.h>
#include <qprocess.h>
#include <qiconview.h>
#include <qtooltip.h>

#include "gofun_desktop_object.h"

class GofunDesktopEntryData;
class ExecuteOption;
class GofunIconView;
class GofunIconViewToolTip;
class QPopupMenu;
class GofunCatButton;

#ifndef GOFUN_ITEM
#define GOFUN_ITEM

enum
{
  PID_Edit,
  PID_Delete
};

///Items represent executeable Desktop Entries
class GofunItem : public QObject, public QIconViewItem , public GofunDesktopObject
{
	Q_OBJECT

public:
	GofunItem(GofunIconView*, const QString& = 0);
	virtual ~GofunItem();

	virtual void setData(GofunDesktopEntryData*);
	virtual void save();
	void deleteEntry();
	virtual void loadIcon();
	virtual void editEntry() {};
	virtual QPopupMenu* rightClickPopup(const QPoint&);
	
	static void createNewItem(GofunCatButton*);
	
	void setToolTipText(const QString);
	const QString getToolTipText(void) const { return(toolTipText); }
	
	virtual GofunDesktopEntryData* data() { return m_data; }
	virtual void performDefaultAction() {};
	bool isReadOnly() { return readonly; }
	
public slots:
	virtual void popupActivated(int);
protected:
	virtual void implementData();
private:
	QString saveProcArguments(QProcess*);
	void interpretExecString(QString&);
	void addSplittedProcArgument(QProcess*,const QString&);
	
	GofunDesktopEntryData* m_data;
	
	QString toolTipText;
	GofunIconViewToolTip* toolTip;
	bool readonly;
};


class GofunIconViewToolTip : public QToolTip
{
	private:
	QIconView* parent;
	static GofunItem* last_active;
	
	public:
	GofunIconViewToolTip(QIconView*, QToolTipGroup* = 0);
	virtual ~GofunIconViewToolTip(void);

	void maybeTip(const QPoint&);
};

#endif
      

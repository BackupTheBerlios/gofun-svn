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

#include "gofun_item.h"
#include "gofun_link_entry_data.h"
 
#ifndef GOFUN_LINK_ITEM
#define GOFUN_LINK_ITEM

class GofunURLComposer;

enum
{
	PID_OpenLink = PID_Delete + 1
};

class GofunLinkItem : public GofunItem
{
	Q_OBJECT

	public:
	GofunLinkItem(GofunIconView*, const QString& = 0);
	
	void setData(GofunDesktopEntryData*);
	QPopupMenu* rightClickPopup(const QPoint&);
	virtual void editEntry();
	void performDefaultAction();
	void open();
	
	static void createNewItem(GofunCatButton*);
	
	GofunLinkEntryData* data() { return m_data; }
	
	public slots:
	void popupActivated(int);
	
	private:
	GofunLinkEntryData* m_data;
	
friend class GofunURLComposer;
};

#endif


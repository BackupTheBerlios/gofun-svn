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
#include "gofun_data.h"
 
#ifndef GOFUN_FSDEVICE_ITEM
#define GOFUN_FSDEVICE_ITEM

enum
{
	PID_Mount = PID_Delete + 1,
	PID_Unmount,
	PID_Open
};

class GofunFSDeviceItem : public GofunItem
{
	Q_OBJECT;

	public:
	GofunFSDeviceItem(GofunIconView*, const QString& = 0);
	
	void setData(GofunItemData*);
	void save();
	QPopupMenu* rightClickPopup(const QPoint&);
	void editEntry();
	void performDefaultAction();
	bool isMounted();
	void mount();
	void unMount();
	void open();
	void loadIcon();
	
	static void createNewItem(GofunCatButton*);
	
	GofunFSDeviceItemData* data() { return m_data; }
	
	public slots:
	void popupActivated(int);
	
	private:
	GofunFSDeviceItemData* m_data;
};

#endif

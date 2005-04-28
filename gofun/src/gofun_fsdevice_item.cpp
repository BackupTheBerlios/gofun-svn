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

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
 
#include "gofun_fsdevice_item.h"
#include "gofun_fsdevice_item_settings.h"
#include "gofun_misc.h"
#include "gofun_settings_container.h"

GofunFSDeviceItem::GofunFSDeviceItem(GofunIconView* iconview, const QString& string) : GofunItem(iconview, string)
{
	m_data = new GofunFSDeviceEntryData();
}

//Open dialog for editing a Desktop Entry.
void GofunFSDeviceItem::editEntry()
{
	GofunFSDeviceItemSettings* settings_dlg = new GofunFSDeviceItemSettings();
	GofunWindowOperations::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,375,200);
	settings_dlg->setCaption(tr("Edit entry"));
	settings_dlg->load(this);
	settings_dlg->exec();
}

void GofunFSDeviceItem::setData(GofunDesktopEntryData* d)
{
	m_data = dynamic_cast<GofunFSDeviceEntryData*>(d);
	implementData();
}

void GofunFSDeviceItem::loadIcon()
{
	if(data()->isMounted())
		GofunItem::loadIcon();
	else
	{
		QPixmap px = GofunMisc::get_icon(data()->UnmountIcon,32,32);
		if(!px.isNull())
			setPixmap(px);
	}
}

QPopupMenu* GofunFSDeviceItem::rightClickPopup(const QPoint& pos)
{
	QPopupMenu* popup = GofunItem::rightClickPopup(pos);
		
	
	popup->insertItem(tr("Open"),PID_Open,0);
	popup->insertSeparator(1);
	if(data()->isMounted())
	popup->insertItem(tr("Unmount"),PID_Unmount,2);
	else
	popup->insertItem(tr("Mount"),PID_Mount,2);
	
	popup->popup(pos);
	
	return popup;
}

void GofunFSDeviceItem::popupActivated(int id)
{
	GofunItem::popupActivated(id);

	switch(id)
	{
		case PID_Open:
			data()->open(); break;
		case PID_Mount:
			mount(); break;
		case PID_Unmount:
			unMount(); break;
	}
}

void GofunFSDeviceItem::mount()
{
	if(data()->mount())
		loadIcon();
}

void GofunFSDeviceItem::unMount()
{
	if(data()->unMount())
		loadIcon();
}

void GofunFSDeviceItem::performDefaultAction()
{
	open();
}

void GofunFSDeviceItem::open()
{
	data()->open();
}

void GofunFSDeviceItem::createNewItem(GofunCatButton* cat)
{
	GofunFSDeviceItemSettings* settings_dlg = new GofunFSDeviceItemSettings();
	int height = 200;
	GofunWindowOperations::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->setCategory(cat);
	settings_dlg->setDefaults();
	settings_dlg->exec();
	delete settings_dlg;
}




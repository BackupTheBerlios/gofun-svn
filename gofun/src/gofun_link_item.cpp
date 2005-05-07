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
#include <qurl.h>
 
#include "gofun_link_item.h"
#include "gofun_link_item_settings.h"
#include "gofun_misc.h"
#include "gofun_settings.h"

GofunLinkItem::GofunLinkItem(QIconView* iconview, const QString& string) : GofunItem(iconview,string)
{
	m_data = new GofunLinkEntryData();
}

GofunLinkItem::GofunLinkItem(QIconView* iconview, const QString& string, GofunDesktopEntryData* data) : GofunItem(iconview,string)
{
	setData(data);
}

//Open dialog for editing a Desktop Entry.
void GofunLinkItem::editEntry()
{
	GofunLinkEntrySettings* settings_dlg = new GofunLinkEntrySettings();
	GofunWindowOperations::attachWindow(qApp->mainWidget(),settings_dlg,D_Above,D_Under,375,200);
	settings_dlg->setCaption(tr("Edit entry"));
	settings_dlg->load(data());
	settings_dlg->exec();
	implementData();
}

void GofunLinkItem::setData(GofunDesktopEntryData* d)
{
	m_data = dynamic_cast<GofunLinkEntryData*>(d);
	
	implementData();
}

QPopupMenu* GofunLinkItem::rightClickPopup(const QPoint& pos)
{
	QPopupMenu* popup = GofunItem::rightClickPopup(pos);
		
	popup->insertItem(tr("Open"),PID_OpenLink,0);

	popup->popup(pos);
	
	return popup;
}

void GofunLinkItem::popupActivated(int id)
{
	GofunItem::popupActivated(id);

	switch(id)
	{
		case PID_OpenLink:
			open(); break;
	}
}

void GofunLinkItem::open()
{
	data()->open();
}

void GofunLinkItem::performDefaultAction()
{
	open();
}

void GofunLinkItem::createNewItem(QIconView* iconview)
{
	GofunLinkEntryData* new_data = new GofunLinkEntryData;
	GofunLinkEntrySettings* settings_dlg = new GofunLinkEntrySettings();
	int height = 200;
	GofunWindowOperations::attachWindow(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->load(new_data);
	settings_dlg->setDefaults();
	if(settings_dlg->exec() == QDialog::Accepted)
	{
		GofunLinkItem* new_item = new GofunLinkItem(iconview,QString(""),new_data);
		new_item->implementData();
	}
	else
	{
		delete new_data;
		delete settings_dlg;
	}
}



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
 
#include "gofun_link_item.h"
#include "gofun_link_item_settings.h"
#include "gofun_misc.h"

GofunLinkItem::GofunLinkItem(GofunIconView* iconview, const QString& string) : GofunItem(iconview,string)
{
	m_data = new GofunLinkItemData();
}

//Open dialog for editing a Desktop Entry.
void GofunLinkItem::editEntry()
{
	GofunLinkItemSettings* settings_dlg = new GofunLinkItemSettings();
	GofunMisc::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,375,200);
	settings_dlg->setCaption(tr("Edit entry"));
	settings_dlg->load(this);
	settings_dlg->exec();
}

void GofunLinkItem::setData(GofunItemData* d)
{
	delete m_data;
	m_data = dynamic_cast<GofunLinkItemData*>(d);
	loadIcon();
	if(!data()->Comment.isEmpty())
		setToolTipText(data()->Comment);
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

	//if(GofunItem* item = (GofunItemdynamic_cast<GofunIconView*>(view_ws->visibleWidget()))->currentItem() )
	{
		switch(id)
		{
			case PID_OpenLink:
				open(); break;
		}
	}
}

void GofunLinkItem::save()
{
	GofunDesktopObject::save();
	GofunItem::save();

	QFile file( data()->File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "Type=Link\n";
		stream << "URL=" << data()->URL << "\n";
		stream << data()->Unknownkeys.join("\n") << "\n";
		file.close();
	}
}

void GofunLinkItem::open()
{
	QProcess proc(QString("dillo"));
	if(!data()->URL.isEmpty())
		proc.addArgument((GofunMisc::ext_filestring(data()->URL)).simplifyWhiteSpace());
	else
		proc.addArgument(QDir::homeDirPath());
	if(!proc.start())
	{
		std::cout<<QObject::tr("Execution of directory viewer failed. :(\n");
	}	
}

void GofunLinkItem::performDefaultAction()
{
	open();
}


void GofunLinkItem::createNewItem(GofunCatButton* cat)
{
	GofunLinkItemSettings* settings_dlg = new GofunLinkItemSettings();
	int height = 200;
	GofunMisc::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->setCategory(cat);
	settings_dlg->exec();
	delete settings_dlg;
}


/***************************************************************************
 *   Copyright (C) 2005 by Tobias Glaesser                                 *
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

#include <qwidget.h>
#include <qurl.h>

#include "gofun_link_entry_data.h"
#include "gofun_link_item.h"
#include "gofun_settings.h"
#include "gofun_data.h"
#include "gofun_iconview.h"
#include "gofun_misc.h"

GofunDesktopObject* GofunLinkEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	GofunLinkItem* item = new GofunLinkItem(dynamic_cast<GofunIconView*>(parent),Name);
	item->setData(this);
	return item;
}


bool GofunLinkEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;
		
	if(GofunDataLoader::parseLine("URL",line,URL))
		return true;
	return false;
}


void GofunLinkEntryData::open()
{
	QUrl url(GofunMisc::ext_filestring(URL));
	QProcess proc;
	if(url.protocol() == "file")
		proc.addArgument(GSC::get()->filemanager_cmd);
	else
	{
		proc.addArgument(GSC::get()->browser_cmd);
		url = URL;
	}
	if(!URL.isEmpty())
		proc.addArgument(url.toString());
	else
		proc.addArgument(QDir::homeDirPath());
	if(!proc.start())
	{
		std::cout<<QObject::tr("Execution of directory viewer failed. :(\n");
	}
}

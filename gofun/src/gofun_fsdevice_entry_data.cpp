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

#include "gofun_fsdevice_entry_data.h"
#include "gofun_fsdevice_item.h"
#include "gofun_iconview.h"
#include "gofun_data.h"

GofunDesktopObject* GofunFSDeviceEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	GofunFSDeviceItem* item = new GofunFSDeviceItem(dynamic_cast<GofunIconView*>(parent),Name);
	item->setData(this);
	return item;
}

bool GofunFSDeviceEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;
		
	if(GofunDataLoader::parseLine("Dev",line,Device)
	|| GofunDataLoader::parseLine("FSType",line,FSType)
	|| GofunDataLoader::parseLine("MountPoint",line,MountPoint)
	|| GofunDataLoader::parseLine("ReadOnly",line,ReadOnly)
	|| GofunDataLoader::parseLine("UnmountIcon",line,UnmountIcon))
		return true;
	return false;
}

GofunFSDeviceEntryData * GofunFSDeviceEntryData::makeCopy( )
{
	GofunFSDeviceEntryData* copy = new GofunFSDeviceEntryData;
	*copy = *this;
	return copy;
}


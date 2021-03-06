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
 
#include <qstring.h>

#include "gofun_desktop_entry_data.h"
 
#ifndef GOFUN_FSDEVICE_ENTRY_DATA
#define GOFUN_FSDEVICE_ENTRY_DATA

class mntent;

struct GofunFSDeviceEntryData : public GofunDesktopEntryData
{
	QString Device;
	QString FSType;
	QString MountPoint;
	QString ReadOnly;
	QString UnmountIcon;
	
	bool parseLine(const QString&);
	void save();
	bool isMounted();
	bool mount();
	bool unMount();
	bool open();
	QString getMountPoint();
	QString getDevice();
	mntent* getMntEnt();	

	QString noLinkGuarantuee(const QString&);
	
	virtual GofunFSDeviceEntryData* makeCopy();
};

#endif

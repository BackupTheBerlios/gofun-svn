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
 
#include "gofun_desktop_object.h"
#include "gofun_data.h"

void GofunDesktopObject::save()
{
	GofunDesktopEntryData* pdata = reinterpret_cast<GofunDesktopEntryData*>(data());
	QFile file( pdata->File );
	if ( file.open( IO_WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << "[Desktop Entry]\n";
		stream << "Version=0.9.4\n";
		stream << "Encoding=UTF-8\n";
		if(!data()->Icon.isEmpty())
			stream << "Icon=" << pdata->Icon << "\n";
		pdata->Name.desktopEntryPrint("Name",stream);
		pdata->GenericName.desktopEntryPrint("GenericName",stream);
		pdata->Comment.desktopEntryPrint("Comment",stream);
		file.close();
	}
}


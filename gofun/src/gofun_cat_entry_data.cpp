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

#include <qfile.h>
 
#include "gofun_cat_entry_data.h"
#include "gofun_data.h"

/*GofunDesktopObject* GofunCatEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	GofunCatButton* item = new GofunCatButton(Name,parent);
	item->setData(this);
	return item;
}*/

bool GofunCatEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;
		
	if(GofunDataLoader::parseLine("X-GoFun-Color",line,X_GoFun_Color)
	|| GofunDataLoader::parseLine("X-GoFun-Background",line,X_GoFun_Background))
		return true;
	return false;
}

GofunCatEntryData* GofunCatEntryData::makeCopy()
{
	GofunCatEntryData* copy = new GofunCatEntryData;
	*copy = *this;
	return copy;
}

void GofunCatEntryData::save()
{
	GofunDesktopEntryData::save();

	QFile file( File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "Type=Directory\n";
		if(!X_GoFun_Background.isEmpty())
			stream << "X-GoFun-Background=" << X_GoFun_Background << "\n";
		file.close();
	}
}




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
#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>

#include "gofun_data.h"
#include "gofun_desktop_entry_data.h"
#include "gofun_misc.h"

/*GofunDesktopObject* GofunDesktopEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	GofunItem* item = new GofunItem(dynamic_cast<GofunIconView*>(parent),Name);
	item->setData(this);
	return item;
}*/

bool GofunDesktopEntryData::parseLine(const QString& line)
{
	if(GofunDataLoader::parseLine("Name",line,Name)
	|| GofunDataLoader::parseLine("GenericName",line,GenericName)
	|| GofunDataLoader::parseLine("Comment",line,Comment)
	|| GofunDataLoader::parseLine("Icon",line,Icon)
	|| GofunDataLoader::parseLine("Version",line,Version)
	|| GofunDataLoader::parseLine("Type",line,Type)
	|| GofunDataLoader::parseLine("Encoding",line,Encoding)
	|| GofunDataLoader::parseLine("Hidden",line,Hidden)
	|| line.find("[Desktop Entry]") == 0)
		return true;
	return false;
}

GofunDesktopEntryData * GofunDesktopEntryData::makeCopy( )
{
	GofunDesktopEntryData* copy = new GofunDesktopEntryData;
	*copy = *this;
	return copy;
}

void GofunDesktopEntryData::save( )
{
	if(File.isEmpty())
	{
		generateFileName();
		if(!QFileInfo(File).dir().exists())
			GofunMisc::makeDir(QFileInfo(File).dirPath());
	}

	QFile file( File );
	if ( file.open( IO_WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << "[Desktop Entry]\n";
		stream << "Version=0.9.4\n";
		stream << "Encoding=UTF-8\n";
		if(!Icon.isEmpty())
			stream << "Icon=" << Icon << "\n";
		Name.desktopEntryPrint("Name",stream);
		GenericName.desktopEntryPrint("GenericName",stream);
		Comment.desktopEntryPrint("Comment",stream);
		file.close();
	}
}

bool GofunDesktopEntryData::isReadOnly()
{
	if(!File.isEmpty() && QFile::exists(File)  && !QFileInfo(File).isWritable())
		return true;
	else
		return false;
}

void GofunDesktopEntryData::generateFileName()
{
	File = TopDirectory + "/" + Name + ".desktop";
}


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
#include <qstringlist.h>

#include "gofun_locale_string.h"
#include "gofun_desktop_object.h"
 
#ifndef GOFUN_DESKTOP_ENTRY_DATA
#define GOFUN_DESKTOP_ENTRY_DATA

///Base data-type for Desktop Entries
struct GofunDesktopEntryData
{
	QString File;
	GofunLocaleString Name;
	GofunLocaleString GenericName;
	GofunLocaleString Comment;
	QString Icon;
	QString Encoding;
	QString Version;
	QString Type;
	QStringList Unknownkeys;
	QString Hidden;
	
	virtual bool parseLine(const QString&);
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
	virtual GofunDesktopEntryData* makeCopy();
};

#endif

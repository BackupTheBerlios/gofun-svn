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

#include <vector>
#include <map>
 
#include <qstring.h>
#include <qstringlist.h>

#include "gofun_locale_string.h"
#include "gofun_cat_entry_data.h"

class QWidget;
class QProcess;

#ifndef GOFUN_DATA
#define GOFUN_DATA

///Loads Desktop-Entry data
struct GofunDataLoader
{
	static std::vector<GofunCatEntryData>* getData();
	static GofunDesktopEntryData* parseGofunFile(const QString& file);
public:
	static bool parseLine(const QString&,const QString&,QString&);
	static bool parseLine(const QString&,const QString&,std::vector<QString>&);
	static bool parseLine(const QString&,const QString&,GofunLocaleString&);
	static QString getValue(QString line);
	static QString getKey(QString line);
private:
	static std::vector<GofunDesktopEntryData*>* parseCatdir(const QString& catdir);
	static GofunCatEntryData* parseCatInfo(const QString& file);
	static QStringList loadFileData(const QString& _file);
	static QString getLocale(const QString& locale);
};

#endif

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

#include <map>
 
#include <qstring.h>
 
#ifndef GOFUN_LOCALE_STRING
#define GOFUN_LOCALE_STRING

struct GofunLocale
{
	GofunLocale();
	GofunLocale(const QString&);
	void setLocale(const QString&);
	operator const QString();

	QString lang;
	QString country;
	QString modifier;
};

class GofunLocaleString : public QString
{
	public:
	GofunLocaleString();
	GofunLocaleString(const QString&);
	void add(const QString&,const QString&);
	QString desktopEntryPrint(const QString&);
	
	GofunLocaleString& operator=(const QString&);
	
	private:
	bool isBetterMatch(const GofunLocale&);
	
	std::map<QString,QString> locale_strings;
	std::map<QString,QString>::iterator best_match;
	
	static GofunLocale system_locale;
	static GofunLocale best_match_locale;
};

#endif

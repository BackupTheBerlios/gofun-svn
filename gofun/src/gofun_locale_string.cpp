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

#include <qstringlist.h>
 
#include "gofun_locale_string.h"

GofunLocale GofunLocaleString::system_locale(getenv("LC_MESSAGES"));
GofunLocale GofunLocaleString::best_match_locale;

GofunLocale::GofunLocale(const QString& locale)
{
	setLocale(locale);
}

GofunLocale::GofunLocale()
{
}

void GofunLocale::setLocale(const QString& locale)
{
	if(locale.find('_') != -1)
	{
		lang = QStringList::split("_",locale)[0];
		country = QStringList::split("_",locale)[1];
	}
	if(locale.find('@') != -1)
	{
		if(lang.isEmpty())
			lang = QStringList::split("@",locale)[0];
		modifier = QStringList::split("@",locale)[1];
	}
	if(lang.isEmpty())
		lang = locale;
}

GofunLocale::operator const QString()
{
	return lang;
}

GofunLocaleString::GofunLocaleString() : QString()
{
	best_match = 0;
}

GofunLocaleString::GofunLocaleString(const QString& string) : QString(string)
{
	best_match = 0;
}

void GofunLocaleString::add(const QString& locale, const QString& string)
{
	locale_strings[locale] = QString::fromUtf8(string);
	
	if(best_match == 0 || isBetterMatch(locale))
	{
		best_match = locale_strings.find(locale);
		best_match_locale.setLocale(locale);
		operator=((*best_match).second);
	}
}

bool GofunLocaleString::isBetterMatch(const GofunLocale& locale)
{
	//(*best_match).first == 
	if(locale.lang != system_locale.lang)
		return false;
	if((best_match_locale.lang != system_locale.lang))
		return true;
	if((best_match_locale.country != system_locale.country) && !locale.country.isEmpty() && (locale.country == system_locale.country))
		return true;
	if((best_match_locale.modifier != system_locale.modifier) && !locale.modifier.isEmpty() && (locale.modifier == system_locale.modifier))
		return true;
		
}

QString GofunLocaleString::desktopEntryPrint(const QString& key)
{
	QString tmp;
	for(std::map<QString,QString>::iterator it = locale_strings.begin(); it != locale_strings.end(); ++it)
	{
		if(!(*it).first.isEmpty())
		tmp += key + "[" + (*it).first + "]=" + QString((*it).second.utf8()) + "\n";
		else
		tmp += key + "=" + QString((*it).second.utf8()) + "\n";
	}
	return tmp;
}

GofunLocaleString& GofunLocaleString::operator=( const QString& string)
{
	QString::operator=(string);
	return *this;
}


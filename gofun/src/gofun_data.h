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
 
#include <qstring.h>
#include <qstringlist.h>
 
#ifndef GOFUN_DATA
#define GOFUN_DATA

struct GofunDesktopEntryData
{
	QString Name;
	QString Comment;
	QString Unknownkeys;
};

struct GofunItemData : public GofunDesktopEntryData
{
	QString File;
	QString Exec;
	QString Path;
	QString Icon;
	QString Terminal;
	std::vector<QString> X_GoFun_Env;
	QString X_GoFun_User;
	QString Hidden;
};

struct GofunCatData : public GofunDesktopEntryData
{
	QString Version;
	QString Type;
	QString Catdir;
	QString Background;
	QString Encoding;
	QString X_GoFun_Background;
	QString X_GoFun_Color;
	std::vector<GofunItemData>* ItemData;
};

struct ExecuteOption : public GofunItemData
{
	QString Exec;
	QString terminal;
	QString xinit;
};

struct GofunDataLoader
{
	static std::vector<GofunCatData>* getData();
private:
	static std::vector<GofunItemData>* parse_catdir(const QString& catdir);
	static GofunItemData* parse_gofun_file(const QString& file);
	static GofunCatData* parse_cat_info(const QString& file);
	static QStringList load_file_data(const QString& _file);
	static QString get_value(QString line);
};

#endif

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

class QWidget;
class GofunDesktopObject;

#ifndef GOFUN_DATA
#define GOFUN_DATA

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
};

///Data-type describing parameters in a Desktop Entry
struct GofunParameterData
{
	QString Flag;
	QStringList Values;
	QString Default_Value;
	QString Prompt;
	GofunLocaleString Comment;
};

struct GofunApplicationEntryData : public GofunDesktopEntryData
{

	QString Exec;
	QString Path;
	QString Terminal;
	std::vector<QString> X_GoFun_Env;
	QString X_GoFun_User;
	QString X_GoFun_NewX;
	std::map<int,GofunParameterData> X_GoFun_Parameter;
	
	bool parseLine(const QString&);
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

struct GofunFSDeviceEntryData : public GofunDesktopEntryData
{
	QString Device;
	QString FSType;
	QString MountPoint;
	QString ReadOnly;
	QString UnmountIcon;
	
	bool parseLine(const QString&);
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

struct GofunLinkEntryData : public GofunDesktopEntryData
{
	QString URL;
	
	bool parseLine(const QString&);
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

///Data-type for Desktop Entries, that represent a category
/** Contains a std::vector with the GofunDesktopEntryData, that is present
    in this category. **/
struct GofunCatEntryData : public GofunDesktopEntryData
{
	QString Catdir;
	QString Background;
	QString X_GoFun_Background;
	QString X_GoFun_Color;
	std::vector<GofunDesktopEntryData*>* ItemData;
	
	bool parseLine(const QString&);
		
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

///Data-type that can contain special start-options
struct ExecuteOption : public GofunDesktopEntryData
{
	QString Exec;
	QString terminal;
	QString xinit;
};

///Loads Desktop-Entry data
struct GofunDataLoader
{
	static std::vector<GofunCatEntryData>* getData();
public:
	static bool parseLine(const QString&,const QString&,QString&);
	static bool parseLine(const QString&,const QString&,std::vector<QString>&);
	static bool parseLine(const QString&,const QString&,GofunLocaleString&);
	static QString getValue(QString line);
	static QString getKey(QString line);
private:
	static std::vector<GofunDesktopEntryData*>* parseCatdir(const QString& catdir);
	static GofunDesktopEntryData* parseGofunFile(const QString& file);
	static GofunCatEntryData* parseCatInfo(const QString& file);
	static QStringList loadFileData(const QString& _file);
	static QString getLocale(const QString& locale);
};

struct GofunSettingsData
{
	public:
		//Loads the settings
		GofunSettingsData();
		//Saves the settings
		~GofunSettingsData();
	private:
		QString data_directory;
		QString terminal_cmd;	
	
};

#endif

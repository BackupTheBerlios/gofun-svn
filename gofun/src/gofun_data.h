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

class QWidget;
class GofunDesktopObject;

#ifndef GOFUN_DATA
#define GOFUN_DATA

///Base data-type for Desktop Entries
struct GofunDesktopEntryData
{
	QString File;
	QString Name;
	QString Comment;
	QString Icon;
	QString Encoding;
	QString Version;
	QString Type;
	QStringList Unknownkeys;
	QString Hidden;
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent) = 0;
};

///Data-type describing parameters in a Desktop Entry
struct GofunParameterData
{
	QString Flag;
	QStringList Values;
	QString Default_Value;
	QString Prompt;
	QString Comment;
};

///Data-type for Desktop Entries, that can be executed
struct GofunItemData : public GofunDesktopEntryData
{
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

struct GofunApplicationItemData : public GofunItemData
{

	QString Exec;
	QString Path;
	QString Terminal;
	std::vector<QString> X_GoFun_Env;
	QString X_GoFun_User;
	QString X_GoFun_NewX;
	std::map<int,GofunParameterData> X_GoFun_Parameter;
	
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

struct GofunFSDeviceItemData : public GofunItemData
{
	QString Device;
	QString FSType;
	QString MountPoint;
	QString ReadOnly;
	QString UnmountIcon;
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

struct GofunLinkItemData : public GofunItemData
{
	QString URL;
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

///Data-type for Desktop Entries, that represent a category
/** Contains a std::vector with the GofunItemData, that is present
    in this category. **/
struct GofunCatData : public GofunDesktopEntryData
{
	QString Catdir;
	QString Background;
	QString X_GoFun_Background;
	QString X_GoFun_Color;
	std::vector<GofunItemData*>* ItemData;
	
	virtual GofunDesktopObject* GofunDesktopObjectFactory(QWidget* parent);
};

///Data-type that can contain special start-options
struct ExecuteOption : public GofunItemData
{
	QString Exec;
	QString terminal;
	QString xinit;
};

///Loads Desktop-Entry data
struct GofunDataLoader
{
	static std::vector<GofunCatData>* getData();
private:
	static std::vector<GofunItemData*>* parse_catdir(const QString& catdir);
	static bool parse_desktop_file_line(GofunDesktopEntryData*, const QString&);
	static bool parse_application_file_line(GofunApplicationItemData*, const QString&);
	static bool parse_link_file_line(GofunLinkItemData*, const QString&);
	static bool parse_fsdevice_file_line(GofunFSDeviceItemData*, const QString&);
	static GofunItemData* parse_gofun_file(const QString& file);
	static GofunCatData* parse_cat_info(const QString& file);
	static QStringList load_file_data(const QString& _file);
	static QString get_value(QString line);
	static QString get_key(QString line);
	static bool parse_line(const QString&,const QString&,QString&);
	static bool parse_line(const QString&,const QString&,std::vector<QString>&);
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

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

#include <glob.h>
 
#include <qfile.h>
#include <qregexp.h>
#include <qwidget.h>
 
#include "gofun_data.h"
#include "gofun_settings.h"
#include "gofun_item.h"
#include "gofun_application_item.h"
#include "gofun_fsdevice_item.h"
#include "gofun_link_item.h"
#include "gofun_cat_button.h"
#include "gofun_iconview.h"

GofunDesktopObject* GofunItemData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunApplicationItemData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunApplicationItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunFSDeviceItemData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunFSDeviceItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunLinkItemData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunLinkItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunCatData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunCatButton(Name,parent);
}

QString GofunDataLoader::get_value(QString line)
{
	line.remove(0,line.find("=")+1);
	return line;
}

QString GofunDataLoader::get_key(QString line)
{
	line.remove(line.find("="),line.length());
	return line;
}

QStringList GofunDataLoader::load_file_data(const QString& _file)
{
	QStringList lines;
	QFile file( _file );
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
        	QString line;
        	int i = 1;
		while ( !stream.atEnd() )
		{
			line = stream.readLine(); // line of text excluding '\n'
			lines += line;
        	}
        	file.close();
    	}
	return lines;
}

bool GofunDataLoader::parse_line(const QString& key, const QString& line, QString& target)
{
	if(line.find(key) == 0)
	{
		target = get_value(line);
		return true;
	}
	return false;
}

bool GofunDataLoader::parse_line(const QString& key, const QString& line, std::vector<QString>& target)
{
	if(line.find(key) == 0)
	{
		QStringList qsl = QStringList::split(QRegExp(";"),get_value(line));
		for(QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it)
		{
			target.push_back(QString((*it)).replace("\\;",";"));
		}
		return true;
	}
	return false;
}

GofunCatData* GofunDataLoader::parse_cat_info(const QString& file)
{
	QStringList catdata = load_file_data(file);
			
	GofunCatData* cdata = new GofunCatData;
	cdata->Catdir = QString(file).remove(QRegExp("\\.directory$"));
			
	for(QStringList::Iterator it = catdata.begin(); it != catdata.end(); ++it)
	{
		if(parse_desktop_file_line(cdata,(*it)))
			0;
		if(!(parse_line("X-GoFun-Color",(*it),cdata->X_GoFun_Color)
		|| parse_line("X-GoFun-Background",(*it),cdata->X_GoFun_Background)))
			cdata->Unknownkeys += (*it);
	}
		
	cdata->File = file;
	
	return cdata;	
}

//TODO: implement general parser
bool GofunDataLoader::parse_desktop_file_line(GofunDesktopEntryData* ddata, const QString& line)
{
	if(parse_line("Name",line,ddata->Name)
	|| parse_line("Comment",line,ddata->Comment)
	|| parse_line("Icon",line,ddata->Icon)
	|| parse_line("Version",line,ddata->Version)
	|| parse_line("Type",line,ddata->Type)
	|| parse_line("Encoding",line,ddata->Encoding)
	|| parse_line("Hidden",line,ddata->Hidden)
	|| line.find("[Desktop Entry]") == 0)
		return true;
	return false;
}

bool GofunDataLoader::parse_link_file_line(GofunLinkItemData* data, const QString& line)
{
	if(parse_line("URL",line,data->URL))
		return true;
	return false;
}

bool GofunDataLoader::parse_fsdevice_file_line(GofunFSDeviceItemData* data, const QString& line)
{
	if(parse_line("Dev",line,data->Device)
	|| parse_line("FSType",line,data->FSType)
	|| parse_line("MountPoint",line,data->MountPoint)
	|| parse_line("ReadOnly",line,data->ReadOnly)
	|| parse_line("UnmountIcon",line,data->UnmountIcon))
		return true;
	return false;
}

bool GofunDataLoader::parse_application_file_line(GofunApplicationItemData* data, const QString& line)
{	
	if(parse_line("Exec",line,data->Exec)
	|| parse_line("Path",line,data->Path)
	|| parse_line("Terminal",line,data->Terminal)
	|| parse_line("X-GoFun-NewX",line,data->X_GoFun_NewX)
	|| parse_line("X-GoFun-User",line,data->X_GoFun_User)
	|| parse_line("X-GoFun-Env",line,data->X_GoFun_Env))
		0;
	else if(line.find("X-GoFun-Parameter") == 0)
	{
		if(line.find("X-GoFun-Parameter-Flag") == 0)
		{
			data->X_GoFun_Parameter[get_key(line).remove(0,QString("X-GoFun-Parameter-Flag-").length()).toInt()].Flag = get_value(line);
		}
		else if(line.find("X-GoFun-Parameter-Values") == 0)
		{
			data->X_GoFun_Parameter[get_key(line).remove(0,QString("X-GoFun-Parameter-Values-").length()).toInt()].Values = QStringList::split(';',get_value(line));
		}
		else if(line.find("X-GoFun-Parameter-Default") == 0)
		{
			data->X_GoFun_Parameter[get_key(line).remove(0,QString("X-GoFun-Parameter-Default-").length()).toInt()].Default_Value = get_value(line);
		}
		else if(line.find("X-GoFun-Parameter-Prompt") == 0)
		{
			data->X_GoFun_Parameter[get_key(line).remove(0,QString("X-GoFun-Parameter-Prompt-").length()).toInt()].Prompt = get_value(line);
		}
		else if(line.find("X-GoFun-Parameter-Comment") == 0)
		{
			data->X_GoFun_Parameter[get_key(line).remove(0,QString("X-GoFun-Parameter-Comment-").length()).toInt()].Comment = get_value(line);
		}
		else
			return false;
	}
	else
			return false;
	return true;
}

GofunItemData* GofunDataLoader::parse_gofun_file(const QString& file)
{
	QStringList gofundata = load_file_data(file);

	GofunItemData* idata = new GofunItemData;
	bool found_type = false;

	for(QStringList::Iterator it = gofundata.begin(); it != gofundata.end(); ++it)
	{
		/*if (parse_desktop_file_line(idata,(*it))
		|| parse_application_file_line(static_cast<GofunApplicationItemData*>(idata),(*it))
		|| parse_line("Hidden",(*it),idata->Hidden))
			0;*/
			
		if(parse_desktop_file_line(idata,(*it)))
		{
			if(!(found_type == true || idata->Type.isEmpty()))
			{
			if(idata->Type == "Application")
			{
				GofunItemData* tmp = new GofunApplicationItemData;
				*tmp = *idata;
				delete idata;
				idata = tmp;
				found_type = true;
				for(QStringList::Iterator sit = idata->Unknownkeys.begin(); sit != idata->Unknownkeys.end(); ++sit)
				{
					if(parse_application_file_line(static_cast<GofunApplicationItemData*>(idata),(*sit)))
						remove((*sit));
				}
			}
			else if(idata->Type == "Link")
			{
				GofunItemData* tmp = new GofunLinkItemData;
				*tmp = *idata;
				delete idata;
				idata = tmp;
				found_type = true;
				for(QStringList::Iterator sit = idata->Unknownkeys.begin(); sit != idata->Unknownkeys.end(); ++sit)
				{
					if(parse_link_file_line(static_cast<GofunLinkItemData*>(idata),(*sit)))
						remove((*sit));
				}
			}
			else if(idata->Type == "FSDevice")
			{
				GofunItemData* tmp = new GofunFSDeviceItemData;
				*tmp = *idata;
				delete idata;
				idata = tmp;
				found_type = true;
				for(QStringList::Iterator sit = idata->Unknownkeys.begin(); sit != idata->Unknownkeys.end(); ++sit)
				{
					if(parse_fsdevice_file_line(static_cast<GofunFSDeviceItemData*>(idata),(*sit)))
						remove((*sit));
				}
			}
			}
		}
		else if(!idata->Type.isEmpty())
		{
			if(idata->Type == "Application")
				parse_application_file_line(static_cast<GofunApplicationItemData*>(idata),(*it));
			else if(idata->Type == "Link")
				parse_link_file_line(static_cast<GofunLinkItemData*>(idata),(*it));
			else if(idata->Type == "FSDevice")
				parse_fsdevice_file_line(static_cast<GofunFSDeviceItemData*>(idata),(*it));
		}
		else
		{
			idata->Unknownkeys += (*it);
		}
	}
	
	idata->File = file;
	
	return idata;
}

std::vector<GofunItemData*>* GofunDataLoader::parse_catdir(const QString& catdir)
{
	glob_t files;
	glob((catdir + "*.desktop"),0,0,&files);
	
	std::vector<GofunItemData*>* VoID = new std::vector<GofunItemData*>;
	
	for(int i = 0; i <files.gl_pathc; ++i)
	{
		if((QString(files.gl_pathv[i]) == catdir + ".directory")  || (QString(files.gl_pathv[i]).find(".*~$")) != -1)
		{
			continue;
		}
		GofunItemData* p_idata = parse_gofun_file(files.gl_pathv[i]);
		VoID->push_back(p_idata);
	}
	globfree(&files);
	
	return VoID;
}

std::vector<GofunCatData>* GofunDataLoader::getData()
{
	glob_t categories; //default data dir 
	glob((GSC::get()->gofun_dir + "/*/.directory").ascii(),0,0,&categories);

	std::vector<GofunCatData>* VaCD = new std::vector<GofunCatData>;
	
	GofunCatData* p_cdata = parse_cat_info("tools/.directory");
	VaCD->push_back(*p_cdata);
	delete p_cdata;
	
	for(int i = 0; i < categories.gl_pathc; ++i)
	{
		GofunCatData* p_cdata = parse_cat_info(categories.gl_pathv[i]);
		VaCD->push_back(*p_cdata);
		delete p_cdata;
	}
	globfree(&categories);
	
	for(std::vector<GofunCatData>::iterator it = VaCD->begin(); it != VaCD->end(); ++it)
	{
		std::vector<GofunItemData*>* _gofun = parse_catdir((*it).Catdir);
		(*it).ItemData = _gofun;
	}
 
	return VaCD;
}

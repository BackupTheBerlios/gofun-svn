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

GofunDesktopObject* GofunDesktopEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunApplicationEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunApplicationItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunFSDeviceEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunFSDeviceItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunLinkEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunLinkItem(dynamic_cast<GofunIconView*>(parent),Name);
}

GofunDesktopObject* GofunCatEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	return new GofunCatButton(Name,parent);
}

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

bool GofunApplicationEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;

	if(GofunDataLoader::parseLine("Exec",line,Exec)
	|| GofunDataLoader::parseLine("TryExec",line,TryExec)
	|| GofunDataLoader::parseLine("Path",line,Path)
	|| GofunDataLoader::parseLine("Terminal",line,Terminal)
	|| GofunDataLoader::parseLine("X-GoFun-NewX",line,X_GoFun_NewX)
	|| GofunDataLoader::parseLine("X-GoFun-User",line,X_GoFun_User)
	|| GofunDataLoader::parseLine("X-GoFun-Env",line,X_GoFun_Env))
		0;
	else if(line.find("X-GoFun-Parameter") == 0)
	{
		if(line.find("X-GoFun-Parameter-Flag") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Flag-").length()).toInt()].Flag = GofunDataLoader::getValue(line);
		}
		else if(line.find("X-GoFun-Parameter-Values") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Values-").length()).toInt()].Values = QStringList::split(';',GofunDataLoader::getValue(line));
		}
		else if(line.find("X-GoFun-Parameter-Default") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Default-").length()).toInt()].Default_Value = GofunDataLoader::getValue(line);
		}
		else if(line.find("X-GoFun-Parameter-Prompt") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Prompt-").length()).toInt()].Prompt = GofunDataLoader::getValue(line);
		}
		else if(line.find("X-GoFun-Parameter-Comment") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Comment-").length()).toInt()].Comment = GofunDataLoader::getValue(line);
		}
		else
			return false;
	}
	else
			return false;
	return true;
}

bool GofunLinkEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;
		
	if(GofunDataLoader::parseLine("URL",line,URL))
		return true;
	return false;
}

bool GofunFSDeviceEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;
		
	if(GofunDataLoader::parseLine("Dev",line,Device)
	|| GofunDataLoader::parseLine("FSType",line,FSType)
	|| GofunDataLoader::parseLine("MountPoint",line,MountPoint)
	|| GofunDataLoader::parseLine("ReadOnly",line,ReadOnly)
	|| GofunDataLoader::parseLine("UnmountIcon",line,UnmountIcon))
		return true;
	return false;
}

bool GofunCatEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;
		
	if(GofunDataLoader::parseLine("X-GoFun-Color",line,X_GoFun_Color)
	|| GofunDataLoader::parseLine("X-GoFun-Background",line,X_GoFun_Background))
		return true;
	return false;
}
 
QString GofunDataLoader::getValue(QString line)
{
	line.remove(0,line.find("=")+1);
	return line;
}

QString GofunDataLoader::getLocale(const QString& line)
{
	QString tmp = getKey(line);
	if(tmp.find("[") != -1)
	{
		tmp.remove(0,tmp.find("[")+1);
		tmp.remove(tmp.find("]"),1);
		return tmp;
	}
	else
		return "";
}

QString GofunDataLoader::getKey(QString line)
{
	line.remove(line.find("="),line.length());
	return line;
}

QStringList GofunDataLoader::loadFileData(const QString& _file)
{
	QStringList lines;
	QFile file( _file );
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		stream.setEncoding(QTextStream::Latin1);
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

bool GofunDataLoader::parseLine(const QString& key, const QString& line, QString& target)
{
	if(line.find(key) == 0)
	{
		target = getValue(line);
		return true;
	}
	return false;
}

bool GofunDataLoader::parseLine(const QString& key, const QString& line, std::vector<QString>& target)
{
	if(line.find(key) == 0)
	{
		QStringList qsl = QStringList::split(QRegExp(";"),getValue(line));
		for(QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it)
		{
			target.push_back(QString((*it)).replace("\\;",";"));
		}
		return true;
	}
	return false;
}

bool GofunDataLoader::parseLine(const QString& key, const QString& line, GofunLocaleString& target)
{
	if(line.find(key) == 0)
	{
		target.add(getLocale(line),getValue(line));
		return true;
	}
	return false;
}

GofunCatEntryData* GofunDataLoader::parseCatInfo(const QString& file)
{
	QStringList catdata = loadFileData(file);
			
	GofunCatEntryData* cdata = new GofunCatEntryData;
	cdata->Catdir = QString(file).remove(QRegExp("\\.directory$"));
			
	for(QStringList::Iterator it = catdata.begin(); it != catdata.end(); ++it)
	{
		if(cdata->parseLine((*it)))
			0;
		else
			cdata->Unknownkeys += (*it);
	}
		
	cdata->File = file;
	
	return cdata;	
}

GofunDesktopEntryData* GofunDataLoader::parseGofunFile(const QString& file)
{
	QStringList gofundata = loadFileData(file);

	GofunDesktopEntryData* idata = new GofunDesktopEntryData;
	bool found_type = false;
	QString section;

	for(QStringList::Iterator it = gofundata.begin(); it != gofundata.end(); ++it)
	{
		if((*it)[0] == '[')
			section = (*it);
		if(section != "[Desktop Entry]")
			continue;
		
		if(idata->parseLine((*it)))
		{
			if(!(found_type == true || idata->Type.isEmpty()))
			{
			if(idata->Type == "Application")
			{
				GofunDesktopEntryData* tmp = new GofunApplicationEntryData;
				*tmp = *idata;
				delete idata;
				idata = tmp;
				found_type = true;
				for(QStringList::Iterator sit = idata->Unknownkeys.begin(); sit != idata->Unknownkeys.end(); ++sit)
				{
					if(idata->parseLine((*sit)))
						remove((*sit));
				}
			}
			else if(idata->Type == "Link")
			{
				GofunDesktopEntryData* tmp = new GofunLinkEntryData;
				*tmp = *idata;
				delete idata;
				idata = tmp;
				found_type = true;
				for(QStringList::Iterator sit = idata->Unknownkeys.begin(); sit != idata->Unknownkeys.end(); ++sit)
				{
					if(idata->parseLine((*sit)))
						remove((*sit));
				}
			}
			else if(idata->Type == "FSDevice")
			{
				GofunDesktopEntryData* tmp = new GofunFSDeviceEntryData;
				*tmp = *idata;
				delete idata;
				idata = tmp;
				found_type = true;
				for(QStringList::Iterator sit = idata->Unknownkeys.begin(); sit != idata->Unknownkeys.end(); ++sit)
				{
					if(idata->parseLine((*sit)))
						remove((*sit));
				}
			}
			}
		}
		else
		{
			idata->Unknownkeys += (*it);
		}
	}
	
	idata->File = file;
	
	return idata;
}

std::vector<GofunDesktopEntryData*>* GofunDataLoader::parseCatdir(const QString& catdir)
{
	glob_t files;
	glob((catdir + "*.desktop"),0,0,&files);
	
	std::vector<GofunDesktopEntryData*>* VoID = new std::vector<GofunDesktopEntryData*>;
	
	for(int i = 0; i <files.gl_pathc; ++i)
	{
		if((QString(files.gl_pathv[i]) == catdir + ".directory")  || (QString(files.gl_pathv[i]).find(".*~$")) != -1)
		{
			continue;
		}
		VoID->push_back(parseGofunFile(files.gl_pathv[i]));
	}
	globfree(&files);
	
	return VoID;
}

std::vector<GofunCatEntryData>* GofunDataLoader::getData()
{
	glob_t categories; //default data dir 
	glob((GSC::get()->gofun_dir + "/*/.directory").ascii(),0,0,&categories);

	std::vector<GofunCatEntryData>* VaCD = new std::vector<GofunCatEntryData>;
	
	GofunCatEntryData* p_cdata = parseCatInfo("tools/.directory");
	VaCD->push_back(*p_cdata);
	delete p_cdata;
	
	for(int i = 0; i < categories.gl_pathc; ++i)
	{
		GofunCatEntryData* p_cdata = parseCatInfo(categories.gl_pathv[i]);
		VaCD->push_back(*p_cdata);
		delete p_cdata;
	}
	globfree(&categories);
	
	for(std::vector<GofunCatEntryData>::iterator it = VaCD->begin(); it != VaCD->end(); ++it)
	{
		(*it).ItemData = parseCatdir((*it).Catdir);
	}
 
	return VaCD;
}

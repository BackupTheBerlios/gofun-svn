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
#include <qapplication.h>
#include <qurl.h>
 
#include "gofun_data.h"
#include "gofun_settings_container.h"
#include "gofun_item.h"
#include "gofun_application_item.h"
#include "gofun_fsdevice_item.h"
#include "gofun_link_item.h"
#include "gofun_cat_button.h"
 
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
		while ( !stream.atEnd() )
		{
			lines += stream.readLine(); // line of text excluding '\n'
        	}
        	file.close();
    	}
	return lines;
}

bool GofunDataLoader::parseLine(const QString& key, const QString& line, QString& target)
{
	if(line.stripWhiteSpace().find(key) == 0)
	{
		target = getValue(line);
		return true;
	}
	return false;
}

bool GofunDataLoader::parseLine(const QString& key, const QString& line, std::vector<QString>& target)
{
	if(line.stripWhiteSpace().find(key) == 0)
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
	if(line.stripWhiteSpace().find(key) == 0)
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


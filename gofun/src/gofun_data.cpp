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
 
#include "gofun_data.h"
#include "gofun_settings.h"

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

GofunCatData* GofunDataLoader::parse_cat_info(const QString& file)
{
	QStringList catdata = load_file_data(file);
			
	GofunCatData* cdata = new GofunCatData;
	cdata->Catdir = QString(file).remove(QRegExp("\\.directory$"));
			
	for(QStringList::Iterator it = catdata.begin(); it != catdata.end(); ++it)
	{
		if((*it).find("Name=") == 0)
			cdata->Name = get_value((*it));
		else if((*it).find("Comment=") == 0)
			cdata->Comment = get_value((*it));
		else if((*it).find("Version=") == 0)
			cdata->Version = get_value((*it));
		else if((*it).find("Type=") == 0)
			cdata->Type = get_value((*it));
		else if((*it).find("Icon=") == 0)
			cdata->Icon = get_value((*it));
		else if((*it).find("Encoding=") == 0)
			cdata->Encoding = "";
		else if((*it).find("X-GoFun-Color") == 0)
			cdata->X_GoFun_Color = get_value((*it));
		else if((*it).find("X-GoFun-Background") == 0)
			cdata->X_GoFun_Background = get_value((*it));
		else if((*it).find("[Desktop Entry]") == 0)
			0;
		else
			cdata->Unknownkeys += (*it);
	}
		
	cdata->File = file;
	
	return cdata;	
}

GofunItemData* GofunDataLoader::parse_gofun_file(const QString& file)
{
	QStringList gofundata = load_file_data(file);

	GofunItemData* idata = new GofunItemData;

	for(QStringList::Iterator it = gofundata.begin(); it != gofundata.end(); ++it)
	{
		if((*it).find("Name=") == 0)
			idata->Name = get_value((*it));
		else if ((*it).find("Exec=") == 0)
			idata->Exec = get_value((*it));
		else if((*it).find("Path=") == 0)
			idata->Path = get_value((*it));
		else if((*it).find("Comment=") == 0)
			idata->Comment = get_value((*it));
		else if((*it).find("Icon=") == 0)
			idata->Icon = get_value((*it));
		else if((*it).find("Terminal=") == 0)
			idata->Terminal = get_value((*it));
		else if((*it).find("Hidden=") == 0)
			idata->Hidden = get_value((*it));
		else if((*it).find("Version=") == 0)
			0;
		else if((*it).find("Type=") == 0)
			0;
		else if((*it).find("Encoding=") == 0)
			0;
		else if((*it).find("X-GoFun-User") == 0)
			idata->X_GoFun_User = get_value((*it));
		else if((*it).find("X-GoFun-NewX") == 0)
			idata->X_GoFun_NewX = get_value((*it));
		else if((*it).find("X-GoFun-Env") == 0)
		{
			QStringList qsl = QStringList::split("\t",get_value((*it)));
			for(QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it)
			{
				idata->X_GoFun_Env.push_back((*it));
			}
		}
		else if((*it).find("X-GoFun-Parameter") == 0)
		{
			if((*it).find("X-GoFun-Parameter-Flag") == 0)
			{
				idata->X_GoFun_Parameter[get_key((*it)).remove(0,QString("X-GoFun-Parameter-Flag-").length()).toInt()].Flag = get_value((*it));
			}
			else if((*it).find("X-GoFun-Parameter-Values") == 0)
			{
				idata->X_GoFun_Parameter[get_key((*it)).remove(0,QString("X-GoFun-Parameter-Values-").length()).toInt()].Values = QStringList::split(';',get_value((*it)));
			}
			else if((*it).find("X-GoFun-Parameter-Default") == 0)
			{
				idata->X_GoFun_Parameter[get_key((*it)).remove(0,QString("X-GoFun-Parameter-Default-").length()).toInt()].Default_Value = get_value((*it));
			}
			else if((*it).find("X-GoFun-Parameter-Prompt") == 0)
			{
				idata->X_GoFun_Parameter[get_key((*it)).remove(0,QString("X-GoFun-Parameter-Prompt-").length()).toInt()].Prompt = get_value((*it));
			}
			else if((*it).find("X-GoFun-Parameter-Comment") == 0)
			{
				idata->X_GoFun_Parameter[get_key((*it)).remove(0,QString("X-GoFun-Parameter-Comment-").length()).toInt()].Comment = get_value((*it));
			}
		}
	}
	
	idata->File = file;
	
	return idata;
}

std::vector<GofunItemData>* GofunDataLoader::parse_catdir(const QString& catdir)
{
	glob_t files;
	glob((catdir + "*.desktop"),0,0,&files);
	
	std::vector<GofunItemData>* VoID = new std::vector<GofunItemData>;
	
	for(int i = 0; i <files.gl_pathc; ++i)
	{
		if((QString(files.gl_pathv[i]) == catdir + ".directory")  || (QString(files.gl_pathv[i]).find(".*~$")) != -1)
		{
			continue;
		}
		GofunItemData* p_idata = parse_gofun_file(files.gl_pathv[i]);
		VoID->push_back(*p_idata);
		delete p_idata;
	}
	globfree(&files);
	
	return VoID;
}

std::vector<GofunCatData>* GofunDataLoader::getData()
{
	glob_t categories; //default data dir 
	glob((GSC::get()->gofun_dir + "/*/.directory").ascii(),0,0,&categories);

	std::vector<GofunCatData>* VaCD = new std::vector<GofunCatData>;
	
	for(int i = 0; i < categories.gl_pathc; ++i)
	{
		GofunCatData* p_cdata = parse_cat_info(categories.gl_pathv[i]);
		VaCD->push_back(*p_cdata);
		delete p_cdata;
	}
	globfree(&categories);
	
	for(std::vector<GofunCatData>::iterator it = VaCD->begin(); it != VaCD->end(); ++it)
	{
		std::vector<GofunItemData>* _gofun = parse_catdir((*it).Catdir);
		(*it).ItemData = _gofun;
	}
 
	return VaCD;
}

/***************************************************************************
 *   Copyright (C) 2005 by Tobias Glaesser                                 *
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

#include <qapplication.h>
 
#include "gofun_application_entry_data.h"
#include "gofun_application_item.h"
#include "gofun_data.h"
#include "gofun_settings.h"
#include "gofun_parameter_prompt.h"
#include "gofun_misc.h"
#include "gofun_iconview.h"

GofunDesktopObject* GofunApplicationEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	GofunApplicationItem* item = new GofunApplicationItem(dynamic_cast<GofunIconView*>(parent),Name);
	item->setData(this);
	return item;	
}

void GofunApplicationEntryData::execute()
{
	QString poststring;
	QProcess proc;
	QString exec = Exec;
	
	if(!X_GoFun_Parameter.empty())
	{
		GofunParameterPrompt* prompt = new GofunParameterPrompt();
		for(std::map<int,GofunParameterData>::iterator it = X_GoFun_Parameter.begin();it != X_GoFun_Parameter.end(); ++it)
		{
			prompt->addParameter(&((*it).second));
		}
		if(prompt->exec() == QDialog::Accepted)
		{
			exec += prompt->parameterString();
		}
		else
		{
			return;
		}
	}
	if(!X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = X_GoFun_Env.begin(); it != X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			QStringList vk_pair = QStringList::split('=',(*it));
			exec = "export " + vk_pair[0] + "='" + QString((*it)).remove(0,vk_pair[0].length()+1) + "';" + exec;
		}
	}
	if(Terminal == "true")
	{ 
		addSplittedProcArgument(&proc,GSC::get()->terminal_cmd);
		if(exec[exec.length()-1] == ';')
			exec.setLength(exec.length()-1);
		exec += ";echo -e \"\\E[${2:-44};${3:-7}m\n" + QObject::tr("End of execution has been reached. Press any key to remove this terminal\";");
		exec += "read -n 1";
		
	}
	if(Path.isEmpty())
	{
		proc.setWorkingDirectory(QDir::homeDirPath());
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		exec = "cd " + GofunMisc::shellify_path(QDir::homeDirPath()) + ";" + exec;
		proc.addArgument(exec);
	}
	else
	{
		proc.setWorkingDirectory(QDir(Path));
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		exec = "cd " + GofunMisc::shellify_path(Path) + ";" + exec;
		proc.addArgument(exec);
	}
	if(!X_GoFun_User.isEmpty())
	{		
		QString spa_file = saveProcArguments(&proc);
		
		QProcess proc_gosu;
		
		proc_gosu.addArgument("gosu");
		proc_gosu.addArgument(X_GoFun_User);
		proc_gosu.addArgument("--color");
		proc_gosu.addArgument(qApp->palette().color(QPalette::Active,QColorGroup::Background).name());
		proc_gosu.addArgument("-l");
		proc_gosu.addArgument("-g");
		proc_gosu.start();
		
	}
	else
	{
		saveProcArguments(&proc);
		proc.clearArguments();
		proc.addArgument("golauncher");
		proc.addArgument("-datafile");
		proc.addArgument(QDir::homeDirPath() + "/.gofun/tmp_proc_exec");
		if(X_GoFun_NewX == "true")
			proc.addArgument("-xstart");
		proc.start();
	}
}

void GofunApplicationEntryData::addSplittedProcArgument(QProcess* proc,const QString& argument)
{
	QStringList arguments = QStringList::split(' ',argument);
	for(QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		proc->addArgument((*it));
	}
}

QString GofunApplicationEntryData::saveProcArguments(QProcess* proc)
{
	QStringList arguments = proc->arguments();
	QString tmp = QString(getenv("HOME")) + QString("/.gofun/tmp_proc_exec"); //You'll probably wonder "wtf is this tmp needed"? The answer is "cause gcc is mucking around without this ugly code on some systems"
	QFile file(tmp);
	if(file.open( IO_WriteOnly ))
	{
		QTextStream stream(&file);
		for(QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it)
		{
			stream << (*it) << '\0';
		}
		file.close();
	}
	return file.name();
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
			QString num = GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Comment-").length());
			if(num.find("[") != -1)
				num.truncate(num.find("["));
			X_GoFun_Parameter[num.toInt()].Comment = GofunDataLoader::getValue(line);
		}
		else if(line.find("X-GoFun-Parameter-Type") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Type-").length()).toInt()].Type = GofunDataLoader::getValue(line);
		}
		else if(line.find("X-GoFun-Parameter-Minimum") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Minimum-").length()).toInt()].Minimum = GofunDataLoader::getValue(line);
		}
		else if(line.find("X-GoFun-Parameter-Maximum") == 0)
		{
			X_GoFun_Parameter[GofunDataLoader::getKey(line).remove(0,QString("X-GoFun-Parameter-Maximum-").length()).toInt()].Maximum = GofunDataLoader::getValue(line);
		}
		else
			return false;
	}
	else
			return false;
	return true;
}

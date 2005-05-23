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
#include <qprocess.h>
 
#include "gofun_application_entry_data.h"
#include "gofun_data.h"
#include "gofun_settings_container.h"
#include "gofun_parameter_prompt.h"
#include "gofun_misc.h"
#include "gofun_shell_operations.h"

/*GofunDesktopObject* GofunApplicationEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	GofunApplicationItem* item = new GofunApplicationItem(dynamic_cast<GofunIconView*>(parent),Name);
	item->setData(this);
	return item;	
}*/

bool GofunApplicationEntryData::handleParameters(QString& parstr)
{
	if(X_GoFun_Parameter.empty())
	{
		parstr = QString();
		return true;
	}
	
	GofunParameterPrompt* prompt = new GofunParameterPrompt();
	for(std::map<int,GofunParameterData>::iterator it = X_GoFun_Parameter.begin();it != X_GoFun_Parameter.end(); ++it)
		prompt->addParameter(&((*it).second));
	if(prompt->exec() == QDialog::Accepted)
		parstr = prompt->parameterString();
	else
		return false;
	return true;
}

void GofunApplicationEntryData::handleEnvVars()
{
	if(X_GoFun_Env.empty())
		return;
	
	QStringList envs;
	for(std::vector<QString>::iterator it = X_GoFun_Env.begin(); it != X_GoFun_Env.end(); ++it)
	{
		if((*it).isEmpty())
			continue;
		QStringList vk_pair = QStringList::split('=',(*it));
		envs += vk_pair[0] + "=" + GofunShellOperations::extendFileString(vk_pair[1]);
	}
	saveStringListToFile(QDir::homeDirPath() + "/.gofun/tmp_proc_env",&envs);
}

void GofunApplicationEntryData::execute() //FIXME: 1. hackish, 2. maybe outsource code to golauncher 3. at least clean up and split 4. use golauncher to do stuff like opening in terminal
{
	QString poststring;
	QProcess proc;
	QString exec(Exec);

	handleParameterVariables(exec);
	QString parstr;
	if(handleParameters(parstr))
		exec += parstr;
	else
		return;
	handleEnvVars();
	if(GofunMisc::stringToBool(Terminal))
	{ 
		GofunMisc::addSplittedProcArgument(&proc,GSC::get()->terminal_cmd);
		proc.addArgument("-e");
		if(exec[exec.length()-1] == ';')
			exec.setLength(exec.length()-1);
		exec += ";echo -e \"\\E[${2:-44};${3:-7}m\n" + QObject::tr("End of execution has been reached.\nPress any key to remove this terminal.\nPress -c- to continue operation in this terminal. \";");
		exec += "read -n 1 EVAL; tput sgr0; if [ \"$EVAL\" == \"c\" ]; then /bin/sh; fi";
	}
	proc.addArgument("/bin/sh");
	proc.addArgument("-c");
	if(Path.isEmpty())
	{
		proc.setWorkingDirectory(QDir::homeDirPath());
		exec = "cd " + GofunShellOperations::shellifyPath(QDir::homeDirPath()) + ";" + exec;
	}
	else
	{
		proc.setWorkingDirectory(QDir(Path));
		exec = "cd " + GofunShellOperations::shellifyPath(Path) + ";" + exec;
	}
	proc.addArgument(exec);
	saveOptionalArguments();
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
		proc.addArgument("-argumentsfile");
		proc.addArgument(QDir::homeDirPath() + "/.gofun/tmp_proc_exec");
		proc.addArgument("-envvarsfile");
		proc.addArgument(QDir::homeDirPath() + "/.gofun/tmp_proc_env");
		proc.addArgument("-optargsfile");
		proc.addArgument(QDir::homeDirPath() + "/.gofun/tmp_opt_args");
		/*if(GofunMisc::stringToBool(X_GoFun_NewX))
		{
			proc.addArgument("-xstart");
			if(!X_GoFun_XOptions.isEmpty())
			{
				proc.addArgument("-xoptions");
				proc.addArgument(X_GoFun_XOptions);
			}
		}*/
		proc.start();
	}
}

QString GofunApplicationEntryData::saveOptionalArguments()
{
	QStringList optional_arguments;
	optional_arguments += Path;
	optional_arguments += X_GoFun_XOptions;

	saveStringListToFile(QDir::homeDirPath() + "/.gofun/tmp_opt_args",&optional_arguments);

	if(GofunMisc::stringToBool(X_GoFun_NewX))
		QFile(QDir::homeDirPath() + "/.gofun/tmp_opt_newx").open(IO_WriteOnly);
}

QString GofunApplicationEntryData::saveStringListToFile(const QString& _file,QStringList* stringlist)
{
	QFile file(_file);
	if(file.open( IO_WriteOnly ))
	{
		QTextStream stream(&file);
		for(QStringList::Iterator it = stringlist->begin(); it != stringlist->end(); ++it)
		{
			stream << (*it) << '\0';
		}
		file.close();
	}
	return file.name();
}

QString GofunApplicationEntryData::saveProcArguments(QProcess* proc)
{
	QString tmp = QString(getenv("HOME")) + QString("/.gofun/tmp_proc_exec"); //You'll probably wonder "wtf is this tmp needed"? The answer is "cause gcc is mucking around without this ugly code on some systems"
	QStringList arguments = proc->arguments();
	return saveStringListToFile(tmp,&arguments);
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
	|| GofunDataLoader::parseLine("X-GoFun-XOptions",line,X_GoFun_XOptions)
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

GofunApplicationEntryData* GofunApplicationEntryData::makeCopy()
{
	GofunApplicationEntryData* copy = new GofunApplicationEntryData;
	*copy = *this;
	return copy;
}

void GofunApplicationEntryData::save()
{
	GofunDesktopEntryData::save();

	QFile file( File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "Type=Application\n";
		if(!Exec.isEmpty())
			stream << "Exec=" << Exec << "\n";
		if(!TryExec.isEmpty())
			stream << "TryExec=" << TryExec << "\n";
		if(!Path.isEmpty())
			stream << "Path=" << Path << "\n";
		stream << "Terminal=" << Terminal << "\n";
		stream << "X-GoFun-NewX=" << X_GoFun_NewX << "\n";
		stream << "X-GoFun-XOptions=" << X_GoFun_XOptions << "\n";
		if(!X_GoFun_Env.empty())
		{
			stream << "X-GoFun-Env=";
			for(std::vector<QString>::iterator it = X_GoFun_Env.begin(); it != X_GoFun_Env.end(); ++it)
			{
				QString encoded = (*it);
				encoded.replace(";","\\;");
				stream << encoded << ";";
			}
			stream << "\n";
		}
		if(!X_GoFun_User.isEmpty())
			stream << "X-GoFun-User=" << X_GoFun_User << "\n";
		if(!X_GoFun_Parameter.empty())
		{
			for(std::map<int,GofunParameterData>::iterator it = X_GoFun_Parameter.begin(); it != X_GoFun_Parameter.end(); ++it)
			{
				stream << "X-GoFun-Parameter-Prompt-" << (*it).first << "=" << (*it).second.Prompt << "\n";
				if(!(*it).second.Flag.isEmpty())
					stream << "X-GoFun-Parameter-Flag-" << (*it).first << "=" << (*it).second.Flag << "\n";
				if(!(*it).second.Values.join(";").isEmpty())
					stream << "X-GoFun-Parameter-Values-" << (*it).first << "=" << (*it).second.Values.join(";") << "\n";
				if(!(*it).second.Default_Value.isEmpty())
					stream << "X-GoFun-Parameter-Default-" << (*it).first << "=" << (*it).second.Default_Value << "\n";
				stream << "X-GoFun-Parameter-Type-" << (*it).first << "=" << (*it).second.Type << "\n";
				if(!(*it).second.Minimum.isEmpty())
					stream << "X-GoFun-Parameter-Minimum-" << (*it).first << "=" << (*it).second.Minimum << "\n";
				if(!(*it).second.Maximum.isEmpty())
					stream << "X-GoFun-Parameter-Maximum-" << (*it).first << "=" << (*it).second.Maximum << "\n";	
				(*it).second.Comment.desktopEntryPrint("X-GoFun-Parameter-Comment-" + QString::number((*it).first),stream);
			}
		}
		stream << Unknownkeys.join("\n") << "\n";
		file.close();
	}
}

void GofunApplicationEntryData::handleParameterVariables(QString& exec)
{
	exec.replace("%u","");
}




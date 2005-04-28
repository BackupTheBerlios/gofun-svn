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
 
#include <unistd.h>  

#include <iostream>
 
#include <qapplication.h>
#include <qprocess.h>
#include <qmessagebox.h>
#include <qeventloop.h>

#include "gofun_misc.h"
#include "gofun_process_logger.h"
#include "gofun_process_problem_feedback.h"
#include "golauncher.h"
#include "gofun_data.h"
#include "gofun_desktop_entry_data.h"
#include "gofun_application_entry_data.h"
#include "gofun_process.h"
#include "golauncher_widget.h"

Golauncher::Golauncher()
{
	envvars = 0;	

	xmode = false;
	xstart = false;
	wait = false;
	
	launch_data_loaded = false;
}

void Golauncher::loadLaunchData()
{
	if(!argumentsfile.isEmpty())
		getArgumentsFromArgumentsFile(argumentsfile,&arguments);
	else if(!desktopentryfile.isEmpty())
		getArgumentsFromDesktopEntry(desktopentryfile,&arguments);
	
	if(!envvarsfile.isEmpty())
	{
		envvars = new QStringList;
		*envvars = getOriginalEnvironmentVariables();
		getEnvVarsFromEnvVarsFile(envvarsfile,envvars);
	}
	
	launch_data_loaded = true;
}

void Golauncher::launch()
{
	QProcess* proc = new QProcess;
	if(xstart)
	{
		proc->addArgument("xinit");
		proc->addArgument(GofunMisc::bin_dir() + "golauncher");
		proc->addArgument("-xmode");
		proc->addArgument("-argumentsfile");
		proc->addArgument(argumentsfile);
		proc->addArgument("--");
		int xservnum = getNumberOfRunningXServers();
		proc->addArgument(":"+QString::number(xservnum));
		proc->start();

		sleep(2000);
			
		return;
	}
	
	if(!launch_data_loaded)
	{
		loadLaunchData();
		cleanupLaunchDataOnFS();
	}

	proc->setArguments(arguments);
	proc_output = "";
	if(dumpoutput)
		GofunProcessLogger::get()->connectProcToStdout(proc);
	else
		GofunProcessLogger::get()->connectProcToBuffer(proc,&proc_output);
	if(wait)
	{
		GofunProcess* gproc = new GofunProcess;
		gproc->setArguments(arguments);
		gproc->start(envvars);
	}
	else
		proc->start(envvars);

	if(!wait)
		if(!proc->isRunning())
			problemFeedback(proc);

	if(xmode)
		launchWidget();
	else
		if(sleep(1) && !proc->isRunning())
			problemFeedback(proc);

		
	//We _could_ get the processes started by the process with processIdentifier using 'ps --ppid'
	//the first number in the line is alway a pidi	
}

void Golauncher::cleanupLaunchDataOnFS()
{
	if(!argumentsfile.isEmpty())
		QFile(argumentsfile).remove();
	if(!envvarsfile.isEmpty())
		QFile(envvarsfile).remove();
}

void Golauncher::problemFeedback(QProcess* proc)
{
	if(!proc->isRunning())
		if(!proc->normalExit() || proc->exitStatus() != 0)
		{
			GofunProcessProblemFeedback pp_feedback;
			pp_feedback.setCommand(proc->arguments().join(" "));
			pp_feedback.setExitStatus(proc->exitStatus());
			pp_feedback.setNormalExit(proc->normalExit());
			pp_feedback.setOutput(proc_output);
			if(pp_feedback.exec() == QDialog::Accepted)
			{
				launch();
			}
		}
}

void Golauncher::setXMode(bool b)
{
	xmode = b;
}

void Golauncher::setXStart(bool b)
{
	xstart = b;
}

void Golauncher::setArgumentsFile(const QString& s)
{
	argumentsfile = s;
}

void Golauncher::setDesktopEntryFile(const QString& s)
{
	desktopentryfile = s;
}

void Golauncher::getArgumentsFromArgumentsFile(const QString& _file, QStringList* arguments)
{
	getStringListFromNullTerminatedStringListFile(_file,arguments);
}

void Golauncher::getArgumentsFromDesktopEntry(const QString& file, QStringList* arguments)
{
	GofunDesktopEntryData* desktop_entry = GofunDataLoader::parseGofunFile(file);
	GofunApplicationEntryData* application_entry;
	if((application_entry = dynamic_cast<GofunApplicationEntryData*>(desktop_entry)))
		application_entry->execute();
}

void Golauncher::launchWidget()
{
	GolauncherWidget wid;
	
	//Declare gofun_widget to the official mainWidget
	qApp->setMainWidget(&wid);
	//Paint it on the screen
	wid.show();
	//Finally we start the application!
	qApp->exec();
}


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	Golauncher golauncher;
			
	for ( int i = 0; i < app.argc(); i++ )
	{
		if((QString(app.argv()[i]) == "-argumentsfile") && (i+1 < app.argc()))
			golauncher.setArgumentsFile(app.argv()[i+1]);
		else if((QString(app.argv()[i]) == "-desktopentryfile") && (i+1 < app.argc()))
			golauncher.setDesktopEntryFile(app.argv()[i+1]);
		else if((QString(app.argv()[i]) == "-envvarsfile") && (i+1 < app.argc()))
			golauncher.setEnvVarsFile(app.argv()[i+1]);
		else if((QString(app.argv()[i])) == "-xmode")
			golauncher.setXMode(true);
		else if((QString(app.argv()[i])) == "-xstart")
			golauncher.setXStart(true);
		else if((QString(app.argv()[i])) == "-wait")
			golauncher.setWait(true);
		else if((QString(app.argv()[i])) == "-dumpoutput")
			golauncher.setDumpOutput(true);
	}
	
	golauncher.launch();
	return 0;
}

void Golauncher::setWait(bool b)
{
	wait = b;
}

int Golauncher::getNumberOfRunningXServers()
{
	(GofunMisc::shell_call("ps -Ac | grep X | wc -l").stripWhiteSpace()).toInt();
}

void Golauncher::setDumpOutput(bool b)
{
	dumpoutput = b;
}

void Golauncher::setEnvVarsFile(const QString& _file)
{
	envvarsfile = _file;
}

void Golauncher::getEnvVarsFromEnvVarsFile(const QString& file,QStringList* envvars)
{
	getStringListFromNullTerminatedStringListFile(file,envvars);
}

void Golauncher::getStringListFromNullTerminatedStringListFile(const QString& _file, QStringList* stringlist)
{
	QFile file(_file);
	if(file.open( IO_ReadOnly ))
	{
		QDataStream stream(&file);
		QString str;
		Q_INT8 c;
		while ( !stream.atEnd() )
		{
			stream >> c;
			if(c != '\0')
			{
				str += c;
			}
			else
			{
				*stringlist += str;
				str = "";
			}
		}
		file.close();
	}
}

QStringList Golauncher::getOriginalEnvironmentVariables()
{
	QStringList envs;
	for(int i = 0;environ[i]; ++i)
		envs += environ[i];
	return envs;
}


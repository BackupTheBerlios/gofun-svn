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
#include "gofun_shell_operations.h"
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
	keep_data_files = false;
	dumpoutput = false;
	
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

	if(!optargsfile.isEmpty())
	{
		getOptionalArgumentsFromOptionalArgumentsFile(optargsfile);
	}
	
	launch_data_loaded = true;
}

void Golauncher::launch()
{
	if(!optargsfile.isEmpty())
	{
		QFile file(QFileInfo(optargsfile).dirPath() + "/tmp_opt_newx");
		if(file.open(IO_ReadOnly))
		{
			file.readLine(x_options,1024);
			file.close();
			file.remove();
			setXStart(true);
		}
	}

	if(xstart)
	{
		startInNewX();
		return;
	}
	
	if(!launch_data_loaded)
	{
		loadLaunchData();
		if(!keep_data_files)
			cleanupLaunchDataOnFS();
	}

	if(!working_directory.isEmpty())
	{
		chdir(working_directory);
		if(!envvars)
		{	//NOTE: code duplication with loadLaunchData()
		 	envvars = new QStringList;
			*envvars = getOriginalEnvironmentVariables();
		}
		envvars->gres("PATH=","PATH=.:",true); //hack to make execution of binaries in current directory possible without ./
	}

	QProcess* proc = new QProcess;
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
	if(!optargsfile.isEmpty())
		QFile(optargsfile).remove();
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

void Golauncher::getOptionalArgumentsFromOptionalArgumentsFile(const QString& file)
{
	QStringList tmp;
	getStringListFromNullTerminatedStringListFile(file,&tmp);
	working_directory = tmp[0];
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
	
	GoArgParser arg_parser(argc,argv);
	
	while(arg_parser.notDone())
	{
		if(arg_parser.parse("-argumentsfile",true))
			golauncher.setArgumentsFile(arg_parser.getParameter());
		else if(arg_parser.parse("-desktopentryfile",true))
			golauncher.setDesktopEntryFile(arg_parser.getParameter());
		else if(arg_parser.parse("-envvarsfile",true))
			golauncher.setEnvVarsFile(arg_parser.getParameter());
		else if(arg_parser.parse("-optargsfile",true))
			golauncher.setOptionalArgumentsFile(arg_parser.getParameter());
		else if(arg_parser.parse("-xmode"))
			golauncher.setXMode(true);
		else if(arg_parser.parse("-xstart"))
			golauncher.setXStart(true);
		else if(arg_parser.parse("-wait"))
			golauncher.setWait(true);
		else if(arg_parser.parse("-dumpoutput"))
			golauncher.setDumpOutput(true);
		else if(arg_parser.parse("-keepdatafiles"))
			golauncher.setKeepDataFiles(true);
		else if(arg_parser.parse("-workingdir",true))
			golauncher.setWorkingDirectory(arg_parser.getParameter());
		else if(arg_parser.parse("-xoptions",true))
			golauncher.setXOptions(arg_parser.getParameter());
		
		arg_parser.increment();
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
	(GofunShellOperations::shellCall("ps -Ac | grep X | wc -l").stripWhiteSpace()).toInt();
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

void Golauncher::setWorkingDirectory(const QString& _working_directory)
{
	working_directory = _working_directory;
}

void Golauncher::setKeepDataFiles(bool b)
{
	keep_data_files = b;
}

void Golauncher::setXOptions(const QString& s)
{
	x_options = s;
}

void Golauncher::setOptionalArgumentsFile(const QString& file)
{
	optargsfile = file;
}

void Golauncher::startInNewX()
{
		QProcess* proc = new QProcess;
		proc->addArgument("xinit");
		proc->addArgument(GofunMisc::binDir() + "golauncher");
		proc->addArgument("-xmode");
		proc->addArgument("-argumentsfile");
		proc->addArgument(argumentsfile);
		proc->addArgument("-envvarsfile");
		proc->addArgument(envvarsfile);
		proc->addArgument("-optargsfile");
		proc->addArgument(optargsfile);
		proc->addArgument("--");
		int xservnum = getNumberOfRunningXServers();
		proc->addArgument(":"+QString::number(xservnum));
		if(!x_options.isEmpty())
			GofunMisc::addSplittedProcArgument(proc,x_options);
		proc->start();

		sleep(2000);
}

GoArgParser::GoArgParser(int _argc, char** _argv) : argc(_argc), argv(_argv), i(0)
{}

bool GoArgParser::notDone()
{
	return (i < argc);
}

bool GoArgParser::increment()
{
	++i;
}

bool GoArgParser::parse(const QString& arg, bool expect_parameter)
{
	if(QString(argv[i]) == arg)
	{
		if(expect_parameter && (i+1 < argc))
			parameter = argv[i+1];
		else
			parameter = "";
		return true;
	}
	return false;
}

QString GoArgParser::getParameter()
{
	return parameter;
}








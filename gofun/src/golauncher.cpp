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
 
#include <qapplication.h>
#include <qprocess.h>
#include <qmessagebox.h>

#include "gofun_misc.h"
#include "gofun_process_logger.h"
#include "gofun_process_problem_feedback.h"
#include "golauncher.h"

Golauncher::Golauncher()
{
	xmode = false;
	xstart = false;
}

void Golauncher::launch()
{
	QProcess* proc = new QProcess;
	if(xstart)
	{
		proc->addArgument("xinit");
		proc->addArgument(GofunMisc::bin_dir() + "golauncher");
		proc->addArgument("-xmode");
		proc->addArgument("-datafile");
		proc->addArgument(datafile);
		proc->addArgument("--");
		QString xservnum = GofunMisc::shell_call("ps -Ac | grep X | wc -l").simplifyWhiteSpace();
		proc->addArgument(":"+xservnum);
		proc->start();

		sleep(2000);
			
		return;
	}
	
	QStringList arguments;
	QFile file(datafile);
	if(file.open( IO_ReadOnly ))
	{
		QDataStream stream(&file);
		QString str;
		while ( !stream.atEnd() )
		{
			Q_INT8 c;
			stream >> c;
			if(c != 0)
			{
				str += c;
			}
			else
			{
				arguments += str;
				str = "";
			}
		}
		file.close();
	}
	proc->setArguments(arguments);
	QString proc_output;
	GofunProcessLogger::get()->connectProcToBuffer(proc,&proc_output);
	proc->start();
	
	if(xmode)
	{	
		GolauncherWidget wid;
		
		//Declare gofun_widget to the official mainWidget
		qApp->setMainWidget(&wid);
		//Paint it on the screen
		wid.show();
		//Finally we start the application!
		qApp->exec();
	}
	
	if(!proc->isRunning() || (sleep(1) && !proc->isRunning()))
		if(!proc->normalExit() || proc->exitStatus() != 0)
		{
			GofunProcessProblemFeedback pp_feedback;
			pp_feedback.setExitStatus(proc->exitStatus());
			pp_feedback.setNormalExit(proc->normalExit());
			pp_feedback.setOutput(proc_output);
			if(pp_feedback.exec() == QDialog::Accepted)
			{
				launch();
			}
		}
		
	//We _could_ get the processes started by the process with processIdentifier using 'ps --ppid'
	//the first number in the line is alway a pidi	
}

void Golauncher::setXMode(bool b)
{
	xmode = b;
}

void Golauncher::setXStart(bool b)
{
	xstart = b;
}

void Golauncher::setDatafile(const QString& s)
{
	datafile = s;
}

GolauncherWidget::GolauncherWidget()
{
	quit = new QPushButton(tr("Quit"),this);
	connect(quit,SIGNAL(clicked()),qApp,SLOT(quit()));
}

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	Golauncher golauncher;
			
	for ( int i = 0; i < app.argc(); i++ )
	{
		if((QString(app.argv()[i]) == "-datafile") && (i+1 < app.argc()))
			golauncher.setDatafile(app.argv()[i+1]);
		else if((QString(app.argv()[i])) == "-xmode")
			golauncher.setXMode(true);
		else if((QString(app.argv()[i])) == "-xstart")
			golauncher.setXStart(true);
	}
	
	golauncher.launch();
	return 0;
}


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
 
#include <qapplication.h>
#include <qprocess.h>
#include <qmessagebox.h>

#include "gofun_misc.h"
#include "golauncher.h"

GolauncherWidget::GolauncherWidget()
{
	quit = new QPushButton(tr("Quit"),this);
	connect(quit,SIGNAL(clicked()),qApp,SLOT(quit()));
}

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	QString datafile;
	bool xmode = false;
	bool xstart = false;
		
	for ( int i = 0; i < app.argc(); i++ )
	{
		if((QString(app.argv()[i]) == "-datafile") && (i+1 < app.argc()))
			datafile = app.argv()[i+1];
		else if((QString(app.argv()[i])) == "-xmode")
			xmode = true;	
		else if((QString(app.argv()[i])) == "-xstart")
			xstart = true;
	}
	
	
	QProcess* proc = new QProcess;
	if(xstart)
	{
		proc->addArgument("xinit");
		proc->addArgument("/usr/local/bin/golauncher");
		proc->addArgument("-xmode");
		proc->addArgument("-datafile");
		proc->addArgument(datafile);
		proc->addArgument("--");
		QString xservnum = GofunMisc::shell_call("ps -Ac | grep X | wc -l");
		xservnum = xservnum.simplifyWhiteSpace();
		proc->addArgument(":"+xservnum);
		proc->start();
		
		sleep(2000);
			
		return 0;
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
	proc->start();
	
	if(xmode)
	{
	GolauncherWidget wid;
	
	  //Declare gofun_widget to the official mainWidget
  	app.setMainWidget(&wid);
  	//Paint it on the screen
  	wid.show();
  	//Finally we start the application!
  	app.exec();
	}	
}


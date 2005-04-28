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

#include <qwidget.h>
#include <qpushbutton.h>
 
#ifndef GOLAUNCHER_WIDGET
#define GOLAUNCHER_WIDGET

class Golauncher
{
	public:
	Golauncher();
	void launch();

	void loadLaunchData();
	void cleanupLaunchDataOnFS();
	void setXMode(bool);
	void setXStart(bool);
	void setWait(bool);
	void setDumpOutput(bool);
	void setEnvVarsFile(const QString&);
	void setArgumentsFile(const QString&);
	void setDesktopEntryFile(const QString&);
	void getStringListFromNullTerminatedStringListFile(const QString&,QStringList*);
	void getArgumentsFromArgumentsFile(const QString&,QStringList*);
	void getEnvVarsFromEnvVarsFile(const QString&,QStringList*);
	void getArgumentsFromDesktopEntry(const QString&,QStringList*);
	QStringList getOriginalEnvironmentVariables();
	void launchWidget();
	void problemFeedback(QProcess*);
	int getNumberOfRunningXServers();
	
	private:
	bool xmode;
	bool xstart;
	bool wait;
	bool dumpoutput;
	bool launch_data_loaded;
	QString argumentsfile;
	QString envvarsfile;
	QString desktopentryfile;
	QString proc_output;
	
	QStringList arguments;
	QStringList* envvars;
};

#endif


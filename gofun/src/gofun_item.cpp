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

#include <iostream> 

#include <qinputdialog.h>

#include "gofun_misc.h"
#include "gofun_item.h"
#include "gofun.h"
#include "gofun_data.h"
#include "gofun_iconview.h"
#include "gofun_settings.h"
 
GofunItem::GofunItem(GofunIconView* iconview, const QString& string) : QIconViewItem(iconview,string)
{
	data = new GofunItemData();
}

/*GofunItem::~GofunItem()
{
	delete data;
}*/

void GofunItem::save()
{
	QFile file( data->File );
	if ( file.open( IO_WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << "[Desktop Entry]\n";
		stream << "Version=0.9.4\n";
		stream << "Encoding=UTF-8\n";
		stream << "Type=Application\n";
		stream << "Name=" << data->Name << "\n";
		stream << "Exec=" << data->Exec << "\n";
		stream << "Path=" << data->Path << "\n";
		stream << "Icon=" << data->Icon << "\n";
		stream << "Comment=" << data->Comment << "\n";
		stream << "Terminal=" << data->Terminal << "\n";
		stream << "X-GoFun-NewX=" << data->X_GoFun_NewX << "\n";
		if(!data->X_GoFun_Env.empty())
		{
			stream << "X-GoFun-Env=";
			for(std::vector<QString>::iterator it = data->X_GoFun_Env.begin(); it != data->X_GoFun_Env.end(); ++it)
			{
				stream << "\t" << (*it);
			}
			stream << "\n";
		}
		stream << "X-GoFun-User=" << data->X_GoFun_User << "\n";
		stream << data->Unknownkeys << "\n";
		file.close();
	}
}

void GofunItem::deleteEntry()
{
	QProcess proc;
	proc.addArgument("rm");
	proc.addArgument("-f");
	proc.addArgument(data->File);
	proc.start();
	while(proc.isRunning())
	{
	}
	delete this;
}

void GofunItem::loadIcon()
{
	if(!data->Icon.isEmpty())
	{
		QString file;
		if(QFile::exists(data->Icon))
			file = data->Icon;
		else if(!(file = GofunMisc::shell_call("find /usr/share/icons -path *"+data->Icon+"*")).isEmpty())
		{
			//FIXME: that code looks still quite hackish
			QStringList::Iterator choice;
			int m_width = 0;
			QStringList files = QStringList::split("\n",file);
			for(QStringList::Iterator it = files.begin(); it != files.end(); ++it)
			{
				QImage img((*it));
				if(img.width() > m_width)
				{
					file = (*it);
					m_width = img.width();
				}
				if(img.width() == 32)
					break;
			}
		}
		QImage image(file);
		QPixmap pixmap;
		pixmap.convertFromImage(image.scale(32,32));
		setPixmap(pixmap);
	}
}

void GofunItem::setData(GofunItemData* d)
{
	delete data;
	data = d;
	loadIcon();
}

void GofunItem::interpretExecString(QString& exec)
{
	int off = -1;
	while((off = exec.find("$GP",off+1)) != -1)
	{
		if(exec.find("$GPN") == off)
		{
			int i = QInputDialog::getInteger("Enter a number","Enter a number");
			exec.remove(off,off+QString("$GPN").length());
			exec.insert(off,QString::number(i));
		}
		else if(exec.find("$GPT") == off)
		{
		}
	}
}

#include <qtranslator.h>

void GofunItem::executeCommand(ExecuteOption* option)
{
	QString poststring;
	QProcess proc;
	
	QString exec;
	if(!option->Exec.isEmpty())
	exec = option->Exec;
	else
	exec = data->Exec;
	
	interpretExecString(exec);

	if(!data->X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = data->X_GoFun_Env.begin(); it != data->X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			exec = "export " + (*it) + ";" + exec;
		}
	}
	if((!option->terminal.isEmpty()) || (data->Terminal == "true"))
	{ 
		addSplittedProcArgument(&proc,GSC::get()->terminal_cmd);
		exec =  exec + ";echo -e \"\\E[${2:-44};${3:-7}m\n" + QObject::tr("End of execution has been reached. Press any key to remove this terminal\"; read evar");
	}
	if((!option->xinit.isEmpty()) || (data->X_GoFun_NewX == "true"))
	{
		proc.addArgument("xinit");
	}
	if(data->Path.isEmpty())
	{
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		proc.addArgument(exec);
	}
	else
	{
		//#system($prestring."cd ".directory." && ".command.$poststring);
		proc.setWorkingDirectory(QDir(data->Path));
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		exec = "cd " + data->Path + ";" + exec;
		proc.addArgument(exec);
	}
	if((!option->xinit.isEmpty()) || (data->X_GoFun_NewX == "true"))
	{
		proc.addArgument("--");
		QString xservnum = GofunMisc::shell_call("ps -Ac | grep X | wc -l");
		xservnum = xservnum.simplifyWhiteSpace();
		proc.addArgument(":"+xservnum);

	}
	if(!data->X_GoFun_User.isEmpty())
	{		
		QString spa_file = saveProcArguments(&proc);
		
		QProcess proc_gosu;
		proc_gosu.addArgument("gosu");
		proc_gosu.addArgument(data->X_GoFun_User);
		proc_gosu.addArgument("-l");
		proc_gosu.addArgument("-g");
		proc_gosu.start();
		
	}
	else
	{
		proc.start();
	}
}

void GofunItem::addSplittedProcArgument(QProcess* proc,const QString& argument)
{
	QStringList arguments = QStringList::split(' ',argument);
	for(QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		proc->addArgument((*it));
	}
}

QString GofunItem::saveProcArguments(QProcess* proc)
{
	QStringList arguments = proc->arguments();
	QFile file( QString(getenv("HOME")) + QString("/.gofun/tmp_proc_exec"));
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

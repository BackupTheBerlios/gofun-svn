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

#include "gofun_application_item.h"

#include <iostream> 

#include <qapplication.h>
#include <qinputdialog.h>
#include <qtooltip.h>
#include <qprocess.h>
#include <qpopupmenu.h>

#include "gofun_misc.h"
#include "gofun_item.h"
#include "gofun.h"
#include "gofun_data.h"
#include "gofun_iconview.h"
#include "gofun_settings.h"
#include "gofun_parameter_prompt.h"
#include "gofun_costum_start.h"
#include "gofun_application_item_settings.h"

GofunApplicationItem::GofunApplicationItem(GofunIconView* iconview, const QString& string) : GofunItem(iconview,string)
{
	m_data = new GofunApplicationItemData();
}

/*GofunItem::~GofunItem()
{
	delete data;
}*/

void GofunApplicationItem::save()
{
	GofunDesktopObject::save();
	GofunItem::save();

	QFile file( data()->File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "Type=Application\n";
		stream << "Exec=" << data()->Exec << "\n";
		stream << "Path=" << data()->Path << "\n";
		stream << "Terminal=" << data()->Terminal << "\n";
		stream << "X-GoFun-NewX=" << data()->X_GoFun_NewX << "\n";
		if(!data()->X_GoFun_Env.empty())
		{
			stream << "X-GoFun-Env=";
			for(std::vector<QString>::iterator it = data()->X_GoFun_Env.begin(); it != data()->X_GoFun_Env.end(); ++it)
			{
				QString encoded = (*it);
				encoded.replace(";","\\;");
				stream << encoded << ";";
			}
			stream << "\n";
		}
		stream << "X-GoFun-User=" << data()->X_GoFun_User << "\n";
		if(!data()->X_GoFun_Parameter.empty())
		{
			for(std::map<int,GofunParameterData>::iterator it = data()->X_GoFun_Parameter.begin(); it != data()->X_GoFun_Parameter.end(); ++it)
			{
				stream << "X-GoFun-Parameter-Flag-" << (*it).first << "=" << (*it).second.Flag << "\n";
				stream << "X-GoFun-Parameter-Values-" << (*it).first << "=" << (*it).second.Values.join(";") << "\n";
				stream << "X-GoFun-Parameter-Default-" << (*it).first << "=" << (*it).second.Default_Value << "\n";
				stream << "X-GoFun-Parameter-Prompt-" << (*it).first << "=" << (*it).second.Prompt << "\n";
				stream << (*it).second.Comment.desktopEntryPrint("X-GoFun-Parameter-Comment-" + QString::number((*it).first));
			}
		}
		stream << data()->Unknownkeys.join("\n") << "\n";
		file.close();
	}
}

void GofunApplicationItem::setData(GofunItemData* d)
{
	delete m_data;
	m_data = dynamic_cast<GofunApplicationItemData*>(d);
	loadIcon();
	if(!data()->Comment.isEmpty())
		setToolTipText(data()->Comment);
}

void GofunApplicationItem::interpretExecString(QString& exec)
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

void GofunApplicationItem::executeCommand(ExecuteOption* option)
{
	QString poststring;
	QProcess proc;
	
	QString exec;
	if(!option->Exec.isEmpty())
	exec = option->Exec;
	else
	exec = data()->Exec;
	
	interpretExecString(exec);
	
	if(!data()->X_GoFun_Parameter.empty())
	{
		GofunParameterPrompt* prompt = new GofunParameterPrompt();
		for(std::map<int,GofunParameterData>::iterator it = data()->X_GoFun_Parameter.begin();it != data()->X_GoFun_Parameter.end(); ++it)
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
	if(!data()->X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = data()->X_GoFun_Env.begin(); it != data()->X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			QStringList vk_pair = QStringList::split('=',(*it));
			exec = "export " + vk_pair[0] + "='" + QString((*it)).remove(0,vk_pair[0].length()+1) + "';" + exec;
		}
	}
	if((!option->terminal.isEmpty()) || (data()->Terminal == "true"))
	{ 
		addSplittedProcArgument(&proc,GSC::get()->terminal_cmd);
		if(exec[exec.length()-1] == ';')
			exec.setLength(exec.length()-1);
		exec += ";echo -e \"\\E[${2:-44};${3:-7}m\n" + QObject::tr("End of execution has been reached. Press any key to remove this terminal\";");
		exec += "read -n 1";
		
	}
	if(data()->Path.isEmpty())
	{
		proc.setWorkingDirectory(QDir::homeDirPath());
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		exec = "cd " + GofunMisc::shellify_path(QDir::homeDirPath()) + ";" + exec;
		proc.addArgument(exec);
	}
	else
	{
		proc.setWorkingDirectory(QDir(data()->Path));
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		exec = "cd " + GofunMisc::shellify_path(data()->Path) + ";" + exec;
		proc.addArgument(exec);
	}
	if(!data()->X_GoFun_User.isEmpty())
	{		
		QString spa_file = saveProcArguments(&proc);
		
		QProcess proc_gosu;
		proc_gosu.addArgument("gosu");
		proc_gosu.addArgument(data()->X_GoFun_User);
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
		if((!option->xinit.isEmpty()) || (data()->X_GoFun_NewX == "true"))
			proc.addArgument("-xstart");
		proc.start();
	}
}

void GofunApplicationItem::addSplittedProcArgument(QProcess* proc,const QString& argument)
{
	QStringList arguments = QStringList::split(' ',argument);
	for(QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		proc->addArgument((*it));
	}
}

QString GofunApplicationItem::saveProcArguments(QProcess* proc)
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

//Open the (working)-directory of a Desktop Entry in a file-manager.
void GofunApplicationItem::openDirectory()
{
	QProcess proc(GSC::get()->filemanager_cmd);
	if(!data()->Path.isEmpty())
		proc.addArgument((GofunMisc::ext_filestring(data()->Path)));
	else
		proc.addArgument(QDir::homeDirPath());
	if(!proc.start())
	{
		std::cout<<QObject::tr("Execution of directory viewer failed. :(\n");
	}	
}

//Show the costumized-start-dialog for the item
void GofunApplicationItem::costumizedStart()
{
	GofunCostumStart* cstart_widget = new GofunCostumStart();
	GofunMisc::attach_window(qApp->mainWidget(),cstart_widget,D_Left,D_Right,375,200);
	cstart_widget->load(this);
	cstart_widget->show();
}

QPopupMenu* GofunApplicationItem::rightClickPopup(const QPoint& pos)
{
	QPopupMenu* popup = GofunItem::rightClickPopup(pos);
		
	popup->insertItem(tr("Start"),PID_Execute,0);
	popup->insertSeparator(1);
	popup->insertItem(tr("\" in Terminal"),PID_Execute_in_terminal,2);
	popup->insertItem(tr("Open directory"),PID_Open_directory,3);
	popup->insertItem(tr("Start in new XServer"),PID_Execute_with_xinit,4);
	popup->insertItem(tr("Customized start"),PID_Costumized_start,5);

	popup->popup(pos);
	
	return popup;
}

void GofunApplicationItem::popupActivated(int id)
{
	GofunItem::popupActivated(id);

	//if(GofunItem* item = (GofunItemdynamic_cast<GofunIconView*>(view_ws->visibleWidget()))->currentItem() )
	{
		switch(id)
		{
			case PID_Execute:
				execute(); break;
			case PID_Execute_in_terminal:
				execute( "terminal" ); break;
			case PID_Open_directory:
				openDirectory();
				break;
			case PID_Execute_with_xinit: 
				execute("xinit"); break;
			case PID_Costumized_start: 
				costumizedStart(); break;
		}
	}
}

//Open dialog for editing a Desktop Entry.
void GofunApplicationItem::editEntry()
{
	GofunApplicationItemSettings* settings_dlg = new GofunApplicationItemSettings();
	GofunMisc::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,375,200);
	settings_dlg->setCaption(tr("Edit entry"));
	settings_dlg->load(this);
	settings_dlg->exec();
}

//Forwards execution of a Desktop Entry.
void GofunApplicationItem::execute(const QString& option)
{
	ExecuteOption* eo = new ExecuteOption();
	if(option != QString::null && !option.isEmpty())
	{
		if(option == "terminal")
		{
			eo->terminal = "true";
		}
		if(option == "xinit")
		{
			eo->xinit = "true";
		}
	}
	executeCommand(eo);
}

void GofunApplicationItem::createNewItem(GofunCatButton* cat)
{
	GofunApplicationItemSettings* settings_dlg = new GofunApplicationItemSettings();
	int height = 200;
	GofunMisc::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->setCategory(cat);
	settings_dlg->exec();
	delete settings_dlg;
}


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

#include <qapplication.h>
#include <qinputdialog.h>
#include <qtooltip.h>
#include <qprocess.h>
#include <qpopupmenu.h>

#include "gofun_application_item.h"
#include "gofun_misc.h"
#include "gofun_iconview.h"
#include "gofun_settings.h"
#include "gofun_parameter_prompt.h"
#include "gofun_costum_start.h"
#include "gofun_application_item_settings.h"

GofunApplicationItem::GofunApplicationItem(GofunIconView* iconview, const QString& string) : GofunItem(iconview,string)
{
	m_data = new GofunApplicationEntryData();
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
		if(!data()->Exec.isEmpty())
			stream << "Exec=" << data()->Exec << "\n";
		if(!data()->TryExec.isEmpty())
			stream << "TryExec=" << data()->TryExec << "\n";
		if(!data()->Path.isEmpty())
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
		if(!data()->X_GoFun_User.isEmpty())
			stream << "X-GoFun-User=" << data()->X_GoFun_User << "\n";
		if(!data()->X_GoFun_Parameter.empty())
		{
			for(std::map<int,GofunParameterData>::iterator it = data()->X_GoFun_Parameter.begin(); it != data()->X_GoFun_Parameter.end(); ++it)
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
		stream << data()->Unknownkeys.join("\n") << "\n";
		file.close();
	}
}

void GofunApplicationItem::setData(GofunDesktopEntryData* d)
{
	m_data = dynamic_cast<GofunApplicationEntryData*>(d);
	
	implementData();
	if(!data()->TryExec.stripWhiteSpace().isEmpty() && !QFileInfo(data()->TryExec).isExecutable())
		iconView()->takeItem(this);	
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

void GofunApplicationItem::executeCommand()
{
	data()->execute();
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
	popup->insertItem(tr("\" in new XServer"),PID_Execute_with_xinit,3);
	popup->insertItem(tr("Customized start"),PID_Costumized_start,4);
	if(!data()->Path.isEmpty())
		popup->insertItem(tr("Open directory"),PID_Open_directory,5);

	popup->popup(pos);
	
	return popup;
}

void GofunApplicationItem::popupActivated(int id)
{
	GofunItem::popupActivated(id);

	switch(id)
	{
		case PID_Execute:
			execute(); break;
		case PID_Execute_in_terminal:
			execute( "terminal" ); break;
		case PID_Open_directory:
			openDirectory(); break;
		case PID_Execute_with_xinit: 
			execute("xinit"); break;
		case PID_Costumized_start: 
			costumizedStart(); break;
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
	GofunApplicationEntryData* eo = new GofunApplicationEntryData();
	*eo = *data();
	if(option != QString::null && !option.isEmpty())
	{
		if(option == "terminal")
		{
			eo->Terminal = "true";
		}
		if(option == "xinit")
		{
			eo->X_GoFun_NewX = "true";
		}
	}
	eo->execute();
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


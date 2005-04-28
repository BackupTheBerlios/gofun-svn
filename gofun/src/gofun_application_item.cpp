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
#include "gofun_settings_container.h"
#include "gofun_parameter_prompt.h"
#include "gofun_custom_start.h"
#include "gofun_application_item_settings.h"

GofunApplicationItem::GofunApplicationItem(GofunIconView* iconview, const QString& string) : GofunItem(iconview,string)
{
	m_data = new GofunApplicationEntryData();
}

/*GofunItem::~GofunItem()
{
	delete data;
}*/

void GofunApplicationItem::setData(GofunDesktopEntryData* d)
{
	m_data = dynamic_cast<GofunApplicationEntryData*>(d);
	
	implementData();
	if(!data()->TryExec.stripWhiteSpace().isEmpty() && !QFileInfo(data()->TryExec).isExecutable())
		iconView()->takeItem(this);	
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

//Show the customized-start-dialog for the item
void GofunApplicationItem::customizedStart()
{
	GofunCustomStart* cstart_widget = new GofunCustomStart();
	GofunWindowOperations::attach_window(qApp->mainWidget(),cstart_widget,D_Left,D_Right,375,200);
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
	popup->insertItem(tr("Customized start"),PID_Customized_start,4);
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
		case PID_Customized_start: 
			customizedStart(); break;
	}
}

//Open dialog for editing a Desktop Entry.
void GofunApplicationItem::editEntry()
{
	GofunApplicationItemSettings* settings_dlg = new GofunApplicationItemSettings();
	GofunWindowOperations::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,375,200);
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
	GofunWindowOperations::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->setCategory(cat);
	settings_dlg->setDefaults();
	settings_dlg->exec();
	delete settings_dlg;
}


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

#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
 
#include "gofun_costum_start.h"
#include "gofun_application_item.h"
#include "gofun_data.h"
#include "gofun_misc.h"

GofunCostumStart::GofunCostumStart()
{
	setCaption(tr("Costumized Start"));
	
	QGridLayout* grid = new QGridLayout(this,4,2);
	
	caption = new QLabel(tr("Start "),this);
	QLabel* command_label = new QLabel(tr("Command"),this);
	command = new QLineEdit(this);
	directory = new QLineEdit(this);
	terminal = new QCheckBox(tr("Start in terminal"),this);
	newxserver = new QCheckBox(tr("Start in new X-Server"),this);
	user = new QCheckBox(tr("Start as (user)"),this);
	user_name = new QComboBox(this);
	user_name->insertStringList(QStringList::split('\n',GofunMisc::shell_call("cat /etc/passwd | grep /home/ | sed -e 's/:.*$//'")));
	user_name->insertItem("root");
	
	connect(user,SIGNAL(toggled(bool)),user_name,SLOT(setEnabled(bool)));
	
	QPushButton* start_button = new QPushButton(tr("Start"), this);

	grid->addMultiCellWidget(caption,0,0,0,2);
	grid->addWidget(command_label,1,0);
	grid->addWidget(command,1,1);
	grid->addWidget(new QLabel(tr("Directory"),this),2,0);
	grid->addWidget(directory,2,1);
	grid->addMultiCellWidget(terminal,3,3,0,1);
	grid->addMultiCellWidget(newxserver,4,4,0,1);
	grid->addMultiCellWidget(user,5,5,0,1);
	grid->addWidget(user_name,5,2);
	grid->addWidget(start_button,6,0);
	
	connect(start_button, SIGNAL(clicked()),this, SLOT(start()));
	
	item = 0;
}

void GofunCostumStart::start()
{
	GofunApplicationEntryData eo = *item->data();
	if(terminal->isChecked())
	{
		eo.Terminal = "true";
	}
	if(newxserver->isChecked())
	{
		eo.X_GoFun_NewX  = "true";
	}
	if(user->isChecked())
	{
		eo.X_GoFun_User = user_name->currentText();
	}
	eo.Exec = command->text();
	eo.Path = directory->text();
	item->executeCommand(&eo);
}

void GofunCostumStart::load(GofunApplicationItem* _item)
{
	item = _item;
	caption->setText(item->data()->Name);
	command->setText(item->data()->Exec);
	directory->setText(item->data()->Path);
	if(!item->data()->Icon.isEmpty())
	{
		setIcon(QPixmap(item->data()->Icon));
	}
	terminal->setChecked(GofunMisc::stringToBool(item->data()->Terminal));
	newxserver->setChecked(GofunMisc::stringToBool(item->data()->X_GoFun_NewX));
	if(!item->data()->X_GoFun_User.isEmpty())
	{
		user->setChecked(true);
		user_name->setCurrentText(item->data()->X_GoFun_User);
	}
	else
		user->setChecked(false);
}




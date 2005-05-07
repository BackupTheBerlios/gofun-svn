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
#include <qlabel.h>
#include <qpushbutton.h>
 
#include "gofun_item_wizard.h"
#include "gofun_misc.h"

GofunItemWizard::GofunItemWizard()
{
	setCaption(tr("")+tr("Add Entry Wizard"));
	
	QWidget* widget_start = new QWidget(this);
	addPage(widget_start,"<b>This feature isn't implemented!</b>" + tr("Start"));
	
	
	
	QGridLayout* grid_start = new QGridLayout(widget_start,4,2);
	
	QPushButton* app_select = new QPushButton(tr("Application"),widget_start);
	app_select->setPixmap(GofunMisc::getIcon("default_application.png"));
	app_select->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
	
	connect(app_select,SIGNAL(clicked()),this,SLOT(appSelected()));
	
	QPushButton* dev_select = new QPushButton(tr("Device"),widget_start);
	dev_select->setPixmap(GofunMisc::getIcon("default_device.png"));
	dev_select->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
	
	connect(dev_select,SIGNAL(clicked()),this,SLOT(devSelected()));
	
	QPushButton* link_select = new QPushButton(tr("Link"),widget_start);
	link_select->setPixmap(GofunMisc::getIcon("default_link.png"));
	link_select->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
	
	connect(link_select,SIGNAL(clicked()),this,SLOT(linkSelected()));
	
	grid_start->addMultiCellWidget(new QLabel(tr("Which kind of Desktop Entry do you want to create?"),widget_start),0,0,0,1);
	grid_start->addWidget(app_select,1,0);
	grid_start->addWidget(new QLabel(tr("Create an application entry."),widget_start),1,1);	grid_start->addWidget(dev_select,2,0);
	grid_start->addWidget(new QLabel(tr("Create a device entry."),widget_start),2,1);
	grid_start->addWidget(link_select,3,0);
	grid_start->addWidget(new QLabel(tr("Create a link entry."),widget_start),3,1);
}

void GofunItemWizard::appSelected()
{
	QWidget* widget_command = new QWidget(this);
	addPage(widget_command,tr("Command"));
	setAppropriate(widget_command,true);
	
	QGridLayout* grid_command = new QGridLayout(widget_command);
	
	command = new QLineEdit(widget_command);
	
	grid_command->addWidget(new QLabel(tr("You can either enter the command for the application\nyou want to run or you can try to find it using the CommandBrowser"),widget_command),0,0);
	grid_command->addWidget(command,1,0);
	
	next();
}

void GofunItemWizard::devSelected()
{
}

void GofunItemWizard::linkSelected()
{
}



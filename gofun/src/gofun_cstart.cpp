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
 
#include "gofun_cstart.h"
#include "gofun_item.h"
#include "gofun_data.h"

GofunCStart::GofunCStart()
{
	setCaption(tr("Costumized Start"));
	
	QGridLayout* grid = new QGridLayout(this,4,2);
	
	caption = new QLabel(tr("Start "),this);
	QLabel* command_label = new QLabel(tr("Command"),this);
	command = new QLineEdit(this);
	terminal = new QCheckBox(tr("Start in terminal"),this);
	newxserver = new QCheckBox(tr("Start in new X-Server"),this);
	
	QPushButton* start_button = new QPushButton(tr("Start"), this);

	grid->addMultiCellWidget(caption,0,0,0,2);
	grid->addWidget(command_label,1,0);
	grid->addWidget(command,1,1);
	grid->addMultiCellWidget(terminal,2,2,0,1);
	grid->addMultiCellWidget(newxserver,3,3,0,1);
	grid->addWidget(start_button,4,0);
	
	connect(start_button, SIGNAL(clicked()),this, SLOT(start()));
	
	item = 0;
}

void GofunCStart::start()
{
	ExecuteOption eo;
	if(terminal->isChecked())
	{
		eo.terminal = "true";
	}
	if(newxserver->isChecked())
	{
		eo.xinit  = "true";
	}
	eo.Exec = command->text();
	item->executeCommand(&eo);
}

void GofunCStart::load(GofunItem* _item)
{
	item = _item;
	caption->setText(item->data->Name);
	command->setText(item->data->Exec);
	if(!item->data->Icon.isEmpty())
	{
		setIcon(QPixmap(item->data->Icon));
	}
	if(item->data->Terminal == "true")
	{
		terminal->setChecked(true);
	}
}


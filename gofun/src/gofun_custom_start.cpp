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
#include <qgroupbox.h>
 
#include "gofun_custom_start.h"
#include "gofun_application_item.h"
#include "gofun_misc.h"
#include "gofun_application_item_settings_advanced.h"

GofunCustomStart::GofunCustomStart()
{
	setCaption(tr("Customized Start"));
	
	QGridLayout* grid = new QGridLayout(this,5,2);
	
	icon = new QLabel(this);
	caption = new QLabel(tr("Start "),this);
	QLabel* command_label = new QLabel(tr("Command"),this);
	command = new QLineEdit(this);
	directory = new QLineEdit(this);
	
	QGroupBox* gb_adv = new QGroupBox(1,Qt::Horizontal,this);
	widget_adv = new GofunApplicationEntrySettingsAdvanced(gb_adv);
		
	QPushButton* start_button = new QPushButton(tr("Start"), this);
	QPushButton* cancel_button = new QPushButton(tr("Cancel"), this);

	grid->addWidget(icon,0,0);
	grid->addMultiCellWidget(caption,0,0,1,2);
	grid->addWidget(command_label,1,0);
	grid->addMultiCellWidget(command,1,1,1,2);
	grid->addWidget(new QLabel(tr("Directory"),this),2,0);
	grid->addMultiCellWidget(directory,2,2,1,2);
	grid->addMultiCellWidget(gb_adv,3,3,0,2);
	grid->addWidget(start_button,4,0);
	grid->addWidget(cancel_button,4,2);
	
	grid->setRowStretch(3,1);
	
	connect(start_button, SIGNAL(clicked()),this, SLOT(start()));
	connect(cancel_button, SIGNAL(clicked()),this, SLOT(reject()));
	
	item = 0;
}

void GofunCustomStart::start()
{
	GofunApplicationEntryData eo = *item->data();	
	eo.Exec = command->text();
	eo.Path = directory->text();
	widget_adv->apply(&eo);
	eo.execute();
}

void GofunCustomStart::load(GofunApplicationItem* _item)
{
	item = _item;
	caption->setText(item->data()->Name);
	command->setText(item->data()->Exec);
	directory->setText(item->data()->Path);
	if(!item->data()->Icon.isEmpty())
	{
		QPixmap px = GofunMisc::getIcon(item->data()->Icon,64,64);
		if(!px.isNull())
		{
			setIcon(px);
			icon->setPixmap(px);
		}
	}
	widget_adv->load(_item->data());
}




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

#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
 
#include "gofun_fsdevice_item_settings.h"
#include "gofun_fsdevice_item.h"
#include "gofun_misc.h"
#include "gofun_cat_button.h"

GofunFSDeviceItemSettings::GofunFSDeviceItemSettings()
{	
	QWidget* widget_main = new QWidget(this);	
	QGridLayout* grid = new QGridLayout(widget_main,5,3);
	
	tabwidget->addTab(widget_main,tr("Main"));
		
	caption = new QLineEdit(widget_main);
	icon = new QLineEdit(widget_main);
	icon_button = new QToolButton(widget_main);
	unmount_icon = new QLineEdit(widget_main);
	unmount_icon_button = new QToolButton(widget_main);
	comment = new QLineEdit(widget_main);
	device = new QComboBox(true,widget_main);
	grid->addWidget(new QLabel(tr("Caption"),widget_main),0,0);
	grid->addWidget(caption,0,1);
	grid->addWidget(new QLabel(tr("Device"),widget_main),1,0);
	grid->addWidget(device,1,1);
	grid->addWidget(new QLabel(tr("Icon"),widget_main),2,0);
	grid->addWidget(icon,2,1);
	grid->addWidget(icon_button,2,2);
	grid->addWidget(new QLabel(tr("UnmountIcon"),widget_main),3,0);
	grid->addWidget(unmount_icon,3,1);
	grid->addWidget(unmount_icon_button,3,2);
	grid->addWidget(new QLabel(tr("Comment"),widget_main),4,0);
	grid->addWidget(comment,4,1);
	
	QWidget* widget_advanced = new QWidget(this);
	QGridLayout* grid_adv = new QGridLayout(widget_advanced,3,3);
	
	tabwidget->addTab(widget_advanced,tr("Advanced"));
	
	readonly_chk = new QCheckBox(tr("Use filesystem in ReadOnly mode"),widget_advanced);
	grid_adv->addMultiCellWidget(readonly_chk,0,0,0,2);
	
	connect(icon_button, SIGNAL(clicked()),this, SLOT(iconDialog()));
	
	item = 0;
}

void GofunFSDeviceItemSettings::load(GofunFSDeviceItem* _item)
{
	GofunItemSettings::load(_item);
	
	unmount_icon->setText(data()->UnmountIcon);
	unmount_icon_button->setPixmap(GofunMisc::get_icon(data()->UnmountIcon,32,32));
	
	readonly_chk->setChecked(GofunMisc::stringToBool(data()->ReadOnly));
}

void GofunFSDeviceItemSettings::dirDialog()
{
}

void GofunFSDeviceItemSettings::save()
{
	GofunItemSettings::save();
}

void GofunFSDeviceItemSettings::apply()
{
	if(!item)
		item = new GofunFSDeviceItem(category->iconview,QString(""));
		
	GofunItemSettings::apply();
	
	data()->UnmountIcon = unmount_icon->text();
	data()->ReadOnly = GofunMisc::boolToString(readonly_chk->isChecked());
}

bool GofunFSDeviceItemSettings::inputValid()
{
	if(!GofunItemSettings::inputValid())
		return false;
	else
		return true;
}

void GofunFSDeviceItemSettings::iconDialog()
{
}

GofunFSDeviceItemData* GofunFSDeviceItemSettings::data()
{
	return dynamic_cast<GofunFSDeviceItem*>(item)->data();
}


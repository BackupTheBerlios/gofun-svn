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
 
#include "gofun_link_item_settings.h"
#include "gofun_link_item.h"
#include "gofun_cat_button.h"
#include "gofun_desktop_entry_settings_widget.h"

GofunLinkItemSettings::GofunLinkItemSettings()
{	
	QWidget* widget_main = new QWidget(this);	
	QGridLayout* grid = new QGridLayout(widget_main,5,3);
	
	tabwidget->addTab(widget_main,tr("Main"));
		
	desw = new GofunDesktopEntrySettingsWidget(widget_main);
	url = new QLineEdit(widget_main);
	url_button = new QToolButton(widget_main);
	grid->addMultiCellWidget(desw,0,0,0,2);
	grid->addWidget(new QLabel(tr("URL"),widget_main),1,0);
	grid->addWidget(url,1,1);
	grid->addWidget(url_button,1,2);
	
	connect(desw->icon_button, SIGNAL(clicked()),this, SLOT(iconDialog()));
	
	item = 0;
}

void GofunLinkItemSettings::load(GofunLinkItem* _item)
{
	GofunItemSettings::load(_item);
	
	url->setText(data()->URL);
}

void GofunLinkItemSettings::dirDialog()
{
}

void GofunLinkItemSettings::save()
{
	GofunItemSettings::save();
}

void GofunLinkItemSettings::apply()
{
	if(!item)
		item = new GofunLinkItem(category->iconview,QString(""));
		
	GofunItemSettings::apply();
	
	data()->URL = url->text();
}

bool GofunLinkItemSettings::inputValid()
{
	if(!GofunItemSettings::inputValid())
		return false;
	else
		return true;
}

void GofunLinkItemSettings::iconDialog()
{
}

GofunLinkItemData* GofunLinkItemSettings::data()
{
	return dynamic_cast<GofunLinkItem*>(item)->data();
}

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

GofunLinkItemSettings::GofunLinkItemSettings()
{	
	QWidget* widget_main = new QWidget(this);	
	QGridLayout* grid = new QGridLayout(widget_main,5,3);
	
	tabwidget->addTab(widget_main,"Main");
		
	caption = new QLineEdit(widget_main);
	icon = new QLineEdit(widget_main);
	icon_button = new QToolButton(widget_main);
	comment = new QLineEdit(widget_main);
	grid->addWidget(new QLabel(tr("Caption"),widget_main),0,0);
	grid->addWidget(caption,0,1);
	grid->addWidget(new QLabel(tr("Icon"),widget_main),3,0);
	grid->addWidget(icon,3,1);
	grid->addWidget(icon_button,3,2);
	grid->addWidget(new QLabel(tr("Comment"),widget_main),4,0);
	grid->addWidget(comment,4,1);
	
	connect(icon_button, SIGNAL(clicked()),this, SLOT(iconDialog()));
	
	item = 0;
}

void GofunLinkItemSettings::load(GofunLinkItem* _item)
{
	GofunItemSettings::load(_item);
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

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
 
#include "gofun_link_entry_settings.h"
#include "gofun_link_item.h"
#include "gofun_directory_button.h"
#include "gofun_desktop_entry_settings_widget.h"
#include "gofun_url_composer.h"

GofunLinkEntrySettings::GofunLinkEntrySettings()
{	
	QGridLayout* grid = new QGridLayout(widget_main,3,3);

	url = new QLineEdit(widget_main);
	url_button = new QToolButton(widget_main);
	grid->addMultiCellWidget(desw,0,0,0,2);
	grid->addWidget(new QLabel(tr("URL"),widget_main),1,0);
	grid->addWidget(url,1,1);
	grid->addWidget(url_button,1,2);

	connect(url_button,SIGNAL(clicked()),this, SLOT(urlComposer()));
	
	item = 0;
}

void GofunLinkEntrySettings::urlComposer()
{
	GofunURLComposer* url_composer = new GofunURLComposer;
	url_composer->setStartURL(url->text());
	url_composer->setLinkItem(dynamic_cast<GofunLinkItem*>(item));
	if(url_composer->exec() == QDialog::Accepted)
		url->setText(url_composer->getURL());
}

void GofunLinkEntrySettings::load(GofunLinkEntryData* _item)
{
	GofunDesktopEntrySettings::load(_item);
	
	url->setText(data()->URL);
}

void GofunLinkEntrySettings::dirDialog()
{
}

void GofunLinkEntrySettings::save()
{
	GofunDesktopEntrySettings::save();
}

void GofunLinkEntrySettings::apply()
{
	if(!item)
		item = new GofunLinkEntryData;
		
	GofunDesktopEntrySettings::apply();

	
	if(data()->Type.isEmpty())
		data()->Type = "Link";	

	data()->URL = url->text();
}

bool GofunLinkEntrySettings::inputValid()
{
	if(!GofunDesktopEntrySettings::inputValid())
		return false;
	else
		return true;
}

GofunLinkEntryData* GofunLinkEntrySettings::data()
{
	return dynamic_cast<GofunLinkEntryData*>(item);
}

void GofunLinkEntrySettings::setDefaults()
{
	GofunDesktopEntrySettings::setDefaults();
	
	desw->setIcon("default_link.png");
}


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

#include <qlabel.h>

#include "gofun_cat_settings.h"
 
GofunCatSettings::GofunCatSettings()
{
	setCaption("GoFun Category Settings");
	
	QWidget* widget_main = new QWidget(this);
	QGridLayout* grid = new QGridLayout(widget_main,6,3);

	tabwidget->addTab(widget_main,"Main");
	
	caption = new QLineEdit(widget_main);
	command = new QLineEdit(widget_main);
	background = new QLineEdit(widget_main);
	background_button = new QToolButton(widget_main);
	grid->addWidget(new QLabel("Caption",widget_main),0,0);
	grid->addWidget(caption,0,1);
	grid->addWidget(new QLabel("Command",widget_main),1,0);
	grid->addWidget(command,1,1);
	grid->addWidget(new QLabel("Background",widget_main),2,0);
	grid->addWidget(background,2,1);
	grid->addWidget(background_button,2,2);

	item = 0;	
}

void GofunCatSettings::save()
{
	if(!item)
	{
		file.setName(caption->text() + "/" + ".desktop");
	}
	
	if(file.open(IO_WriteOnly))
	{
		QTextStream stream;
		stream << "[Desktop Entry]\n";
		stream << "Type=Directory\n";
		stream << "Name=" << caption->text() << "\n";
		stream << "Background=" << background->text() << "\n";
		if(item)
		{		
			stream << item->data->Unknownkeys << "\n";
		}
		file.close();
	}
}

void GofunCatSettings::apply()
{
	if(item)
	{

	}
}

void GofunCatSettings::load(GofunCatButton* _item)
{
	item = _item;
	caption->setText(item->text());
	background->setText(item->data->Background);
	background_button->setPixmap(QPixmap(item->data->Background));
}

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

#include <qpopupmenu.h>
#include <qcursor.h>
 
#include "gofun_item.h"
#include "gofun_cat_button.h"
 
GofunCatButton::GofunCatButton(const QString& str, QWidget* widget) : QPushButton(str,widget)
{
	setText(str);
	setToggleButton(true);	
	setAcceptDrops(true);

	conf_button = new QToolButton(this);
	conf_button->setPalette(QPalette(QColor(int(rand()%256),int(rand()%256),int(rand()%256))));
	connect(conf_button, SIGNAL(clicked()),this, SLOT(catSettings()));
	
	data = new GofunCatData();
}

void GofunCatButton::resizeEvent(QResizeEvent* event)
{
	//conf_button->setGeometry(geometry().width()+5,0,5,geometry().height()-2);	
	int width = 5;
	int height = geometry().height();
	conf_button->setGeometry(geometry().width()-width,0,width,height);
}

void GofunCatButton::catSettings()
{
	GofunCatSettings* settings_dlg = new GofunCatSettings();
	settings_dlg->load(this);
	QWidget* p = dynamic_cast<QWidget*>(parent()->parent());
	settings_dlg->setGeometry(p->geometry().x()+p->width(),p->y(),p->width(),p->height());
	settings_dlg->exec();
	delete settings_dlg;
}

void GofunCatButton::setData(GofunCatData* d)
{
	data = d;
}

void GofunCatButton::setIconView(QIconView* iv)
{
	iconview = iv;
}

void GofunCatButton::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept(QIconDrag::canDecode(event));
}

void GofunCatButton::dropEvent(QDropEvent* event)
{	
	QIconView* itemview = dynamic_cast<QIconView*>(event->source()->parent());
		
	if(itemview)
	{
		if(itemview->currentItem())
		{
			current_item = dynamic_cast<GofunItem*>(itemview->currentItem());
			QPopupMenu* popup = new QPopupMenu(this);
			connect(popup,SIGNAL(activated(int)),this,SLOT(popupItemDnD(int)));
			popup->insertItem("Copy",1);
			popup->insertItem("Move",2);
			popup->popup(QCursor::pos());
		}
	}
}

void GofunCatButton::popupItemDnD(int id)
{
	GofunItem* gi = new GofunItem(iconview, current_item->text());
	GofunItemData* _data = new GofunItemData(*current_item->data);
	switch(id)
	{
		case 1:
			gi->setData(_data);
			gi->data->File = data->Catdir + gi->data->Name + ".desktop";
			gi->save();
			break;
		case 2:
			gi->setData(_data);
			gi->data->File = data->Catdir + gi->data->Name + ".desktop";
			gi->save();
			current_item->deleteEntry();
			break;
	}
}

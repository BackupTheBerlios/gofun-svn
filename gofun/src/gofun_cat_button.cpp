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

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qtooltip.h>

#include "gofun_item.h"
#include "gofun_cat_button.h"
#include "gofun_cat_settings.h"
#include "gofun.h"
#include "gofun_data.h"
#include "gofun_iconview.h"
#include "gofun_misc.h"
#include "gofun_widget.h"
 
GofunCatButton::GofunCatButton(const QString& str, QWidget* widget) : QPushButton(str,widget)
{
	setText(str);
	setToggleButton(true);	
	//Needed so GofunItems can be moved/copied comfortably between categories
	setAcceptDrops(true);

	//Create the appendix of this button
	conf_button = new QToolButton(this);
	conf_button->setPalette(QPalette(QColor(int(rand()%256),int(rand()%256),int(rand()%256))));

	//connect(conf_button, SIGNAL(clicked()),this, SLOT(catSettings()));
	connect(conf_button, SIGNAL(clicked()),this, SLOT(popupConfButton()));
	
	if(!data()->Icon.isEmpty())
	{
		
	}
	
	//Make sure it's being initialized from the start
	m_data = new GofunCatData();
}

GofunCatButton::~GofunCatButton()
{
	//delete data; //FIXME
	
	delete iconview;
}

void GofunCatButton::popupConfButton()
{
	QPopupMenu* popup = new QPopupMenu(this);
	connect(popup,SIGNAL(activated(int)),this,SLOT(popupCBActivated(int)));
	popup->insertItem(tr("Add Entry"),PID_ADD_ENTRY);
	popup->insertItem(tr("Add Entry Wizard"),PID_ADD_ENTRY_WIZARD);
	popup->insertSeparator();
	popup->insertItem(tr("Settings"),PID_SETTINGS);
	popup->popup(QCursor::pos());
	
	emit clicked();
	setOn(true);
}

void GofunCatButton::popupCBActivated(int id)
{
	switch(id)
	{
		case PID_ADD_ENTRY:
			dynamic_cast<GofunWidget*>(qApp->mainWidget())->addEntry();
			break;
		case PID_ADD_ENTRY_WIZARD:
			break;
		case PID_SETTINGS:
			catSettings();
			break;
	}
}

void GofunCatButton::save()
{
	GofunDesktopObject::save();

	QFile file( data()->File );
	if ( file.open( IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "X-GoFun-Background=" << data()->X_GoFun_Background << "\n";
		file.close();
	}
}

//Make sure the right-button is in the right place (!you got the word joke?)
void GofunCatButton::resizeEvent(QResizeEvent* event)
{
	//conf_button->setGeometry(geometry().width()+5,0,5,geometry().height()-2);	
	int width = 16;
	int height = geometry().height();
	setGeometry(geometry().x(),geometry().y(),geometry().width(),geometry().height());
	conf_button->setGeometry(geometry().width()-width,0,width,height);
}

void GofunCatButton::catSettings()
{
	GofunCatSettings* settings_dlg = new GofunCatSettings();
	settings_dlg->load(this);
	QWidget* p = dynamic_cast<QWidget*>(parent()->parent());
	GofunMisc::attach_window(p,settings_dlg,D_Right,D_Left,275,200);
	settings_dlg->exec();
	delete settings_dlg;
}

//Too obvious for a comment, oh ...
void GofunCatButton::setData(GofunCatData* d)
{
	delete m_data;
	m_data = d;
	
	loadIcon();
	if(!data()->Comment.isEmpty())
		QToolTip::add(this,data()->Comment);
}

void GofunCatButton::loadIcon()
{
	QPixmap px = GofunMisc::get_icon(data()->Icon,16,16);
	if(!px.isNull())
		setPixmap(px);
}

//Specify which IconView is related to this CatButton
void GofunCatButton::setIconView(GofunIconView* iv)
{
	iconview = iv;
}

//Drag'n'Drop magic
void GofunCatButton::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept(QIconDrag::canDecode(event));
}

//Drag'n'Drop magic 2nd part
void GofunCatButton::dropEvent(QDropEvent* event)
{	
	GofunIconView* itemview = dynamic_cast<GofunIconView*>(event->source()->parent());
		
	if(itemview) //Did we receive something from an itemview?
	{
		if(itemview->currentItem()) //Let's say an item?
		{
			//Store this information and create a new popup presenting
			//the users options.
			current_item = dynamic_cast<GofunItem*>(itemview->currentItem());
			QPopupMenu* popup = new QPopupMenu(this);
			connect(popup,SIGNAL(activated(int)),this,SLOT(popupItemDnD(int)));
			popup->insertItem(tr("Copy"),PID_COPY_ITEM);
			popup->insertItem(tr("Move"),PID_MOVE_ITEM);
			popup->popup(QCursor::pos());
		}
	}
}

//The popup-menu resulting from a drag'n'drop action
//has been triggered
void GofunCatButton::popupItemDnD(int id)
{
	//Wise man prepare ...
	GofunItem* gi = new GofunItem(iconview, current_item->text());
	GofunItemData* _data = new GofunItemData(*current_item->data());
	
	switch(id)
	{
		case PID_COPY_ITEM: //Make a deep copy
			gi->setData(_data);
			gi->data()->File = data()->Catdir + gi->data()->Name + ".desktop";
			gi->save();
			break;
		case PID_MOVE_ITEM: //Make a deep copy and remove the original
			gi->setData(_data);
			gi->data()->File = data()->Catdir + gi->data()->Name + ".desktop";
			gi->save();
			current_item->deleteEntry();
			break;
	}
}


void GofunCatButton::refreshBackground()
{
		if(!data()->X_GoFun_Background.isEmpty())
			iconview->setPaletteBackgroundPixmap(QPixmap(data()->X_GoFun_Background));	
		else
			iconview->setPaletteBackgroundColor(QApplication::palette().color(QPalette::Active,QColorGroup::Base));
}


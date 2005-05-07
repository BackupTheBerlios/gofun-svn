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

#include <iostream>
 
#include <qapplication.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qtooltip.h>

#include "gofun_item.h"
#include "gofun_directory_button.h"
#include "gofun_directory_entry_settings.h"
#include "gofun_iconview.h"
#include "gofun_misc.h"
#include "gofun_widget.h"
#include "gofun_item_wizard.h"
#include "gofun_application_item.h"
#include "gofun_fsdevice_item.h"
#include "gofun_link_item.h"
 
GofunDirectoryButton::GofunDirectoryButton(const QString& str, QWidget* widget) : QPushButton(str,widget)
{
	setText(str + "   ");
	setToggleButton(true);	
	//Needed so GofunItems can be moved/copied comfortably between directories
	setAcceptDrops(true);

	//Create the appendix of this button
	conf_button = new QToolButton(this);
	
	int _grey;
	if(dynamic_cast<GofunWidget*>(widget->parent()))
		_grey = int((dynamic_cast<GofunWidget*>(widget->parent())->cats_bg->count()*20)%256);
	else
			_grey = 255;
	conf_button->setPalette(QPalette(QColor(_grey,_grey,_grey)));
	
	//connect(conf_button, SIGNAL(clicked()),this, SLOT(catSettings()));
	connect(conf_button, SIGNAL(clicked()),this, SLOT(popupConfButton()));
	
	//For this category we create an IconView
	iconview = new GofunIconView();
		
	//You don't want wild pointers.
	m_data = 0;
	
	if(dynamic_cast<GofunWidget*>(qApp->mainWidget()))
		dynamic_cast<GofunWidget*>(qApp->mainWidget())->insertDirectory(this);
}

GofunDirectoryButton::~GofunDirectoryButton()
{
	//delete m_data; //FIXME: uncommentings this results segfault/fatal glibc error
	delete iconview;
}

void GofunDirectoryButton::popupConfButton()
{
	QPopupMenu* popup = new QPopupMenu(this);
	QPopupMenu* add_popup = new QPopupMenu(this);
	connect(popup,SIGNAL(activated(int)),this,SLOT(popupCBActivated(int)));
	connect(add_popup,SIGNAL(activated(int)),this,SLOT(popupCBActivated(int )));
	
	if(!readonly)
	{
		add_popup->insertItem("Application",PID_Add_Application);
		add_popup->insertItem("Device",PID_Add_Device);
		add_popup->insertItem("Link",PID_Add_Link);
		popup->insertItem(tr("Add Entry"),add_popup);
		popup->insertItem(tr("Add Entry Wizard"),PID_Add_Wizard);
		popup->insertSeparator();
		popup->insertItem(tr("Settings"),PID_SETTINGS);
		popup->insertItem(tr("Delete"),PID_DELETE);
	}
	else
		popup->insertItem(tr("View settings"),PID_SETTINGS);
	popup->popup(QCursor::pos());
	
	makeCurrent();
}

void GofunDirectoryButton::popupCBActivated(int id)
{
	switch(id)
	{
		case PID_Add_Application:
			GofunApplicationItem::createNewItem(iconview);
			break;
		case PID_Add_Device:
			GofunFSDeviceItem::createNewItem(iconview);
			break;
		case PID_Add_Link:
			GofunLinkItem::createNewItem(iconview);
			break;
		case PID_Add_Wizard:
			runNewItemWizard();
			break;
		case PID_SETTINGS:
			editEntry();
			break;
		case PID_DELETE:
			deleteEntry();
			break;
	}
}

void GofunDirectoryButton::deleteEntry()
{
	//Kindly warn the user
	if(!(QMessageBox::warning(qApp->mainWidget(),tr("Delete category"),tr("Do you really want to delete this category and all its entries, sir?"), tr("Ok"), tr("Cancel")) == 0))
		return;
	
	deleteEntryFile();
	GofunMisc::shellCall("rm -rf '"+data()->Directorydir+"'");
	delete this;
}

void GofunDirectoryButton::deleteEntryFile()
{
	QFile::remove(data()->File);
}

void GofunDirectoryButton::runNewItemWizard()
{
	GofunItemWizard* wizard = new GofunItemWizard();
	wizard->exec();
}

//Make sure the right-button is in the right place (!you got the word joke?)
void GofunDirectoryButton::resizeEvent(QResizeEvent* event)
{
	QPushButton::resizeEvent(event);

	/*if(event->oldSize().width() + 16 == event->size().width())
		return;*/
	int width = 16;
	int height = geometry().height();
	/*setGeometry(geometry().x(),geometry().y(),geometry().width()+width,geometry().height());*/
	conf_button->setGeometry(geometry().width()-width,0,width,height);
	
}

void GofunDirectoryButton::catSettings()
{

	/*GofunDirectoryEntrySettings* settings_dlg = new GofunDirectoryEntrySettings();
	settings_dlg->load(this);
	QWidget* p = qApp->mainWidget(); //dynamic_cast<QWidget*>(parent()->parent());
	GofunWindowOperations::attachWindow(p,settings_dlg,D_Right,D_Left,275,200);
	settings_dlg->exec();
	delete settings_dlg;*/
}

//Too obvious for a comment, oh ...
void GofunDirectoryButton::setData(GofunDirectoryEntryData* d)
{
	m_data = d;
	
	if(!data()->Directorydir.isEmpty() && !QFileInfo(data()->Directorydir).isWritable())
		readonly = true;
	else
		readonly = false;

	implementData();
}

void GofunDirectoryButton::setupToolTip()
{
	if(!data()->Comment.isEmpty())
		QToolTip::add(this,data()->Comment);
}

void GofunDirectoryButton::loadIcon()
{
	QPixmap px = GofunMisc::getIcon(data()->Icon,16,16);
	if(!px.isNull())
		conf_button->setPixmap(px);
	else
		conf_button->setPixmap(QPixmap("default_directory.png"));
}

//Specify which IconView is related to this DirectoryButton
void GofunDirectoryButton::setIconView(GofunIconView* iv)
{
	iconview = iv;
}

//Drag'n'Drop magic
void GofunDirectoryButton::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept(QIconDrag::canDecode(event));
}

//Drag'n'Drop magic 2nd part
void GofunDirectoryButton::dropEvent(QDropEvent* event)
{	
	//Did we receive something from a widget in this application?
	//If not, just return.
	if(!event->source())
		return;
	
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
void GofunDirectoryButton::popupItemDnD(int id)
{
	//Wise man prepare ...
	GofunItem* gi;
	GofunDesktopEntryData* _data;
	
	switch(id)
	{
		case PID_COPY_ITEM: //Make a deep copy //FIXME: This is ugly-ladder-style anti-C++ code
			//FIXME:this needs thoughtful fixing
			if(current_item->data()->Type == "Application")
				gi = new GofunApplicationItem(iconview,current_item->data()->Name);
			else if(current_item->data()->Type == "FSDevice")
				gi = new GofunFSDeviceItem(iconview,current_item->data()->Name);
			else if(current_item->data()->Type == "Link")
				gi = new GofunLinkItem(iconview,current_item->data()->Name);
			gi->setData(current_item->data()->makeCopy());
			gi->data()->File = data()->Directorydir + gi->data()->Name + ".desktop";
			gi->data()->save();
			break;
		case PID_MOVE_ITEM: //Move the item into its new iconview. Save the new and delete the old Desktop Entry file.
			current_item->deleteEntryFile();
			current_item->iconView()->takeItem(current_item);
			iconview->insertItem(current_item);
			current_item->data()->File = data()->Directorydir + current_item->data()->Name + ".desktop";
			current_item->data()->save();
			break;
	}
}


void GofunDirectoryButton::refreshBackground()
{
		if(QFile::exists(data()->X_GoFun_Background))
		{
			QPixmap* bg_image = new QPixmap(data()->X_GoFun_Background);
			//iconview->unsetPalette();
			//iconview->setPaletteBackgroundPixmap(*bg_image);
		}
		else if(data()->X_GoFun_Background.isEmpty())
		{
			iconview->setPaletteBackgroundColor(QApplication::palette().color(QPalette::Active,QColorGroup::Base));
		}
		else if(QColor(data()->X_GoFun_Background).isValid())
		{
			iconview->setPaletteBackgroundColor(QColor(data()->X_GoFun_Background));
		}
}

void GofunDirectoryButton::mouseReleaseEvent( QMouseEvent * event )
{
	if (event->button() & RightButton)
		popupConfButton();
	else if(event->button() & MidButton)
		makeCurrent();
		
	QPushButton::mouseReleaseEvent(event);
}

void GofunDirectoryButton::makeCurrent()
{
	emit clicked();
	setOn(true);
}

void GofunDirectoryButton::implementData()
{
	setText(data()->Name + "   ");
	loadIcon();
	setupToolTip();
	refreshBackground();
}

void GofunDirectoryButton::createNewItem(QWidget* parent)
{
	GofunDirectoryEntryData* new_data = new GofunDirectoryEntryData;
	GofunDirectoryEntrySettings* settings_dlg = new GofunDirectoryEntrySettings();
	int height = 200;
	GofunWindowOperations::attachWindow(qApp->mainWidget(),settings_dlg,D_Right,D_Left,275,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->load(new_data);
	settings_dlg->setDefaults();
	if(settings_dlg->exec() == QDialog::Accepted)
	{
		GofunDirectoryButton* new_item = new GofunDirectoryButton(QString(""),parent);/*iconview,QString(""),new_data);*/
		new_item->setData(new_data);
		new_item->implementData();
		new_item->show();
	}
	else
	{
		delete new_data;
		delete settings_dlg;
	}
}

//Open dialog for editing a Desktop Entry.
void GofunDirectoryButton::editEntry()
{
	GofunDirectoryEntrySettings* settings_dlg = new GofunDirectoryEntrySettings();
	GofunWindowOperations::attachWindow(qApp->mainWidget(),settings_dlg,D_Right,D_Left,275,200);
	settings_dlg->setCaption(tr("Edit entry"));
	settings_dlg->load(data());
	if(settings_dlg->exec() == QDialog::Accepted)
		implementData();
}


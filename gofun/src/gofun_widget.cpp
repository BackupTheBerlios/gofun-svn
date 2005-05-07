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
#include <qtextbrowser.h>
#include <qhttp.h>
#include <qcursor.h>
#include <qaccel.h>
#include <qstyle.h>
#include <qobjectlist.h>
 
#include "gofun_iconview.h"
#include "gofun_widget.h" 
#include "gofun_settings.h"
#include "gofun_settings_container.h"
#include "gofun_misc.h"
#include "gofun_data.h"
#include "gofun_cat_button.h"
#include "gofun_item.h"
#include "gofun_item_settings.h"
#include "gofun_cat_settings.h"
#include "gofun_help.h"
#include "gofun_about.h"
#include "gofun_item_wizard.h"
#include "gofun_application_item.h"
#include "gofun_fsdevice_item.h"
#include "gofun_link_item.h"

QPalette GofunWidget::system_palette;
QString GofunWidget::system_style;

class GofunVButtonGroup : public QVButtonGroup
{
	public:
	GofunVButtonGroup(QWidget* widget) : QVButtonGroup(widget)
	{}
	
	private:
	void wheelEvent(QWheelEvent* e)
	{
		QButton* button;
		if(id(selected()) == -1) //selectedId() isn't used because of backwards compatibility to Qt3.1
		{
			if(count() > 1)
				button = find(1);
			else
				button = 0;
		}
		else if(e->delta() < 0)
		{
			if(!(button = find(id(selected())+1)))
				button = find(1);
		}
		else
		{
			if(id(selected()) == 1)
				button = find(count()-1);
			else if(button = find(id(selected())-1))
				;
		}
		if(button)
		{
			(dynamic_cast<GofunDirectoryButton*>(button))->setOn(true);
			dynamic_cast<GofunWidget*>(qApp->mainWidget())->changeDirectory(id(selected()));
		}
	}
};

//The main constructor.
GofunWidget::GofunWidget(WFlags f) : QWidget(0,0,f)
{    
	//Set up some accelerators
	QAccel* a = new QAccel(this);
	a->connectItem(a->insertItem(Key_F),this,SLOT(toggleFullscreen())); 
	
	//Do some layout magic
	QHBoxLayout* hbox = new QHBoxLayout(this);
	QVBoxLayout* vbox = new QVBoxLayout();
	QHBoxLayout* hboxr = new QHBoxLayout();
	QHBoxLayout* hboxlabel = new QHBoxLayout();
    
	//This button group contains the buttons for the different directories
	cats_bg = new GofunVButtonGroup(this);
	cats_bg->setExclusive(true);
	cats_bg->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding,false);
	connect(cats_bg, SIGNAL(clicked(int)),this, SLOT(changeDirectory(int)));
    
	//We 'load' this WidgetStack with GofunIconViews later on
	view_ws = new QWidgetStack(this);
	view_ws->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding,false);
	
	//It should be obvious what that is being
	QPushButton* quit = new QPushButton(tr("Quit"), this, "quit");
	quit->setFont(QFont("Times", 18));
	quit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum,false);
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
	
	//The button marking your way to the settings-dialog
	QToolButton* config = new QToolButton(this);
    	config->setPixmap(QPixmap("config.png"));
	QToolTip::add(config,tr("Settings"));
	connect(config, SIGNAL(clicked()),this, SLOT(openSettingsDlg()));
	
	//A helpful help-button
	QToolButton* help = new QToolButton(this);
	help->setPixmap(QPixmap("help.png"));
	QToolTip::add(help,tr("Help"));
	connect(help, SIGNAL(clicked()),this, SLOT(showHelp()));
	
	//Layout magic, basically we add the WidgetStack and a vertical box to the
	//top horizontal box
	hbox->addWidget(view_ws);
	hbox->addLayout(vbox);
		    
	//Isn't it nice?
	QLabel* gflabel = new QLabel("GoFun",this);
	gflabel->setFont(QFont("Times",18, QFont::Bold));
	gflabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum,0);
	vbox->addLayout(hboxlabel);
	hboxlabel->addWidget(gflabel);
	
	//Common it is ...
	QToolButton* gficon = new QToolButton(this);
	gficon->setFixedWidth(32);
	gficon->setPixmap(QPixmap("gofun.png"));
	QToolTip::add(gficon,tr("About"));
	hboxlabel->addWidget(gficon);
	
	connect(gficon,SIGNAL(clicked()),this,SLOT(showAbout()));
	
	//The last steps in our layout magic
	vbox->addWidget(cats_bg);
	tools_cat = new GofunDirectoryButton("GoTools",this);
	vbox->addWidget(tools_cat);
	QWidget* wid = new QWidget(this);
	wid->setMinimumHeight(5);
	vbox->addWidget(wid);
	vbox->addLayout(hboxr);
	hboxr->addWidget(quit);            
	hboxr->addWidget(config);
	hboxr->addWidget(help);
	
	//This toolbutton shall be used to add directories
    	QToolButton* cat_add = new QToolButton(Qt::DownArrow,cats_bg);
	connect(cat_add, SIGNAL(clicked()),this, SLOT(popupDirectoryAdd()));
	QToolTip::add(cat_add,tr("Click this button to operate on directories"));
	cats_bg->insert(cat_add);   
	
	//Now load the whole GoFun-Data and care for its displaying
	loadData();
}

void GofunWidget::popupCAActivated(int id)
{
	switch(id)
	{
		case PID_Add_Directory:
			addDirectory();
			break;
	}
}

void GofunWidget::popupDirectoryAdd()
{
	QPopupMenu* popup = new QPopupMenu(this);

	connect(popup,SIGNAL(activated(int)),this,SLOT(popupCAActivated(int)));
	popup->insertItem(tr("Add Directory"),PID_Add_Directory);
	int x = pos().x()+cats_bg->pos().x()+cats_bg->find(0)->pos().x()+cats_bg->find(0)->width();
	int y = QCursor::pos().y();
	QPoint pos(x,y);
	popup->popup(pos);
}

void GofunWidget::showAbout()
{
	GofunAbout* ga = new GofunAbout();
	GofunWindowOperations::centerWindow(ga,540,480);
	ga->show();
}

void GofunWidget::showHelp()
{
	GofunHelp* gh = new GofunHelp(Qt::WDestructiveClose);
	GofunWindowOperations::centerWindow(gh,800,600);
	gh->show();
	
}

void GofunWidget::reloadData()
{
	unloadData();
	loadData();
}

void GofunWidget::unloadData()
{
	int cats_bg_count = cats_bg->count();
	for(int i = 1; i < cats_bg_count+1 && cats_bg_count > 1; ++i)
	{
		GofunDirectoryButton* cb = dynamic_cast<GofunDirectoryButton*>(cats_bg->find(i));
		if(cb)
		{
			for(QIconViewItem* item = cb->iconview->firstItem(); item; 0)
			{
				QIconViewItem* next = item->nextItem();
				delete item;
				item = next;
			}
			view_ws->removeWidget(cb->iconview);
			cats_bg->remove(cb);
			delete cb;
		}
	}
	
	view_ws->raiseWidget(tools_cat->iconview);
}

//Load Desktop Entry data
void GofunWidget::loadData()
{
	//We get the data sorted nicely
	std::vector<GofunDirectoryEntryData>* GfDirectoryData = GofunDataLoader::getData();

	//Now we iterate to the category data
	GofunDirectoryButton* cat;
	GofunItem* gi;
	for(std::vector<GofunDirectoryEntryData>::iterator it = GfDirectoryData->begin(); it != GfDirectoryData->end(); ++it)
	{
		//We create a fresh category button, fill it with data and insert it
		//into the category-button-group
		if(it != GfDirectoryData->begin())
		{
			cat = new GofunDirectoryButton((*it).Name, cats_bg);
			if(!dynamic_cast<GofunWidget*>(qApp->mainWidget()))
				insertDirectory(cat);
		}
		else
		{
			if(dynamic_cast<GofunWidget*>(qApp->mainWidget()))
				continue;
			
			cat = tools_cat;
			connectDirectoryIconview(cat);
			connect(cat,SIGNAL(clicked()),this,SLOT(changeToTools()));
			view_ws->addWidget(cat->iconview, 1001);
		}
		cat->setData(&(*it));
			
		//Now we iterate through the actual item-data to create new GofunItems
		for(std::vector<GofunDesktopEntryData*>::iterator sit = (*it).ItemData->begin(); sit != (*it).ItemData->end(); ++sit)
		{
			if(GofunMisc::stringToBool((*sit)->Hidden))
			{
				continue;
			}
			if((*sit)->Type == "Application")
				gi = new GofunApplicationItem(cat->iconview,(*sit)->Name,(*sit));
			else if((*sit)->Type == "FSDevice")
				gi = new GofunFSDeviceItem(cat->iconview,(*sit)->Name,(*sit));
			else if((*sit)->Type == "Link")
				gi = new GofunLinkItem(cat->iconview,(*sit)->Name,(*sit));
		}
		delete (*it).ItemData;
	}
	tools_cat->setOn(true);
	current_cat = tools_cat;
}

void GofunWidget::changeToTools()
{
	if(cats_bg->id(cats_bg->selected()) != -1)
		dynamic_cast<QPushButton*>(cats_bg->find(cats_bg->id(cats_bg->selected())))->setOn(false);
	tools_cat->setOn(true);
	view_ws->raiseWidget(1001);
	current_cat = tools_cat;
}

void GofunWidget::insertDirectory(GofunDirectoryButton* cat)
{
	cats_bg->insert(cat,cats_bg->count()-1);
	cat->show();

	connectDirectoryIconview(cat);
	
	//At last we add the new IconView to the WidgetStack
	view_ws->addWidget(cat->iconview, cats_bg->count()-1);
}

void GofunWidget::connectDirectoryIconview(GofunDirectoryButton* cat)
{
	connect(cat->iconview, SIGNAL(doubleClicked(QIconViewItem*)),this, SLOT(performDefaultActionOnItem(QIconViewItem*)));
	connect(cat->iconview, SIGNAL(returnPressed(QIconViewItem*)),this, SLOT(performDefaultActionOnItem(QIconViewItem*)));	
	connect(cat->iconview, SIGNAL(contextMenuRequested(QIconViewItem*,const QPoint&)),this, SLOT(rightClickedItem(QIconViewItem*,const QPoint&)));
}

void GofunWidget::performDefaultActionOnItem(QIconViewItem* item)
{
	(dynamic_cast<GofunItem*>(item))->performDefaultAction();
}

//Opens the settings dialog.
void GofunWidget::openSettingsDlg()
{
	GofunSettings* settings_dlg = new GofunSettings();
	GofunWindowOperations::attachWindow(this,settings_dlg,D_Under,D_Above,365,220);
	settings_dlg->load();
	settings_dlg->exec();
	delete settings_dlg;
	
}

//Open a dialog for adding a new Directory
void GofunWidget::addDirectory()
{
	GofunDirectoryButton::createNewItem(cats_bg);
}

//Evaluate popup that is shown, when the user did right-clicks on empty space in a left IconView
void GofunWidget::popupMenuSpace(int id)
{
	GofunItemWizard* wizard = new GofunItemWizard();
	switch(id)
	{
		case PID_Add_Application:
			GofunApplicationItem::createNewItem(current_cat->iconview);
			break;
		case PID_Add_Device:
			GofunFSDeviceItem::createNewItem(current_cat->iconview);
			break;
		case PID_Add_Link:
			GofunLinkItem::createNewItem(current_cat->iconview);
			break;
		case PID_Add_Wizard:
			wizard->exec();
			break;
	}
}

//Generate right-click on Desktop Entry popup.
void GofunWidget::rightClickedItem(QIconViewItem* item,const QPoint& pos)
{
	if(item) //Right-clicked on an item.
	{
		dynamic_cast<GofunItem*>(item)->rightClickPopup(pos);
	}
	else //Right-clicked in empty space.
	{
		if(current_cat->isReadOnly())
			return;
	
		QPopupMenu* popup = new QPopupMenu(this);
		QPopupMenu* add_popup = new QPopupMenu(this);
		connect(popup,SIGNAL(activated(int)),this,SLOT(popupMenuSpace(int)));
		connect(add_popup,SIGNAL(activated(int)),this,SLOT(popupMenuSpace(int )));
	
		add_popup->insertItem(tr("Application"),PID_Add_Application);
		add_popup->insertItem(tr("Device"),PID_Add_Device);
		add_popup->insertItem(tr("Link"),PID_Add_Link);
		/*add_popup->insertItem(GofunMisc::getIcon("default_application.png",16,16),tr("Application"),PID_Add_Application);
		add_popup->insertItem(GofunMisc::getIcon("default_device.png",16,16),tr("Device"),PID_Add_Device);
		add_popup->insertItem(GofunMisc::getIcon("default_link.png",16,16),tr("Link"),PID_Add_Link);*/
		popup->insertItem(tr("Add Entry"),add_popup);
		popup->insertItem(tr("Add Entry Wizard"),PID_Add_Wizard);
		popup->popup(pos);
	}
}

//Switch between directories.
void GofunWidget::changeDirectory(int id)
{
	//If id==0 (the add-Directory-button) return
	if(!id)
		return;
	
	//Do what the function is supposed to do
	GofunDirectoryButton* button = dynamic_cast<GofunDirectoryButton*>(cats_bg->find(id));
	current_cat = button;
	
	view_ws->raiseWidget(id);

	GofunMisc::playSound("doublet.wav");
	
	tools_cat->setOn(false);
}

void GofunWidget::applyColorSettings()
{
	QPalette pal;
	if(GSC::get()->color_source == "random")
		pal = QPalette(QColor(int(rand() % 256),int(rand() % 256),int(rand() % 256)));
	else if(GSC::get()->color_source == "custom")
		pal = QPalette(QColor(GSC::get()->custom_color),QColor(GSC::get()->custom_color));
	else if(GSC::get()->color_source == "system")
		pal = system_palette;
		
	qApp->setPalette(pal,true);
	
	GofunWidget* mw = dynamic_cast<GofunWidget*>(qApp->mainWidget());
	if(mw)
	{
		mw->view_ws->setPalette(QPalette(QColor(GSC::get()->custom_color),QColor(GSC::get()->custom_color)));
		const QObjectList* wl = mw->view_ws->queryList("GofunIconView");
		;
		for(QObjectListIt it(*wl); it.current(); ++it)
		{
		if(static_cast<GofunIconView*>(it.current()))
			static_cast<GofunIconView*>(it.current())->setPalette(pal);
		}
		delete wl; 
	}
}

void GofunWidget::toggleFullscreen( )
{
	if(isFullScreen())
		showNormal();
	else
		showFullScreen();
}

void GofunWidget::applyStyleSettings( )
{
	if(!GSC::get()->style.isEmpty())
		qApp->setStyle(GSC::get()->style);
	else if(system_style != qApp->style().name())
		qApp->setStyle(system_style);
}

GofunWidget::~GofunWidget()
{
	delete tools_cat;
}

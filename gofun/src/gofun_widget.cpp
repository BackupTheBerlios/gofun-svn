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
 
#include "gofun_iconview.h"
#include "gofun_widget.h" 
#include "gofun_costum_start.h"
#include "gofun_settings.h"
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

class GofunVButtonGroup : public QVButtonGroup
{
	public:
	GofunVButtonGroup(QWidget* widget) : QVButtonGroup(widget)
	{}
	
	private:
	void wheelEvent(QWheelEvent* e)
	{
		QButton* button;
		if(selectedId() == -1)
		{
			if(count() > 1)
				button = find(1);
			else
				button = 0;
		}
		else if(e->delta() < 0)
		{
			if(!(button = find(selectedId()+1)))
				button = find(1);
		}
		else
		{
			if(selectedId() == 1)
				button = find(count()-1);
			else if(button = find(selectedId()-1))
				;
		}
		if(button)
		{
			(dynamic_cast<GofunCatButton*>(button))->setOn(true);
			dynamic_cast<GofunWidget*>(qApp->mainWidget())->changeCategory(selectedId());
		}
	}
};

//The main constructor.
GofunWidget::GofunWidget()
{    
	//Set up some accelerators
	QAccel* a = new QAccel(this);
	a->connectItem(a->insertItem(Key_F),this,SLOT(toggleFullscreen())); 
	
	//Do some layout magic
	QHBoxLayout* hbox = new QHBoxLayout(this);
	QVBoxLayout* vbox = new QVBoxLayout();
	QHBoxLayout* hboxr = new QHBoxLayout();
	QHBoxLayout* hboxlabel = new QHBoxLayout();
    
	//This button group contains the buttons for the different categories
	cats_bg = new GofunVButtonGroup(this);
	cats_bg->setExclusive(true);
	cats_bg->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding,false);
	connect(cats_bg, SIGNAL(clicked(int)),this, SLOT(changeCategory(int)));
    
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
	gflabel->setFont(QFont("Times",28, QFont::Bold));
	gflabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum,0);
	vbox->addLayout(hboxlabel);
	hboxlabel->addWidget(gflabel);
	
	//Common it is ...
	QToolButton* gficon = new QToolButton(this);
	gficon->setPixmap(QPixmap("gofun.png"));
	QToolTip::add(gficon,tr("About"));
	hboxlabel->addWidget(gficon);
	
	connect(gficon,SIGNAL(clicked()),this,SLOT(showAbout()));
	
	//The last steps in our layout magic
	vbox->addWidget(cats_bg);
	tools_cat = new GofunCatButton("GoTools",this);
	vbox->addWidget(tools_cat);
	QWidget* wid = new QWidget(this);
	wid->setMinimumHeight(5);
	vbox->addWidget(wid);
	vbox->addLayout(hboxr);
	hboxr->addWidget(quit);            
	hboxr->addWidget(config);
	hboxr->addWidget(help);
	
	//This toolbutton shall be used to add categories
    	QToolButton* cat_add = new QToolButton(Qt::DownArrow,cats_bg);
	connect(cat_add, SIGNAL(clicked()),this, SLOT(popupCatAdd()));
	QToolTip::add(cat_add,tr("Click this button to operate on categories"));
	cats_bg->insert(cat_add);   
	
	//Now load the whole GoFun-Data and care for its displaying
	loadData();
}

void GofunWidget::popupCAActivated(int id)
{
	switch(id)
	{
		case PID_Add_Category:
			addCategory();
			break;
	}
}

void GofunWidget::popupCatAdd()
{
	QPopupMenu* popup = new QPopupMenu(this);

	connect(popup,SIGNAL(activated(int)),this,SLOT(popupCAActivated(int)));
	popup->insertItem(tr("Add Category"),PID_Add_Category);
	int x = pos().x()+cats_bg->pos().x()+cats_bg->find(0)->pos().x()+cats_bg->find(0)->width();
	int y = QCursor::pos().y();
	QPoint pos(x,y);
	popup->popup(pos);
}

void GofunWidget::showAbout()
{
	GofunAbout* ga = new GofunAbout();
	GofunMisc::center_window(ga,540,480);
	ga->show();
}

void GofunWidget::showHelp()
{
	GofunHelp* gh = new GofunHelp();
	GofunMisc::center_window(gh,800,600);
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
		GofunCatButton* cb = dynamic_cast<GofunCatButton*>(cats_bg->find(i));
		if(cb)
		{
			
			for(QIconViewItem* item = cb->iconview->firstItem(); item; 0)
			{
				QIconViewItem* next = item->nextItem();
				delete item;
				item = next;
			}
			view_ws->removeWidget(cb->iconview);
		}
		cats_bg->remove(cb);
		delete cb;
	}
	
	view_ws->raiseWidget(tools_cat->iconview);
}

//Load Desktop Entry data
void GofunWidget::loadData()
{
	//We get the data sorted nicely
	std::vector<GofunCatData>* GfCatData = GofunDataLoader::getData();
	
	//Now we iterate to the category data
	int i = 0;
	for(std::vector<GofunCatData>::iterator it = GfCatData->begin(); it != GfCatData->end(); ++it, ++i)
	{
		//We create a fresh category button, fill it with data and insert it
		//into the category-button-group
		GofunCatButton* cat;
		if(i != 0)
		{
			cat = new GofunCatButton((*it).Name, cats_bg);
			if(!dynamic_cast<GofunWidget*>(qApp->mainWidget()))
				insertCategory(cat);
		}
		else
		{
			if(dynamic_cast<GofunWidget*>(qApp->mainWidget()))
				continue;
			
			cat = tools_cat;
			connectCatIconview(cat);
			connect(cat,SIGNAL(clicked()),this,SLOT(changeToTools()));
			view_ws->addWidget(cat->iconview, 1001);
		}
		cat->setData(&(*it));
			
		//Now we iterate through the actual item-data to create new GofunItems
		for(std::vector<GofunItemData*>::iterator sit = (*it).ItemData->begin(); sit != (*it).ItemData->end(); ++sit)
		{
			if((*sit)->Hidden == "true")
			{
				continue;
			}
			GofunItem* gi = dynamic_cast<GofunItem*>((*sit)->GofunDesktopObjectFactory(cat->iconview));
			gi->setData((*sit));
		}
		delete (*it).ItemData;
	}
	tools_cat->setOn(true);
}

void GofunWidget::changeToTools()
{
	if(cats_bg->selectedId() != -1)
		dynamic_cast<QPushButton*>(cats_bg->find(cats_bg->selectedId()))->setOn(false);
	tools_cat->setOn(true);
	view_ws->raiseWidget(1001);
}

void GofunWidget::insertCategory(GofunCatButton* cat)
{
	cats_bg->insert(cat,cats_bg->count()-1);
	cat->show();

	connectCatIconview(cat);
	
	//At last we add the new IconView to the WidgetStack
	view_ws->addWidget(cat->iconview, cats_bg->count()-1);
}

void GofunWidget::connectCatIconview(GofunCatButton* cat)
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
	GofunMisc::attach_window(this,settings_dlg,D_Under,D_Above,365,150);
	settings_dlg->load();
	settings_dlg->exec();
	delete settings_dlg;
	
}

//Open a dialog for adding a new Category
void GofunWidget::addCategory()
{
	GofunCatSettings* settings_dlg = new GofunCatSettings();
	GofunMisc::attach_window(this,settings_dlg,D_Right,D_Left,275,200);
	settings_dlg->exec();
	delete settings_dlg;
}

//Evaluate popup that is shown, when the user did right-clicks on empty space in a left IconView
void GofunWidget::popupMenuSpace(int id)
{
	GofunItemWizard* wizard = new GofunItemWizard();
	switch(id)
	{
		case PID_Add_Application:
			GofunApplicationItem::createNewItem(current_cat);
			break;
		case PID_Add_Device:
			GofunFSDeviceItem::createNewItem(current_cat);
			break;
		case PID_Add_Link:
			GofunLinkItem::createNewItem(current_cat);
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
		QPopupMenu* popup = new QPopupMenu(this);
		QPopupMenu* add_popup = new QPopupMenu(this);
		connect(popup,SIGNAL(activated(int)),this,SLOT(popupMenuSpace(int)));
		connect(add_popup,SIGNAL(activated(int)),this,SLOT(popupMenuSpace(int )));
	
		add_popup->insertItem(tr("Application"),PID_Add_Application);
		add_popup->insertItem(tr("Device"),PID_Add_Device);
		add_popup->insertItem(tr("Link"),PID_Add_Link);
		popup->insertItem(tr("Add Entry"),add_popup);
		popup->insertItem(tr("Add Entry Wizard"),PID_Add_Wizard);
		popup->popup(pos);
	}
}

//Switch between categories.
void GofunWidget::changeCategory(int id)
{
	//If id==0 (the add-Category-button) return
	if(!id)
		return;
	
	//Do what the function is supposed to do
	GofunCatButton* button = dynamic_cast<GofunCatButton*>(cats_bg->find(id));
	current_cat = button;
	
	view_ws->raiseWidget(id);
	if(QSound::isAvailable()) //If sound can be played, we do so.
	{
		QSound::play("doublet.wav");
	}
	
	tools_cat->setOn(false);
}

void GofunWidget::applyColorSettings()
{
  if(GSC::get()->color_source == "random")
  	qApp->setPalette(QPalette(QColor(int(rand() % 256),int(rand() % 256),int(rand() % 256))),true);
  else if(GSC::get()->color_source == "costum")
  	qApp->setPalette(QPalette(QColor(GSC::get()->costum_color)),true);
  else if(GSC::get()->color_source == "system")
  	qApp->setPalette(system_palette,true);
}

void GofunWidget::toggleFullscreen( )
{
	if(isFullScreen())
		showNormal();
	else
		showFullScreen();
}

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
 
#include "gofun_iconview.h"
#include "gofun_widget.h" 
#include "gofun_cstart.h"
#include "gofun_settings.h"
#include "gofun_misc.h"

//The main constructor.
GofunWidget::GofunWidget()
{    
	QHBoxLayout* hbox = new QHBoxLayout(this);
	QVBoxLayout* vbox = new QVBoxLayout();
	QHBoxLayout* hboxr = new QHBoxLayout();
	QHBoxLayout* hboxlabel = new QHBoxLayout();
    
	cats_bg = new QVButtonGroup(this);
	cats_bg->setExclusive(true);
	cats_bg->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding,false);
	connect(cats_bg, SIGNAL(clicked(int)),this, SLOT(changeCategory(int)));
    
	view_ws = new QWidgetStack(this);
	view_ws->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding,false);
	
	QPushButton* quit = new QPushButton("Quit", this, "quit");
	quit->setFont(QFont("Times", 18));
	quit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum,false);
	
	QToolButton* config = new QToolButton(this);
    	config->setPixmap(QPixmap("config.png"));
	connect(config, SIGNAL(clicked()),this, SLOT(openSettingsDlg()));
	
	hbox->addWidget(view_ws);
	hbox->addLayout(vbox);
		    
	QLabel* gflabel = new QLabel("GoFun",this);
	gflabel->setFont(QFont("Times",28, QFont::Bold));
	gflabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum,0);
	vbox->addLayout(hboxlabel);
	hboxlabel->addWidget(gflabel);
	
	QLabel* gficon = new QLabel(this);
	QPixmap* pixmap = new QPixmap("gofun.png");
	gficon->setPixmap(*pixmap);
	hboxlabel->addWidget(gficon);
	
	vbox->addWidget(cats_bg);
	vbox->addLayout(hboxr);
	hboxr->addWidget(quit);            
	hboxr->addWidget(config);
	
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
     
    	QToolButton* cat_add = new QToolButton(Qt::DownArrow,cats_bg);
	connect(cat_add, SIGNAL(clicked()),this, SLOT(addCategory()));
	cats_bg->insert(cat_add);   
	
	loadData();
}

//Load Desktop Entry data
void GofunWidget::loadData()
{
	std::vector<GofunCatData>* GfCatData = GofunDataLoader::getData();
	    
	int i = 0;
	for(std::vector<GofunCatData>::iterator it = GfCatData->begin(); it != GfCatData->end(); ++it, ++i)
	{
		GofunCatButton* cat = new GofunCatButton((*it).Name, cats_bg);
		cat->setData(&(*it));
		cats_bg->insert(cat,i+1);
		GofunIconView* gicv = new GofunIconView();
		gicv->setResizeMode(QIconView::Adjust);
		cat->setIconView(gicv);
		connect(gicv, SIGNAL(doubleClicked(QIconViewItem*)),this, SLOT(executeItem(QIconViewItem*)));
		connect(gicv, SIGNAL(contextMenuRequested(QIconViewItem*,const QPoint&)),this, SLOT(rightClickedItem(QIconViewItem*,const QPoint&)));
		for(std::vector<GofunItemData>::iterator sit = (*it).ItemData->begin(); sit != (*it).ItemData->end(); ++sit)
		{
			if((*sit).Hidden == "true")
			{
				continue;
			}
			GofunItem* gi = new GofunItem(gicv, (*sit).Name);
			gi->setData(&(*sit));
		}
		view_ws->addWidget(gicv, i);
		if(i == 0)
		{
			current_cat = cat;
		}
	} 
}

//Opens the settings dialog.
void GofunWidget::openSettingsDlg()
{
	GofunSettings* settings_dlg = new GofunSettings();
	settings_dlg->setGeometry(this->x(),this->geometry().y()+this->geometry().height(),this->width(),this->height());
	settings_dlg->setIcon(QPixmap("config.png"));
	settings_dlg->exec();
	delete settings_dlg;
	
}

//Open dialog for adding a Desktop Entry into the current category.
void GofunWidget::addEntry()
{
	GofunItemSettings* settings_dlg = new GofunItemSettings();
	int height = 200;
	settings_dlg->setGeometry(this->x(),this->y()-height-(this->geometry().y()-this->y()),this->width(),height);
	settings_dlg->setCaption("Add entry");
	settings_dlg->setCategory(current_cat);
	settings_dlg->exec();
	delete settings_dlg;
}

//Open a dialog for adding a new Category
void GofunWidget::addCategory()
{
	GofunCatSettings* settings_dlg = new GofunCatSettings();
	settings_dlg->setGeometry(geometry().x()+this->width(),this->y(),this->width(),this->height());
	GofunCatButton* cat = new GofunCatButton("New category",cats_bg);
	cats_bg->insert(cat,cats_bg->count()-1);
	settings_dlg->load(cat);
	settings_dlg->exec();
	delete settings_dlg;
	cat->show();
}

//Evaluate popup that is shown, when the user did right-clicks on empty space in a left IconView
void GofunWidget::popupMenuSpace(int id)
{
	switch(id)
	{
		case PID_Add:
			 addEntry();
			 break;
	}
}

//Open dialog for editing a Desktop Entry.
void GofunWidget::editEntry(GofunItem* item)
{
	GofunItemSettings* settings_dlg = new GofunItemSettings();
	int height = 200;
	settings_dlg->setGeometry(this->x(),this->y()-height-(this->geometry().y()-this->y()),this->width(),height);
	settings_dlg->setCaption("Edit entry");
	settings_dlg->load(item);
	settings_dlg->exec();
}

//Evaluate popup that is shown, when the user right-clicks on a Desktop Entry.
void GofunWidget::popupMenuItem(int id)
{
	if((dynamic_cast<GofunIconView*>(view_ws->visibleWidget()))->currentItem() )
	{
		GofunItem* item = dynamic_cast<GofunItem*>(dynamic_cast<GofunIconView*>(view_ws->visibleWidget())->currentItem());
		switch(id)
		{
			case PID_Execute:
				executeItem(item); break;
			case PID_Execute_in_terminal:
				executeItem(item, "terminal" ); break;
			case PID_Open_directory:
				openDirectoryItem(item); break;
			case PID_Edit:
				editEntry(item); break;
			case PID_Execute_with_xinit: 
				executeItem(item,"xinit"); break;
			case PID_Delete: 
				deleteEntry(item); break;
			case PID_Costumized_start: 
				costumizedStart(item); break;
		}
	} 
}

void GofunWidget::costumizedStart(GofunItem* item)
{
	GofunCStart* cstart_widget = new GofunCStart();
	cstart_widget->setGeometry(this->x()-this->width()-(geometry().x()-this->x()),this->y(),this->width(),this->height());
	cstart_widget->load(item);
	cstart_widget->show();
}

//Generate right-click on Desktop Entry popup.
void GofunWidget::rightClickedItem(QIconViewItem* item,const QPoint& pos)
{
	QPopupMenu* popup = new QPopupMenu(this);
	if(item)
	{
		connect(popup,SIGNAL(activated(int)),this,SLOT(popupMenuItem(int)));
		popup->insertItem("Start",PID_Execute);
		popup->insertSeparator();
		popup->insertItem("\" in Terminal",PID_Execute_in_terminal);
		popup->insertItem("Open directory",PID_Open_directory);
		popup->insertItem("Start in new XServer",PID_Execute_with_xinit);
		popup->insertItem("Customized start",PID_Costumized_start);
		popup->insertSeparator();
		popup->insertItem("Edit entry",PID_Edit);
		popup->insertItem("Delete entry",PID_Delete);
		popup->popup(pos);
	}
	else
	{
		connect(popup,SIGNAL(activated(int)),this,SLOT(popupMenuSpace(int)));
		popup->insertItem("Add Entry",PID_Add);
		popup->popup(pos);
	}
}

//Handles removement of a Desktop Entry.
void GofunWidget::deleteEntry(GofunItem* item)
{
	if(QMessageBox::warning(this,"Delete entry","Do you really want to delete this entry, sir?", "Ok", "Cancel") == 0)
	{
		item->deleteEntry();
	}
}

//Forwards execution of a Desktop Entry.
void GofunWidget::executeItem(QIconViewItem* item,const QString& option)
{
	ExecuteOption* eo = new ExecuteOption();
	if(option != QString::null && !option.isEmpty())
	{
		if(option == "terminal")
		{
	  		eo->terminal = "true";
			
		}
		if(option == "xinit")
		{
			eo->xinit = "true";
		}
	}
	dynamic_cast<GofunItem*>(item)->executeCommand(eo);
}

//Open the (working)-directory of a Desktop Entry in a file-manager.
void GofunWidget::openDirectoryItem(GofunItem* item)
{
	QProcess proc(QString("xfe"),this);
	proc.addArgument(GofunMisc::ext_filestring(item->data->Path));
	if(!proc.start())
	{
		std::cout<<"Execution of directory viewer failed. :(\n";
	}	
}

//Switch between categories.
void GofunWidget::changeCategory(int id)
{
	if(!id)
	{
		return;
	}
	
	GofunCatButton* button = dynamic_cast<GofunCatButton*>(cats_bg->find(id));
	current_cat = button;
	
	view_ws->raiseWidget(id-1);
	if(QSound::isAvailable())
	{
		QSound::play("doublet.wav");
	}
}

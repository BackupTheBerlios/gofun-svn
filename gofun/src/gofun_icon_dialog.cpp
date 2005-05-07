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
 
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qiconview.h>
#include <qhbox.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qprogressbar.h>
#include <qfileinfo.h>

#include "gofun_icon_dialog.h"
#include "gofun_misc.h"
#include "gofun_file_dialog.h"

std::vector<GofunIconItemData*> GofunIconDialog::icon_pool;
GofunIconLoad* GofunIconLoad::_instance = NULL;

GofunIconDialog::GofunIconDialog()
{
	setCaption(tr("Select an Icon"));

	grid = new QGridLayout(this,5);
	grid->setMargin(3);
	
	QGroupBox* gb_selected = new QGroupBox(this);
	gb_selected->setColumnLayout(2, Qt::Horizontal );
	gb_selected->layout()->setSpacing( 6 );
	gb_selected->layout()->setMargin( 5 );
	
	icon_preview = new QLabel(tr("Preview: No icon selected"),gb_selected);
	icon_file = new QLabel(gb_selected);
	
	
	QGroupBox* gb_icon_browse = new QGroupBox(this);
	gb_icon_browse->setColumnLayout(2, Qt::Horizontal );
	gb_icon_browse->layout()->setSpacing( 6 );
	gb_icon_browse->layout()->setMargin( 5 );
	new QLabel(tr("Select icon using file-dialog"),gb_icon_browse);
	QPushButton* browse_icon = new QPushButton(tr("Click"),gb_icon_browse);
	
	connect(browse_icon,SIGNAL(clicked()),this,SLOT(browseForIcon()));
	
	QGroupBox* gb_icon_filter = new QGroupBox(this);
	QGridLayout* grid_filter = new QGridLayout(gb_icon_filter);
	grid_filter->setSpacing( 6 );
	grid_filter->setMargin( 5) ;
	filter_edit = new QLineEdit(gb_icon_filter);
	filter_view = new GofunAdjustAbleIconView(gb_icon_filter);
	filter_view->setMinimumHeight(200);
	
	connect(filter_edit,SIGNAL(textChanged(const QString& )), this,SLOT(updateFilterView(const QString&)));
	connect(filter_view,SIGNAL(doubleClicked(QIconViewItem*)),this,SLOT(setSelectedIcon(QIconViewItem*)));
	connect(filter_view,SIGNAL(returnPressed(QIconViewItem*)),this,SLOT(setSelectedIcon(QIconViewItem*)));
	
	grid_filter->addWidget(new QLabel(tr("Filter icons"),gb_icon_filter),0,0);
	grid_filter->addWidget(filter_edit,0,1);
	grid_filter->addMultiCellWidget(filter_view,1,1,0,1);
	

	QHBox* hbox_oc = new QHBox(this);
	QPushButton* ok = new QPushButton(tr("Ok"),hbox_oc);
	QPushButton* cancel = new QPushButton(tr("Cancel"),hbox_oc);
	
	connect(ok,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
		
	grid->addWidget(gb_selected,0,0);
	grid->addWidget(gb_icon_browse,1,0);
	grid->addWidget(gb_icon_filter,2,0);
	grid->addWidget(hbox_oc,4,0);
	
	if(icon_pool.empty())
	{
		GofunIconLoad::get()->setInitiator(this);
		load_progress = new QProgressBar(100,this);
		grid->addWidget(load_progress,3,0);
		GofunIconLoadThread* icon_loader = new GofunIconLoadThread(GofunIconLoad::get());
		icon_loader->start();
	}
	else
	{
		loadFromIconPool();
	}
}

void GofunIconDialog::loadFromIconPool()
{
	for(std::vector<GofunIconItemData*>::iterator it = icon_pool.begin(); it != icon_pool.end(); ++it)
		new GofunIconItem(filter_view,(*it)->text,(*it)->pixmap,(*it)->file);
	updateFilterView(filter_edit->text());
}

GofunIconDialog::~GofunIconDialog()
{
	GofunIconLoad::get()->setInitiator(0);
	delete filter_view;
}

void GofunIconDialog::browseForIcon()
{
	QString file = GofunFileDialog::getOpenImageFileName(start_dir,tr("Pick an icon"),tr("Icons"));
	if(!file.isEmpty())
	{
		icon_file->setText(file);
		icon_preview->setPixmap(QPixmap(file));
	}
}

QString GofunIconDialog::selected()
{
	return icon_file->text();
}

void GofunIconDialog::setSelectedIcon(QIconViewItem* item)
{
	icon_file->setText((dynamic_cast<GofunIconItem*>(item))->file);
	icon_preview->setPixmap(icon_file->text());
}

GofunIconItem::GofunIconItem(QIconView* iv,const QString& text, const QPixmap& pix, const QString& _file) : QIconViewItem(iv,text,pix)
{
	file = _file;
}

void GofunIconLoadThread::run() 
{
	QStringList icon_paths = QStringList::split("\n",GofunMisc::shellCall("find /usr/share/icons /usr/share/pixmaps -path \\*32\\*.png"));
	icon_paths += QStringList::split("\n",GofunMisc::shellCall("find /usr/share/icons /usr/share/pixmaps -maxdepth 1 -path \\*.png"));
	QApplication::postEvent(icon_load,new GofunIconTotalStepsEvent(icon_paths.count()));
	QFileInfo fi;
	for(QStringList::Iterator it = icon_paths.begin(); it != icon_paths.end(); ++it)
	{
		fi.setFile((*it));
		QImage pix((*it));
		if(pix.width() > 48 || pix.height() > 48)
		{
			pix = pix.scale(48,48);
		}
		QApplication::postEvent(icon_load,new GofunIconItemDataEvent(fi.baseName(),pix,(*it)));
		usleep(20);
	}
	
	QApplication::postEvent(icon_load,new GofunIconsLoadedEvent());
}

void GofunIconDialog::customEvent(QCustomEvent* event)
{
        if ( event->type() == static_cast<QEvent::Type>(IconItemEventID) )
	{ 
            GofunIconItemDataEvent* icon_event = dynamic_cast<GofunIconItemDataEvent*>(event);
	    QPixmap pix;
	    pix.convertFromImage(icon_event->data.pixmap);
	    icon_pool.push_back(new GofunIconItemData(icon_event->data.pixmap,icon_event->data.text,icon_event->data.file));
	    GofunIconItem* item = new GofunIconItem(filter_view,icon_event->data.text,icon_event->data.pixmap,icon_event->data.file);
	    if(!(filter_edit->text().isEmpty() || item->text().contains(filter_edit->text(),false) != 0))
	    {
			filter_view->takeItem(item);
			taken_icons.push_back(dynamic_cast<GofunIconItem*>(item));
	    }
	    
	    if(load_progress)
	    	load_progress->setProgress(load_progress->progress()+1);
        }
	else if ( event->type() == static_cast<QEvent::Type>(IconsLoadedEventID) )
	{
		grid->remove(load_progress);
		delete load_progress;
		load_progress = 0;
	}
}

void GofunIconLoad::customEvent(QCustomEvent* event)
{
        if ( event->type() == static_cast<QEvent::Type>(IconItemEventID) )
	{ 
            GofunIconItemDataEvent* icon_event = dynamic_cast<GofunIconItemDataEvent*>(event);
	    QPixmap pix;
	    pix.convertFromImage(icon_event->data.pixmap);
	    GofunIconDialog::icon_pool.push_back(new GofunIconItemData(icon_event->data.pixmap,icon_event->data.text,icon_event->data.file));
	    
	    if(initiator && initiator->load_progress)
	    	initiator->load_progress->setProgress(initiator->load_progress->progress()+1);
        }
	else if ( event->type() == static_cast<QEvent::Type>(IconTotalStepsEventID) )
	{
		GofunIconTotalStepsEvent* icon_event = dynamic_cast<GofunIconTotalStepsEvent*>(event);
		if(initiator)
			initiator->load_progress->setTotalSteps(icon_event->total_steps);
	}	
	else if ( event->type() == static_cast<QEvent::Type>(IconsLoadedEventID) )
	{
		if(initiator)
		{
			initiator->loadFromIconPool();
			initiator->removeProgressBar();
		}
	}
}

void GofunIconDialog::updateFilterView( const QString & filter)
{
	for(std::vector<GofunIconItem*>::iterator it = taken_icons.begin(); it != taken_icons.end(); ++it)
	{;
		if(filter.isEmpty() || (*it)->text().contains(filter,false) != 0)
			filter_view->insertItem((*it));
	}
	QIconViewItem* item;
	QIconViewItem* next;
	for(item = filter_view->firstItem(); item;)
	{
		next = item->nextItem();
		if(!(filter.isEmpty() || item->text().contains(filter,false) != 0))
		{
			filter_view->takeItem(item);
			taken_icons.push_back(dynamic_cast<GofunIconItem*>(item));
		}
		item = next;
	}
	filter_view->adjustMe();
}


GofunAdjustAbleIconView::GofunAdjustAbleIconView(QWidget* parent) : QIconView(parent)
{
	setSpacing(3);
	setResizeMode(QIconView::Adjust);
	setSorting(true);
}

void GofunAdjustAbleIconView::adjustMe()
{
	adjustItems();
}

void GofunIconDialog::setStartDir( const QString & _start_dir)
{
	if(QFile::exists(_start_dir))
		start_dir = _start_dir;
	else
		start_dir = "/usr/share/icons";
}

void GofunIconDialog::setStartIcon( const QString& _icon)
{
		icon_file->setText(_icon);
		icon_preview->setPixmap(GofunMisc::getIcon(_icon));
}

void GofunIconDialog::removeProgressBar( )
{
	if(load_progress)
	{
		grid->remove(load_progress);
		delete load_progress;
		load_progress = 0;
	}
}









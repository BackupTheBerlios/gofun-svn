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
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qheader.h>
#include <qstringlist.h>
 
#include "gofun_executable_browser.h"
#include "gofun_misc.h"

GofunExecutableBrowser::GofunExecutableBrowser()
{
	QGridLayout* grid = new QGridLayout(this);
	
	QGridLayout* grid_browser = new QGridLayout();
	
	filter = new QLineEdit(this);
	cmd_list = new QListView(this);
	cmd_list->addColumn(tr("Commands"));
	cmd_list->header()->hide();
	
	QStringList path = QStringList::split(':',getenv("PATH"));
	path.sort();
	
	QString fill_cmd = "ls ";
	for(QStringList::Iterator it = path.begin(); it != path.end();)
	{
		fill_cmd += (*it) + " ";
		QStringList::Iterator o_it = it;
		while((*o_it) == (*(++it)))
		{}
	}

	fill_cmd += " -N";
	QStringList cmds = QStringList::split('\n',GofunMisc::shell_call(fill_cmd));
	for(QStringList::Iterator it = cmds.begin(); it != cmds.end(); ++it)
		new QListViewItem(cmd_list,(*it));
		
	connect(filter,SIGNAL(textChanged(const QString&)),this,SLOT(updateList(const QString&)));
	connect(cmd_list,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(updateFilter(QListViewItem*)));
	connect(cmd_list,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(accept()));
	connect(cmd_list,SIGNAL(clicked(QListViewItem*)),this,SLOT(updateFilter(QListViewItem*)));
	
	grid_browser->addWidget(filter,0,0);
	grid_browser->addWidget(cmd_list,1,0);
	
	QPushButton* ok_button = new QPushButton(tr("Ok"),this);
	QPushButton* cancel_button = new QPushButton(tr("Cancel"),this);
	
	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
	
	grid->addMultiCellLayout(grid_browser,0,0,0,1);
	grid->addWidget(ok_button,1,0);
	grid->addWidget(cancel_button,1,1);	
}

QString GofunExecutableBrowser::getExecutable()
{
	return filter->text();
}


void GofunExecutableBrowser::updateFilter(QListViewItem* item)
{
	filter->setText(item->text(0));
}

void GofunExecutableBrowser::updateList(const QString& text)
{
	QListViewItem* found_item = cmd_list->findItem(text,0,Qt::BeginsWith);
	if(!found_item)
		found_item = cmd_list->findItem(text,0,Qt::Contains);
	if(found_item)
	{
		cmd_list->setCurrentItem(found_item);
		cmd_list->setContentsPos(0,found_item->itemPos());
	}
}


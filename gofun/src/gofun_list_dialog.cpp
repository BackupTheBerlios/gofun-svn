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
 
#include "gofun_list_dialog.h"

GofunListDialog::GofunListDialog()
{
	list = new QListView(this);
	add_button = new QPushButton(tr("Add"),this);
	rem_button = new QPushButton(tr("Remove"),this);
	apply_button = new QPushButton(tr("Apply"),this);
	cancel_button = new QPushButton(tr("Cancel"),this);
	
	connect(add_button,SIGNAL(clicked()),this,SLOT(add()));
	connect(rem_button,SIGNAL(clicked()),this,SLOT(remove()));
	connect(apply_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
	
	QGridLayout* grid = new QGridLayout(this,3,2);
	grid->addWidget(add_button,0,0);
	grid->addWidget(rem_button,0,1);
	grid->addMultiCellWidget(list,1,1,0,2);
	grid->addWidget(apply_button,2,0);
	grid->addWidget(cancel_button,2,1);
}

void GofunListDialog::add()
{
	new QListViewItem(list,tr("Value"));
}

void GofunListDialog::remove()
{
}




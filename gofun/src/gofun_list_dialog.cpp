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

#include <qstringlist.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
 
#include "gofun_list_dialog.h"

GofunListWidget::GofunListWidget(QWidget* parent) : QWidget(parent)
{
	list = new QListView(this);
	list->addColumn(tr("Values"));
	list->setResizeMode(QListView::AllColumns);
	list->setSorting(-1);
	
	edit = new QLineEdit(this);
	edit->setEnabled(false);
	
	connect(edit,SIGNAL(textChanged(const QString&)),this,SLOT(updateValue(const QString&)));
	connect(list,SIGNAL(selectionChanged()),this,SLOT(updateEdit()));
	
	add_button = new QPushButton(tr("Add"),this);
	rem_button = new QPushButton(tr("Remove"),this);
	up_button = new QPushButton(tr("Up"),this);
	down_button = new QPushButton(tr("Down"),this);
	
	connect(add_button,SIGNAL(clicked()),this,SLOT(add()));
	connect(rem_button,SIGNAL(clicked()),this,SLOT(remove()));
	connect(up_button,SIGNAL(clicked()),this,SLOT(up()));
	connect(down_button,SIGNAL(clicked()),this,SLOT(down()));
	
	QGridLayout* grid = new QGridLayout(this,4,3);
	grid->addWidget(add_button,0,0);
	grid->addWidget(rem_button,0,1);
	grid->addMultiCellWidget(list,1,1,0,2);
	grid->addMultiCellWidget(edit,2,2,0,2);
	
	QGridLayout* right = new QGridLayout(4,1);
	grid->addMultiCellLayout(right,0,4,3,3);
	
	right->addWidget(up_button,1,0);
	right->addWidget(down_button,2,0);
}

void GofunListWidget::setValidator(const QValidator* validator)
{
	edit->setValidator(validator);
}

QStringList GofunListWidget::returnList()
{
	QStringList _list;
	for(QListViewItem* it = list->firstChild(); it != 0; it = it->nextSibling())
	{
		_list += (*it).text(0);
	}
}

void GofunListWidget::fillList(const QStringList& _list)
{
	for(QStringList::ConstIterator it = _list.begin(); it != _list.end(); ++it)
	{
		new QListViewItem(list,(*it));
	}
}

void GofunListWidget::add()
{
	QListViewItem* new_item = new QListViewItem(list,tr("Value"));
}

void GofunListWidget::remove()
{
	if(list->selectedItem())
		delete list->selectedItem();
}

void GofunListWidget::down()
{
    if (!list->currentItem())
        return;
    if (list->currentItem()->nextSibling() == 0)
        return;

    list->currentItem()->moveItem(list->currentItem()->nextSibling()); 
}

void GofunListWidget::up()
{
    if (!list->currentItem())
        return;
    if (list->currentItem() == list->firstChild()) 
        return;
    
    QListViewItem *item = list->firstChild();
    while (item->nextSibling() != list->currentItem())
        item = item->nextSibling();
    item->moveItem( list->currentItem()); 
}

void GofunListWidget::updateValue(const QString& text)
{
	if(!list->selectedItem())
		return;
	
	list->selectedItem()->setText(0,text);
}

void GofunListWidget::updateEdit()
{
	if(!list->selectedItem())
	{
		edit->setEnabled(false);
		edit->setText("");
	}
	else
	{
		edit->setEnabled(true);
		edit->setText(list->selectedItem()->text(0));
	}
}

GofunListDialog::GofunListDialog()
{
	QGridLayout* grid = new QGridLayout(this);
	
	list_widget = new GofunListWidget(this);
	apply_button = new QPushButton(tr("Apply"),this);
	cancel_button = new QPushButton(tr("Cancel"),this);
	
	connect(apply_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
	
	grid->addMultiCellWidget(list_widget,0,0,0,1);
	grid->addWidget(apply_button,0,0);
	grid->addWidget(cancel_button,0,1);
}




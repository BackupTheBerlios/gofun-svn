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
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include "gofun_desktop_entry_settings_widget.h"

GofunDesktopEntrySettingsWidget::GofunDesktopEntrySettingsWidget(QWidget* parent) : QFrame(parent)
{
	setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

	QGridLayout* grid = new QGridLayout(this,3,3);
	grid->layout()->setMargin(3);
	grid->layout()->setSpacing(1);
	
	caption = new QLineEdit(this);
	comment = new QLineEdit(this);
	icon_button = new QToolButton(this);
	icon_button->setFixedWidth(32);
	icon_button->setFixedHeight(32);
	QToolButton* more = new QToolButton(Qt::RightArrow,this);
	more->setFixedHeight(7);
	
	connect(more,SIGNAL(clicked()),this,SLOT(showMoreSettings()));
	
	grid->addWidget(icon_button,0,0);
	grid->addWidget(new QLabel(tr("Caption"),this),0,1);
	grid->addWidget(caption,0,2);
	grid->addWidget(new QLabel(tr("Comment"),this),1,0);
	grid->addMultiCellWidget(comment,1,1,1,2);
	grid->addMultiCellWidget(more,2,2,0,2);
}

void GofunDesktopEntrySettingsWidget::showMoreSettings()
{
	GofunDesktopEntrySettingsMore more_dlg(this);
	more_dlg.exec();
}

GofunDesktopEntrySettingsMore::GofunDesktopEntrySettingsMore(QWidget* parent) : QDialog(parent)
{
	QGridLayout* grid = new QGridLayout(this);
	
	QPushButton* ok_button = new QPushButton(tr("Ok"),this);
	QPushButton* cancel_button = new QPushButton(tr("Cancel"),this);
	
	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
	
	QGridLayout* grid_more = new QGridLayout;
	
	generic_name = new QLineEdit(this);
	
	grid_more->addWidget(new QLabel(tr("Generic name"),this),0,0);
	grid_more->addWidget(generic_name,0,1);
	
	grid->addMultiCellLayout(grid_more,0,0,0,1);
	grid->addWidget(ok_button,1,0);
	grid->addWidget(cancel_button,1,1);
}


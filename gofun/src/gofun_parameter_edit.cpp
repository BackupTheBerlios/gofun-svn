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

#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qcombobox.h>

#include "gofun_parameter_edit.h"
#include "gofun_list_dialog.h"

GofunParameterEdit::GofunParameterEdit()
{
	QGridLayout* grid = new QGridLayout(this);
	QGridLayout* grid_paredit = new QGridLayout();
	
	flag = new QLineEdit(this);
	
	comment = new QLineEdit(this);
	default_value = new QComboBox(this);
	values = new GofunListDialog;
	values->reparent(this,QPoint());

	QPushButton* ok_button = new QPushButton(tr("Ok"),this);
	QPushButton* cancel_button = new QPushButton(tr("Cancel"),this);
	
	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
	
	grid_paredit->addWidget(new QLabel(tr("Flag"),this),0,0);
	grid_paredit->addWidget(flag,0,1);
	grid_paredit->addWidget(new QLabel(tr("Comment"),this),1,0);
	grid_paredit->addWidget(comment,1,1);
	grid_paredit->addWidget(new QLabel(tr("Default"),this),2,0);
	grid_paredit->addWidget(default_value,2,1);
	grid_paredit->addMultiCellWidget(values,3,3,0,1);
	
	grid->addMultiCellLayout(grid_paredit,0,0,0,1);
	grid->addWidget(ok_button,1,0);
	grid->addWidget(cancel_button,1,1);
}

void GofunParameterEdit::setParameterData(const GofunParameterData& _par_data)
{
	par_data = _par_data;
	flag->setText(par_data.Flag);
	comment->setText(par_data.Comment);
}

GofunParameterData GofunParameterEdit::getParameterData()
{
	par_data.Comment = comment->text();
	par_data.Flag = flag->text();

	return par_data;
}



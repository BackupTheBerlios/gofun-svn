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
#include <qgroupbox.h>

#include "gofun_parameter_edit.h"
#include "gofun_list_dialog.h"

GofunParameterEdit::GofunParameterEdit()
{
	QGridLayout* grid = new QGridLayout(this);
	grid->setMargin(3);
	
	QGridLayout* grid_paredit = new QGridLayout();

	QGroupBox* gb_general = new QGroupBox(tr("General"),this);
	gb_general->setColumnLayout(0, Qt::Vertical );
	gb_general->layout()->setSpacing( 6 );
	gb_general->layout()->setMargin( 5 );
	QGridLayout* grid_general = new QGridLayout(gb_general->layout());
	
	flag = new QLineEdit(gb_general);
	comment = new QLineEdit(gb_general);
	
	grid_general->addWidget(new QLabel(tr("Flag"),gb_general),0,0);
	grid_general->addWidget(flag,0,1);
	grid_general->addWidget(new QLabel(tr("Comment"),gb_general),1,0);
	grid_general->addWidget(comment,1,1);
	
	QGroupBox* gb_value = new QGroupBox(tr("Value"),this);
	gb_value->setColumnLayout(0, Qt::Vertical );
	gb_value->layout()->setSpacing( 6 );
	gb_value->layout()->setMargin( 5 );
	
	QGridLayout* grid_value = new QGridLayout(gb_value->layout());
	
	type = new QComboBox(gb_value);
	type->insertItem(tr("String"));
	type->insertItem(tr("Integer"));
	type->insertItem(tr("Decimal"));
	default_value = new QComboBox(gb_value);
	values = new GofunListWidget(gb_value);
	
	grid_value->addWidget(new QLabel(tr("Type:"),gb_value),0,0);
	grid_value->addWidget(type,1,0);
	grid_value->addWidget(values,2,0);
	grid_value->addWidget(new QLabel(tr("Default:"),gb_value),3,0);
	grid_value->addWidget(default_value,4,0);
	
	grid_paredit->addWidget(gb_general,0,0);
	grid_paredit->addWidget(gb_value,1,0);

	QPushButton* ok_button = new QPushButton(tr("Ok"),this);
	QPushButton* cancel_button = new QPushButton(tr("Cancel"),this);
	
	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
		
	grid->addMultiCellLayout(grid_paredit,0,0,0,1);
	grid->addWidget(ok_button,1,0);
	grid->addWidget(cancel_button,1,1);
}

void GofunParameterEdit::setParameterData(const GofunParameterData& _par_data)
{
	par_data = _par_data;
	flag->setText(par_data.Flag);
	comment->setText(par_data.Comment);
	//type->setCurrentText(_par_data
}

GofunParameterData GofunParameterEdit::getParameterData()
{
	par_data.Comment = comment->text();
	par_data.Flag = flag->text();
	par_data.Type = type->currentText();
	par_data.Default_Value = default_value->currentText();

	return par_data;
}



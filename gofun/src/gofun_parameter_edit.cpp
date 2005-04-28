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
#include <qcheckbox.h>
#include <qspinbox.h>

#include "gofun_parameter_edit.h"
#include "gofun_list_dialog.h"
#include "gofun_decimal_spinbox.h"
#include "gofun_parameter_string_edit_widget.h"
#include "gofun_parameter_integer_edit_widget.h"
#include "gofun_parameter_decimal_edit_widget.h"

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
	
	QGroupBox* gb_type = new QGroupBox(tr("Type"),this);
	gb_type->setColumnLayout(0, Qt::Vertical );
	gb_type->layout()->setSpacing( 6 );
	gb_type->layout()->setMargin( 5 );
	
	grid_type = new QGridLayout(gb_type->layout());
	
	type = new QComboBox(gb_type);
	type->insertItem(tr("String"));
	type->insertItem(tr("Integer"));
	type->insertItem(tr("Decimal"));
	
	type_edit = new GofunParameterStringEditWidget(gb_type);
		
	connect(type,SIGNAL(activated(int)),this,SLOT(typeChange(int)));
	
	grid_type->addWidget(type,0,0);
	grid_type->addWidget(type_edit,1,0);
	
	grid_paredit->addWidget(gb_general,0,0);
	grid_paredit->addWidget(gb_type,1,0);
	grid_paredit->setRowStretch(1,1);

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
	if(!par_data.Type.isEmpty())
		type->setCurrentText(par_data.Type);
	typeChange(type->currentItem());
	type_edit->setValues(par_data.Values);
	type_edit->setMinimum(par_data.Minimum);
	type_edit->setMaximum(par_data.Maximum);
	type_edit->setDefaultValue(par_data.Default_Value);
}

GofunParameterData GofunParameterEdit::getParameterData()
{
	par_data.Comment = comment->text();
	par_data.Flag = flag->text();
	par_data.Type = type->currentText();
	par_data.Default_Value = type_edit->defaultValue();
	par_data.Values = type_edit->values();
	par_data.Maximum = type_edit->maximum();
	par_data.Minimum = type_edit->minimum();
	
	return par_data;
}

void GofunParameterEdit::typeChange(int type)
{
	QWidget* edit_parent = dynamic_cast<QWidget*>(type_edit->parent());
	switch(type)
	{
		case 0:
			delete type_edit;
			type_edit = new
			 GofunParameterStringEditWidget(edit_parent);
			grid_type->addWidget(type_edit,1,0);
			type_edit->show();
			break;
		case 1:
			
			delete type_edit;
			type_edit = new
			 GofunParameterIntegerEditWidget(edit_parent);
			grid_type->addWidget(type_edit,1,0);
			type_edit->show();
			break;
		case 2:
			delete type_edit;
			type_edit = new
			 GofunParameterDecimalEditWidget(edit_parent);
			grid_type->addWidget(type_edit,1,0);
			type_edit->show();
			break;
	}
	
}



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

GofunParameterTypeEdit::GofunParameterTypeEdit(QWidget* parent) : QGroupBox(parent)
{
	setColumnLayout(0, Qt::Vertical );
	layout()->setSpacing( 6 );
	layout()->setMargin( 5 );
}

GofunParameterTypeStringEdit::GofunParameterTypeStringEdit(QWidget* parent) : GofunParameterTypeEdit(parent)
{
	setTitle(tr("String"));

	QGridLayout* grid = new QGridLayout(layout());
			
	default_value = new QComboBox(this);
	value_list = new GofunListWidget(this);
	
	connect(value_list,SIGNAL(listChanged()),this,SLOT(updateCombo()));
	
	grid->addWidget(new QLabel(tr("Default:"),this),0,0);
	grid->addWidget(default_value,1,0);
	grid->addWidget(new QLabel(tr("Values:"),this),2,0);
	grid->addWidget(value_list,3,0);
}

QStringList GofunParameterTypeStringEdit::values()
{
	return value_list->returnList();
}

QString GofunParameterTypeStringEdit::defaultValue()
{
	return default_value->currentText();
}

GofunParameterTypeIntegerEdit::GofunParameterTypeIntegerEdit(QWidget* parent) : GofunParameterTypeEdit(parent)
{
	setTitle(tr("Integer"));
	
	QGridLayout* grid = new QGridLayout(layout());
	
	spin_default = new QSpinBox(this);
	set_maximum = new QCheckBox(tr("Set maximum"),this);
	spin_maximum = new QSpinBox(this);	
	set_minimum = new QCheckBox(tr("Set mimimum"),this);
	spin_minimum = new QSpinBox(this);
	
	connect(set_maximum,SIGNAL(toggled(bool)),spin_maximum,SLOT(setEnabled(bool)));
	connect(set_minimum,SIGNAL(toggled(bool)),spin_minimum,SLOT(setEnabled(bool)));
	connect(set_maximum,SIGNAL(toggled(bool)),this,SLOT(minMaxChange(bool)));
	connect(set_minimum,SIGNAL(toggled(bool)),this,SLOT(minMaxChange(bool)));
	connect(spin_maximum,SIGNAL(valueChanged(int)),this,SLOT(minMaxChange(int)));
	connect(spin_minimum,SIGNAL(valueChanged(int)),this,SLOT(minMaxChange(int)));
	
	grid->addWidget(new QLabel(tr("Default"),this),0,0);
	grid->addWidget(spin_default,0,1);
	grid->addWidget(set_minimum,1,0);
	grid->addWidget(spin_minimum,1,1);
	grid->addWidget(set_maximum,2,0);
	grid->addWidget(spin_maximum,2,1);
}

QString GofunParameterTypeIntegerEdit::defaultValue()
{
	return QString::number(spin_default->value());
}

QString GofunParameterTypeIntegerEdit::minimum()
{
	if(set_minimum->isChecked())
		return QString::number(spin_minimum->value());
	else
		return QString();
}

QString GofunParameterTypeIntegerEdit::maximum()
{
	if(set_maximum->isChecked())
		return QString::number(spin_maximum->value());
	else
		return QString();
}

GofunParameterTypeDecimalEdit::GofunParameterTypeDecimalEdit(QWidget* parent) : GofunParameterTypeEdit(parent)
{
	setTitle(tr("Decimal"));
	
	QGridLayout* grid = new QGridLayout(layout());
	
	spin_default = new GofunDecimalSpinBox(this);
	set_maximum = new QCheckBox(tr("Set maximum"),this);
	spin_maximum = new GofunDecimalSpinBox(this);	
	set_minimum = new QCheckBox(tr("Set mimimum"),this);
	spin_minimum = new GofunDecimalSpinBox(this);
	
	connect(set_maximum,SIGNAL(toggled(bool)),spin_maximum,SLOT(setEnabled(bool)));
	connect(set_minimum,SIGNAL(toggled(bool)),spin_minimum,SLOT(setEnabled(bool)));
	connect(set_maximum,SIGNAL(toggled(bool)),this,SLOT(minMaxChange(bool)));
	connect(set_minimum,SIGNAL(toggled(bool)),this,SLOT(minMaxChange(bool)));
	connect(spin_maximum,SIGNAL(valueChanged(int)),this,SLOT(minMaxChange(int)));
	connect(spin_minimum,SIGNAL(valueChanged(int)),this,SLOT(minMaxChange(int)));
	
	grid->addWidget(new QLabel(tr("Default"),this),0,0);
	grid->addWidget(spin_default,0,1);
	grid->addWidget(set_minimum,1,0);
	grid->addWidget(spin_minimum,1,1);
	grid->addWidget(set_maximum,2,0);
	grid->addWidget(spin_maximum,2,1);
}

QString GofunParameterTypeDecimalEdit::defaultValue()
{
	return QString::number(spin_default->value());
}

QString GofunParameterTypeDecimalEdit::minimum()
{
	if(set_minimum->isChecked())
		return QString::number(spin_minimum->value());
	else
		return QString();
}

QString GofunParameterTypeDecimalEdit::maximum()
{
	if(set_maximum->isChecked())
		return QString::number(spin_maximum->value());
	else
		return QString();
}

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
	
	type_edit = new GofunParameterTypeStringEdit(gb_type);
		
	connect(type,SIGNAL(activated(int)),this,SLOT(typeChange(int)));
	
	grid_type->addWidget(type,0,0);
	grid_type->addWidget(type_edit,1,0);
	
	grid_paredit->addWidget(gb_general,0,0);
	grid_paredit->addWidget(gb_type,1,0);

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
	type_edit->setDefaultValue(par_data.Default_Value);
	type_edit->setValues(par_data.Values);
	type_edit->setMinimum(par_data.Minimum);
	type_edit->setMaximum(par_data.Maximum);
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
			 GofunParameterTypeStringEdit(edit_parent);
			grid_type->addWidget(type_edit,1,0);
			type_edit->show();
			break;
		case 1:
			
			delete type_edit;
			type_edit = new
			 GofunParameterTypeIntegerEdit(edit_parent);
			grid_type->addWidget(type_edit,1,0);
			type_edit->show();
			break;
		case 2:
			delete type_edit;
			type_edit = new
			 GofunParameterTypeDecimalEdit(edit_parent);
			grid_type->addWidget(type_edit,1,0);
			type_edit->show();
			break;
	}
}

void GofunParameterTypeStringEdit::setDefaultValue(const QString& _default)
{
	default_value->insertItem(_default);
}

void GofunParameterTypeStringEdit::setValues(const QStringList& _list)
{
	value_list->fillList(_list);
}

void GofunParameterTypeIntegerEdit::setDefaultValue(const QString& _default)
{
	if(!_default.isEmpty())
		spin_default->setValue(_default.toInt());
}

void GofunParameterTypeIntegerEdit::setMinimum(const QString& _min)
{
	if(!_min.isEmpty())
		spin_minimum->setValue(_min.toInt());
	set_minimum->setChecked(!_min.isEmpty());
	spin_minimum->setEnabled(!_min.isEmpty());
}

void GofunParameterTypeIntegerEdit::setMaximum(const QString& _max)
{
	if(!_max.isEmpty())
		spin_maximum->setValue(_max.toInt());
	set_maximum->setChecked(!_max.isEmpty());
	spin_maximum->setEnabled(!_max.isEmpty());
}

void GofunParameterTypeDecimalEdit::setDefaultValue(const QString& _default)
{
	if(!_default.isEmpty())
		spin_default->setValue(_default.toInt());
}

void GofunParameterTypeDecimalEdit::setMinimum(const QString& _min)
{
	if(!_min.isEmpty())
		spin_minimum->setValue(_min.toInt());
	set_minimum->setChecked(!_min.isEmpty());
	spin_minimum->setEnabled(!_min.isEmpty());
}

void GofunParameterTypeDecimalEdit::setMaximum(const QString& _max)
{
	if(!_max.isEmpty())
		spin_maximum->setValue(_max.toInt());
	set_maximum->setChecked(!_max.isEmpty());
	spin_maximum->setEnabled(!_max.isEmpty());
}

void GofunParameterTypeStringEdit::updateCombo()
{
	default_value->clear();
	default_value->insertStringList(value_list->returnList());
}

void GofunParameterTypeIntegerEdit::minMaxChange(int)
{
	if(spin_maximum->isEnabled())
		spin_default->setMaxValue(spin_maximum->value());
	else
	{
		unsigned int i = 0;
		i -= 1;
		i /= 2;
		spin_default->setMaxValue(i);
	}
	
	if(spin_minimum->isEnabled())
		spin_default->setMinValue(spin_minimum->value());
	else
	{
		unsigned int i = 0;
		i -= 1;
		i /= 2;
		i *= (-1);
		spin_default->setMinValue(-1);
	}
}

void GofunParameterTypeIntegerEdit::minMaxChange( bool )
{
	minMaxChange(0);
}

QString GofunDecimalSpinBox::mapValueToText( int value )
{
	if ( value == -1 ) // special case
		return QString( "Auto" );

	return QString( "%1.%2" ).arg( value / 10 ).arg( value % 10 );  // 0.0 to 10.0
}

int GofunDecimalSpinBox::mapTextToValue( bool * ok )
{
	if ( text() == "Auto" ) // special case
		return -1;

	return (int) ( 10 * text().toFloat() ); // 0 to 100
}

void GofunParameterTypeDecimalEdit::minMaxChange( int )
{
	if(spin_maximum->isEnabled())
		spin_default->setMaxValue(spin_maximum->value());
	else
	{
		unsigned int i = 0;
		i -= 1;
		i /= 2;
		spin_default->setMaxValue(i);
	}
	
	if(spin_minimum->isEnabled())
		spin_default->setMinValue(spin_minimum->value());
	else
	{
		unsigned int i = 0;
		i -= 1;
		i /= 2;
		i *= (-1);
		spin_default->setMinValue(-1);
	}
}

void GofunParameterTypeDecimalEdit::minMaxChange( bool )
{
	minMaxChange(0);
}


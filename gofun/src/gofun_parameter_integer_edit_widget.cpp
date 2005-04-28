/***************************************************************************
 *   Copyright (C) 2005 by Tobias Glaesser                                 *
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

#include <climits>
 
#include <qlayout.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlabel.h>

#include "gofun_parameter_integer_edit_widget.h"

GofunParameterIntegerEditWidget::GofunParameterIntegerEditWidget(QWidget* parent) : GofunParameterEditWidget(parent)
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
	grid->setRowStretch(3,1);
}

QString GofunParameterIntegerEditWidget::defaultValue()
{
	return QString::number(spin_default->value());
}

QString GofunParameterIntegerEditWidget::minimum()
{
	if(set_minimum->isChecked())
		return QString::number(spin_minimum->value());
	else
		return QString();
}

QString GofunParameterIntegerEditWidget::maximum()
{
	if(set_maximum->isChecked())
		return QString::number(spin_maximum->value());
	else
		return QString();
}

void GofunParameterIntegerEditWidget::setDefaultValue(const QString& _default)
{
	if(!_default.isEmpty())
		spin_default->setValue(_default.toInt());
}

void GofunParameterIntegerEditWidget::setMinimum(const QString& _min)
{
	if(!_min.isEmpty())
		spin_minimum->setValue(_min.toInt());
	set_minimum->setChecked(!_min.isEmpty());
	spin_minimum->setEnabled(!_min.isEmpty());
}

void GofunParameterIntegerEditWidget::setMaximum(const QString& _max)
{
	if(!_max.isEmpty())
		spin_maximum->setValue(_max.toInt());
	set_maximum->setChecked(!_max.isEmpty());
	spin_maximum->setEnabled(!_max.isEmpty());
}

void GofunParameterIntegerEditWidget::minMaxChange(int)
{
	if(spin_maximum->isEnabled())
		spin_default->setMaxValue(spin_maximum->value());
	else
	{
		spin_default->setMaxValue(INT_MAX);
	}
	
	if(spin_minimum->isEnabled())
		spin_default->setMinValue(spin_minimum->value());
	else
	{
		spin_default->setMinValue(INT_MAX);
	}
}

void GofunParameterIntegerEditWidget::minMaxChange( bool )
{
	minMaxChange(0);
}




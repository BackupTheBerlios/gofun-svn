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

#include <qlayout.h>
#include <qspinbox.h>
#include <qslider.h>
 
#include "gofun_parameter_integer_prompt_widget.h"

GofunParameterIntegerPromptWidget::GofunParameterIntegerPromptWidget(QWidget* parent) : GofunParameterPromptWidget(parent)
{
	QGridLayout* grid = new QGridLayout(this);
	value_spin = new QSpinBox(this);
	
	grid->addWidget(value_spin,0,1);
}

void GofunParameterIntegerPromptWidget::setParameterData(const GofunParameterData& _par_data)
{
	GofunParameterPromptWidget::setParameterData(_par_data);
	
	if(!par_data.Minimum.isEmpty())	
		value_spin->setMinValue(par_data.Minimum.toInt());
	if(!par_data.Maximum.isEmpty())
		value_spin->setMaxValue(par_data.Maximum.toInt());
	if(!par_data.Minimum.isEmpty() && !par_data.Maximum.isEmpty())
	{
		value_slider = new QSlider(Qt::Horizontal,this);
		value_slider->setMinValue(par_data.Minimum.toInt());
		value_slider->setMaxValue(par_data.Maximum.toInt());
		value_slider->setValue(par_data.Default_Value.toInt());
		
		connect(value_slider,SIGNAL(valueChanged(int)),value_spin,SLOT(setValue(int)));
		connect(value_spin,SIGNAL(valueChanged(int)),value_slider,SLOT(setValue(int)));
	}
	
	value_spin->setValue(_par_data.Default_Value.toInt());
}

QString GofunParameterIntegerPromptWidget::returnParameter( )
{
	return par_data.Flag + " " + QString::number(value_spin->value());
}


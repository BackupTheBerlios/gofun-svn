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
 
#include "gofun_parameter_decimal_prompt_widget.h"
#include "gofun_decimal_spinbox.h"

GofunParameterDecimalPromptWidget::GofunParameterDecimalPromptWidget(QWidget* parent) : GofunParameterPromptWidget(parent)
{
	grid = new QGridLayout(this);
	value_spin = new GofunDecimalSpinBox(this);
	value_spin->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	
	grid->addWidget(value_spin,0,1);
}

void GofunParameterDecimalPromptWidget::setParameterData(const GofunParameterData& _par_data)
{
	GofunParameterPromptWidget::setParameterData(_par_data);
	
	if(!par_data.Minimum.isEmpty())	
		value_spin->setMinValue(value_spin->mapTextToValue(par_data.Minimum));
	if(!par_data.Maximum.isEmpty())
		value_spin->setMaxValue(value_spin->mapTextToValue(par_data.Maximum));
	if(!par_data.Minimum.isEmpty() && !par_data.Maximum.isEmpty())
	{
		value_slider = new QSlider(Qt::Horizontal,this);
		value_slider->setMinValue(value_spin->mapTextToValue(par_data.Minimum));
		value_slider->setMaxValue(value_spin->mapTextToValue(par_data.Maximum));
		value_slider->setValue(value_spin->mapTextToValue(par_data.Default_Value));
		
		connect(value_slider,SIGNAL(valueChanged(int)),value_spin,SLOT(setValue(int)));
		connect(value_spin,SIGNAL(valueChanged(int)),value_slider,SLOT(setValue(int)));

		grid->addWidget(value_slider,0,0);
	}
	
	value_spin->setValue(value_spin->mapTextToValue(_par_data.Default_Value));
}

QString GofunParameterDecimalPromptWidget::returnParameter( )
{
	return par_data.Flag + " " + value_spin->mapValueToText(value_spin->value());
}


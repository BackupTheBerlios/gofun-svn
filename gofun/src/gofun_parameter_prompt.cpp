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
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qslider.h>

#include "gofun_parameter_edit.h"
#include "gofun_parameter_prompt.h"

GofunParameterPrompt::GofunParameterPrompt()
{
	setCaption(tr("Set Parameters"));
	
	QGridLayout* top_grid = new QGridLayout(this);
	
	grid = new QGridLayout();
	
	top_grid->addMultiCellWidget(new QLabel(tr("Using this dialog you can set parameters."),this),0,0,0,1);
	top_grid->setSpacing(5);
	top_grid->setMargin(5);
	
	grid->setSpacing(5);
	grid->setMargin(5);
	
	top_grid->addMultiCellLayout(grid,1,1,0,1);
	
	QPushButton* ok = new QPushButton(tr("Ok"),this);
	QPushButton* cancel = new QPushButton(tr("Cancel"),this);
	connect(ok,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	
	top_grid->addWidget(ok,2,0);
	top_grid->addWidget(cancel,2,1);
	
}

void GofunParameterPrompt::addParameter(GofunParameterData* pd)
{
	if(pd->Prompt != "true")
	{
		
		//parameter.push_back(spd);
		return;	
	}
	
	QGroupBox* gb = new QGroupBox(1,Qt::Horizontal,pd->Comment + " ( " + tr("Flag") + ": " + pd->Flag + " )",this);
	
	GofunParameterPromptWidget* ppw;
	if(pd->Type == "String")
		ppw = new GofunParameterStringPromptWidget(gb);
	else if(pd->Type == "Integer")
		ppw = new GofunParameterIntegerPromptWidget(gb);
	else if(pd->Type == "Decimal")
		ppw = new GofunParameterDecimalPromptWidget(gb);
	else
		ppw = new GofunParameterStringPromptWidget(gb);

	ppw->setParameterData(*pd);
	grid->addWidget(gb,grid->numRows(),0);

	parameter.push_back(ppw);
}
 
QString GofunParameterPrompt::parameterString()
{
	QString parstr;
	for(std::vector<GofunParameterPromptWidget*>::iterator it = parameter.begin(); it != parameter.end(); ++it)
	{
		parstr += " " + (*it)->returnParameter();
	}
	return parstr;
}

GofunParameterStringPromptWidget::GofunParameterStringPromptWidget(QWidget* parent) : GofunParameterPromptWidget(parent)
{
	QGridLayout* grid = new QGridLayout(this);
	value = new QComboBox(this);
	value->setEditable(true);
	
	grid->addWidget(value,0,0);
}

void GofunParameterStringPromptWidget::setParameterData(const GofunParameterData& _par_data)
{
	GofunParameterPromptWidget::setParameterData(_par_data);
	
	value->insertStringList(par_data.Values);
	value->setCurrentText(par_data.Default_Value);
}

GofunParameterIntegerPromptWidget::GofunParameterIntegerPromptWidget(QWidget* parent) : GofunParameterPromptWidget(parent)
{
	QGridLayout* grid = new QGridLayout(this);
	value_spin = new QSpinBox(this);
	
	grid->addWidget(value_spin,0,1);
}

GofunParameterDecimalPromptWidget::GofunParameterDecimalPromptWidget(QWidget* parent) : GofunParameterPromptWidget(parent)
{
	QGridLayout* grid = new QGridLayout(this);
	value_spin = new GofunDecimalSpinBox(this);
	
	grid->addWidget(value_spin,0,1);
}

QString GofunParameterStringPromptWidget::returnParameter( )
{
	return par_data.Flag + " " + value->currentText();
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

void GofunParameterDecimalPromptWidget::setParameterData(const GofunParameterData& _par_data)
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

QString GofunParameterDecimalPromptWidget::returnParameter( )
{
	return par_data.Flag + " " + value_spin->mapValueToText(value_spin->value());
}


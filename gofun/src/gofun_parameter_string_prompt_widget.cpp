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
#include <qcombobox.h>
 
#include "gofun_parameter_string_prompt_widget.h"

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

QString GofunParameterStringPromptWidget::returnParameter( )
{
	return par_data.Flag + " " + value->currentText();
}

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
	GofunSmallParameterData spd;
	spd.Flag = pd->Flag;
	spd.Value = pd->Default_Value;
	
	if(pd->Prompt != "true")
	{
		spd.combo = 0;
		parameter.push_back(spd);
		return;	
	}
	
	QGroupBox* gb = new QGroupBox(1,Qt::Horizontal,pd->Comment + " ( " + tr("Flag") + ": " + pd->Flag + " )",this);
	QComboBox* cb = new QComboBox(gb);
	
	grid->addWidget(gb,grid->numRows(),0);
	cb->insertStringList(pd->Values);
	if(!pd->Default_Value.isEmpty())
	{
		cb->setCurrentText(pd->Default_Value);
	}
	else
	{
		if(cb->count() > 0)
		{
			cb->insertItem("");
			cb->setCurrentText("");
		}
	}		
	cb->setEditable(true);

	spd.combo = cb;
	parameter.push_back(spd);
}
 
QString GofunParameterPrompt::parameterString()
{
	QString parstr;
	for(std::vector<GofunSmallParameterData>::iterator it = parameter.begin(); it != parameter.end(); ++it)
	{
		if((*it).combo != 0)
			(*it).Value = (*it).combo->currentText();
		if(!(*it).Value.isEmpty())
		{
			parstr += " " + (*it).Flag;
			parstr += " " + (*it).Value;
		}
	}
	return parstr;
}


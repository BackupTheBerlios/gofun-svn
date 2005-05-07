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
#include <qtable.h>
#include <qpushbutton.h>

#include "gofun_application_entry_settings_parameter.h"
#include "gofun_parameter_edit.h"
#include "gofun_misc.h"

GofunApplicationEntrySettingsParameter::GofunApplicationEntrySettingsParameter(QWidget* parent) : QWidget(parent)
{
	QGridLayout* grid_par = new QGridLayout(this,3,3);
	tb_par = new QTable(0,3,this);
	QHeader* tb_h = tb_par->horizontalHeader();
	tb_h->setLabel(0,tr("Comment"));
	tb_h->setLabel(1,tr("Prompt"));
	tb_h->setLabel(2,"    ");
	tb_par->adjustColumn(0);
	tb_par->adjustColumn(1);
	tb_par->adjustColumn(2);
	tb_par->setColumnReadOnly(0,true);
	tb_par->setColumnStretchable(0,true);
	paradd = new QPushButton(tr("Add"), this);
	parrem = new QPushButton(tr("Remove"), this);	
	parup = new QPushButton(tr("Up"), this);
	pardown = new QPushButton(tr("Down"), this);
	
	grid_par->addMultiCellWidget(tb_par,0,0,0,4);
	grid_par->addWidget(paradd,2,0);
	grid_par->addWidget(parrem,2,1);
	QSpacerItem* parspacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid_par->addItem(parspacer,2,2);
	grid_par->addWidget(parup,2,3);
	grid_par->addWidget(pardown,2,4);
	
	connect(paradd, SIGNAL(clicked()), this, SLOT(addParRow()));
	connect(parrem, SIGNAL(clicked()), this, SLOT(remParRow()));
	connect(parup, SIGNAL(clicked()), this, SLOT(parRowUp()));
	connect(pardown, SIGNAL(clicked()), this, SLOT(parRowDown()));
}

void GofunApplicationEntrySettingsParameter::addParRow()
{
	tb_par->insertRows(tb_par->numRows());
	tb_par->setItem(tb_par->numRows()-1,0,new QTableItem(tb_par,QTableItem::WhenCurrent,""));
	QPushButton* par_edit_bt = new QPushButton(tr("Edit"),tb_par);
	connect(par_edit_bt,SIGNAL(clicked()),this,SLOT(parEditDialog()));

	tb_par->setItem(tb_par->numRows()-1,1,new QCheckTableItem(tb_par,""));
	tb_par->setCellWidget(tb_par->numRows()-1,2,par_edit_bt);
	
	par_data.push_back(GofunParameterData());
}

void GofunApplicationEntrySettingsParameter::parEditDialog()
{
	const QWidget* widget = dynamic_cast<const QWidget*>(sender());
	int y = widget->y() + dynamic_cast<QWidget*>(widget->parent())->y() + tb_par->contentsY();
	int row = tb_par->rowAt(y);
	
	GofunParameterEdit* par_edit = new GofunParameterEdit();
	par_edit->setParameterData(par_data[row]);
	QString tmp_prompt = par_data[row].Prompt;
	if(par_edit->exec() == QDialog::Accepted)
	{
		par_data[row] = par_edit->getParameterData();
		par_data[row].Prompt = tmp_prompt;
		tb_par->item(row,0)->setText(par_data[row].Comment);
		//dynamic_cast<QCheckTableItem*>(tb_par->item(row,1))->setChecked(GofunMisc::stringToBool(par_data[row].Prompt));
		tb_par->updateCell(row,0);
		tb_par->updateCell(row,1);
	}
}

void GofunApplicationEntrySettingsParameter::remParRow()
{
	int cr = tb_par->currentRow();
	tb_par->removeRow(cr);
	
	if(tb_par->numRows() > 1)
	{
		QValueList<GofunParameterData>::Iterator it = par_data.at(cr);
		par_data.erase(it);
	}
	else
		par_data.clear();
}

void GofunApplicationEntrySettingsParameter::parRowUp()
{
	tb_par->setRowMovingEnabled(true);
	int cr = tb_par->currentRow();
	if(cr > 0)
	{
		tb_par->swapRows(cr, cr-1, true);
		par_data.insert(par_data.at(cr-1),par_data[cr]);
		par_data.erase(par_data.at(cr+1));
		tb_par->clearSelection();
		tb_par->selectRow(cr-1);
		tb_par->setSelectionMode(QTable::NoSelection);
		tb_par->setSelectionMode(QTable::SingleRow);
		tb_par->updateContents();
	}
}

void GofunApplicationEntrySettingsParameter::parRowDown()
{
	int cr = tb_par->currentRow();
	if(cr < tb_par->numRows()-1)
	{
		tb_par->swapRows(cr, cr+1, true);
		par_data.insert(par_data.at(cr+2),par_data[cr]);
		par_data.erase(par_data.at(cr));
		tb_par->clearSelection();
		tb_par->selectRow(cr+1);
		tb_par->setSelectionMode(QTable::NoSelection);
		tb_par->setSelectionMode(QTable::SingleRow);
		tb_par->updateContents();
	}
}

void GofunApplicationEntrySettingsParameter::apply(GofunApplicationEntryData* data)
{
	data->X_GoFun_Parameter.clear();

	int i = 0;
	for(QValueList<GofunParameterData>::Iterator it = par_data.begin(); it != par_data.end(); ++it, ++i)
	{
		(*it).Prompt = GofunMisc::boolToString(dynamic_cast<QCheckTableItem*>(tb_par->item(i,1))->isChecked());
		data->X_GoFun_Parameter[i] = (*it);
	}
}

void GofunApplicationEntrySettingsParameter::load(GofunApplicationEntryData* data)
{
	if(!data->X_GoFun_Parameter.empty())
	{
		for(std::map<int,GofunParameterData>::iterator it = data->X_GoFun_Parameter.begin(); it != data->X_GoFun_Parameter.end(); ++it)
		{
			par_data.push_back((*it).second);
		
			tb_par->insertRows(tb_par->numRows());
			
			tb_par->setItem(tb_par->numRows()-1,0,new QTableItem(tb_par,QTableItem::WhenCurrent,(*it).second.Comment));

			tb_par->setItem(tb_par->numRows()-1,1,new QCheckTableItem(tb_par,""));
			dynamic_cast<QCheckTableItem*>(tb_par->item(tb_par->numRows()-1,1))->setChecked(GofunMisc::stringToBool((*it).second.Prompt));
			
			QPushButton* par_edit_bt = new QPushButton(tr("Edit"),tb_par);
			tb_par->setCellWidget(tb_par->numRows()-1,2,par_edit_bt);
			connect(par_edit_bt,SIGNAL(clicked()),this,SLOT(parEditDialog()));
		}
	}
}


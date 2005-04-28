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
#include <qlabel.h>
 
#include "gofun_parameter_string_edit_widget.h"
#include "gofun_list_dialog.h"

GofunParameterStringEditWidget::GofunParameterStringEditWidget(QWidget* parent) : GofunParameterEditWidget(parent)
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

QStringList GofunParameterStringEditWidget::values()
{
	return value_list->returnList();
}

QString GofunParameterStringEditWidget::defaultValue()
{
	return default_value->currentText();
}

void GofunParameterStringEditWidget::setDefaultValue(const QString& _default)
{
	default_value->insertItem(_default);
}

void GofunParameterStringEditWidget::setValues(const QStringList& _list)
{
	value_list->fillList(_list);
}

void GofunParameterStringEditWidget::updateCombo()
{
	default_value->clear();
	default_value->insertStringList(value_list->returnList());
}


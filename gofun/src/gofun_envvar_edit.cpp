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
#include <qlabel.h>
#include <qpushbutton.h>

#include "gofun_envvar_edit.h"
#include "gofun_misc.h"

GofunEnvVarEdit::GofunEnvVarEdit(QWidget* parent) : QDialog(parent,0,1)
{
	setCaption(tr("Edit environment variable"));

	QGridLayout* grid = new QGridLayout(this,2,2);
	grid->layout()->setMargin(3);
	grid->layout()->setSpacing(2);

	QGridLayout* grid_edit = new QGridLayout();
	
	grid_edit->addWidget(new QLabel(tr("Name"),this),0,0);
	name_le = new QLineEdit(this);
	grid_edit->addWidget(name_le,0,1);
	grid_edit->addWidget(new QLabel(tr("Value"),this),1,0);
	value_le = new QLineEdit(this);
	grid_edit->addWidget(value_le,1,1);
	grid_edit->addWidget(new QLabel(tr("Interpreted"),this),2,0);
	GofunInterpretedLineEdit* interpreted_le = new GofunInterpretedLineEdit(this);
	interpreted_le->setReadOnly(true);
	interpreted_le->setEnabled(false);
	grid_edit->addWidget(interpreted_le,2,1);
	grid_edit->setRowStretch(3,1);
	
	QPushButton* ok = new QPushButton(tr("Ok"),this);
	connect(value_le, SIGNAL(textChanged(const QString&)), interpreted_le, SLOT(setText(const QString&)));
	connect(ok, SIGNAL(clicked()),this, SLOT(accept()));
	QPushButton* cancel = new QPushButton(tr("Cancel"),this);
	connect(cancel, SIGNAL(clicked()),this, SLOT(reject()));
	
	grid->addMultiCellLayout(grid_edit,0,0,0,1);
	grid->addWidget(ok,1,0);
	grid->addWidget(cancel,1,1);
}

void GofunEnvVarEdit::setName(const QString& name)
{
	name_le->setText(name);
}

void GofunEnvVarEdit::setValue(const QString& value)
{
	value_le->setText(value);
}

QString GofunEnvVarEdit::getName()
{
	return name_le->text();
}

QString GofunEnvVarEdit::getValue()
{
	return value_le->text();
}

GofunInterpretedLineEdit::GofunInterpretedLineEdit(QWidget* parent) : QLineEdit(parent)
{
}

void GofunInterpretedLineEdit::setText(const QString& text)
{
	QLineEdit::setText(GofunMisc::shellCall("echo -n "+text));
}




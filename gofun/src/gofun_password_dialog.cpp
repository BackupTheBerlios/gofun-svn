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

#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtimer.h>

#include "gofun_password_dialog.h"

GofunPasswordDialog::GofunPasswordDialog(WFlags f) : QDialog(0,0,0,f)
{
	setCaption(tr("Password dialog"));
	setModal(true);

	QGridLayout* grid = new QGridLayout(this);
	grid->layout()->setSpacing(5);
	grid->layout()->setMargin(5);
	
	password = new QLineEdit(this);
	password->setEchoMode(QLineEdit::Password);
	
	QPushButton* ok_button = new QPushButton(tr("Ok"),this);
	QPushButton* cancel_button = new QPushButton(tr("Cancel"),this);
	
	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
	
	description = new QLabel(tr("Enter password:"),this);
	description->setTextFormat(Qt::RichText);
	
	grid->addMultiCellWidget(description,0,0,0,2);
	grid->addMultiCellWidget(password,1,1,0,2);
	QSpacerItem* spacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid->addItem(spacer,2,0);
	grid->addWidget(ok_button,2,1);
	grid->addWidget(cancel_button,2,2);
	
	
}

void GofunPasswordDialog::setUser(const QString& _user)
{
	user = _user;
	description->setText(tr("Enter your <b>"+user+"</b> password:"));
	
}

QString GofunPasswordDialog::getPassword()
{
	return password->text();
}

void GofunPasswordDialog::delay()
{
	password->setEnabled(false);
	QTimer::singleShot(2000,this,SLOT(delayEnd()));
}

void GofunPasswordDialog::delayEnd()
{
	password->setEnabled(true);
}




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

#include <qpushbutton.h>
#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qprocess.h>
 
#include "golauncher_widget.h"
#include "gofun_settings_container.h"

GolauncherWidget::GolauncherWidget()
{
	setGeometry(QApplication::desktop()->screen()->width() - 200,0,200,-1);

	QGridLayout* grid = new QGridLayout(this);
	
	QLabel* header = new QLabel(tr("GoFun - Auxiliary Panel"),this);
	header->setFont(QFont("Times", 18));

	QPushButton* quit = new QPushButton(tr("Quit"),this);
	QPushButton* open_terminal = new QPushButton(tr("Open terminal"),this);
	QPushButton* open_gofun = new QPushButton(tr("Open GoFun"),this);
	
	connect(quit,SIGNAL(clicked()),qApp,SLOT(quit()));
	connect(open_terminal,SIGNAL(clicked()),this,SLOT(openTerminal()));
	connect(open_gofun,SIGNAL(clicked()),this,SLOT(openGofun()));

	grid->addMultiCellWidget(header,0,0,0,1);
	grid->addMultiCellWidget(quit,1,1,0,1);
	grid->addWidget(open_gofun,2,0);
	grid->addWidget(open_terminal,2,1);
}

void GolauncherWidget::openTerminal()
{
	QProcess proc(GSC::get()->terminal_cmd);
	proc.start();	
}

void GolauncherWidget::openGofun()
{
	QProcess proc(QString("gofun"));
	proc.start();
}



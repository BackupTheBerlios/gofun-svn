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

#include <qpushbutton.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qtextedit.h>
#include <qfile.h>
 
#include "gofun_about.h"

GofunAbout::GofunAbout()
{
	setCaption("GoFun About");
	
	QGridLayout* grid = new QGridLayout(this,3,1);
	
	QGroupBox* gb_gpl = new QGroupBox("License",this);
	gb_gpl->setColumnLayout(0, Qt::Vertical );
	gb_gpl->layout()->setSpacing( 6 );
	gb_gpl->layout()->setMargin( 5 );
	QGroupBox* gb_credits = new QGroupBox("Credits",this);
	gb_credits->setColumnLayout(0, Qt::Vertical );
	gb_credits->layout()->setSpacing( 6 );
	gb_credits->layout()->setMargin( 5 );
	
	QGridLayout* grid_gpl = new QGridLayout(gb_gpl->layout(),1,1);
	QGridLayout* grid_credits = new QGridLayout(gb_credits->layout(),1,1);
	
	QTextEdit* t_gpl = new QTextEdit(gb_gpl);
	t_gpl->setReadOnly(true);
	QFile file( "../doc/gofun/COPYING" );
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		t_gpl->setText( stream.read() );
		file.close();
	}
	QTextEdit* t_credits = new QTextEdit(gb_credits);
	t_credits->setReadOnly(true);
	file.setName( "../doc/gofun/AUTHORS" );
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		t_credits->setText( stream.read() );
		file.close();
	}
	
	QPushButton* close = new QPushButton("Close",this);
	connect(close,SIGNAL(clicked()),this,SLOT(hide()));
	
	grid->addWidget(gb_gpl,0,0);
	grid->addWidget(gb_credits,1,0);
	grid->addWidget(close,2,0);
		
	grid_gpl->addWidget(t_gpl,0,0);
	grid_credits->addWidget(t_credits,0,0);
}


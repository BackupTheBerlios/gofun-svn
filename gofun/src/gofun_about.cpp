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
#include <qlabel.h>
#include <qimage.h>
 
#include "gofun_about.h"

GofunAbout::GofunAbout()
{
	setCaption(tr("GoFun About"));
	
	//This is the main grid
	QGridLayout* grid = new QGridLayout(this,4,1);
	grid->setMargin(2);
	
	//This is the 'propaganda' part ;)
	QLabel* left_propaganda = new QLabel(tr("The GNU"),this);
	left_propaganda->setAlignment(Qt::AlignCenter);
	QFont font;
	font.setBold(true);
	left_propaganda->setFont(font);
	QLabel* right_propaganda = new QLabel(tr("Fun Extension"),this);
	right_propaganda->setAlignment(Qt::AlignCenter);
	right_propaganda->setFont(font);
	QLabel* logo_label = new QLabel(this);
	logo_label->setPixmap(QImage("gofun.png").scale(100,100,QImage::ScaleMin));
	logo_label->setAlignment(Qt::AlignHCenter);
	QHBoxLayout* propaganda_layout = new QHBoxLayout;
	propaganda_layout->addWidget(left_propaganda);
	propaganda_layout->addWidget(logo_label);
	propaganda_layout->addWidget(right_propaganda);
	
	//In this group-box licensing information is presented
	QGroupBox* gb_gpl = new QGroupBox(tr("License"),this);
	gb_gpl->setColumnLayout(0, Qt::Vertical );
	gb_gpl->layout()->setSpacing( 6 );
	gb_gpl->layout()->setMargin( 5 );
	
	//In this group-box the credits are shown
	QGroupBox* gb_credits = new QGroupBox(tr("Credits"),this);
	gb_credits->setColumnLayout(0, Qt::Vertical );
	gb_credits->layout()->setSpacing( 6 );
	gb_credits->layout()->setMargin( 5 );
	
	//In this group-box contact and link information is presented
	QGroupBox* gb_cl = new QGroupBox(tr("Contact and links"),this);
	gb_cl->setColumnLayout(0, Qt::Vertical );
	gb_cl->layout()->setSpacing( 6 );
	gb_cl->layout()->setMargin( 5 );
	
	//The following layouts are used inside the group-boxes
	QGridLayout* grid_gpl = new QGridLayout(gb_gpl->layout(),1,1);
	QGridLayout* grid_credits = new QGridLayout(gb_credits->layout(),1,1);
	QGridLayout* grid_cl = new QGridLayout(gb_cl->layout(),2,2);
	
	//Create the text-fields and load the appropriate texts
	QTextEdit* t_gpl = new QTextEdit(gb_gpl);
	t_gpl->setReadOnly(true);
	t_gpl->setText(getFileText("../doc/gofun/COPYING"));
	QTextEdit* t_credits = new QTextEdit(gb_credits);
	t_credits->setReadOnly(true);
	t_credits->setText(getFileText("../doc/gofun/AUTHORS"));
		
	//This is our close-button
	QPushButton* close = new QPushButton(tr("Close"),this);
	connect(close,SIGNAL(clicked()),this,SLOT(hide()));
	
	grid->addLayout(propaganda_layout,0,0);
	grid->addWidget(gb_gpl,1,0);
	grid->addWidget(gb_credits,2,0);
	grid->addWidget(gb_cl,3,0);
	grid->addWidget(close,4,0);
		
	grid_gpl->addWidget(t_gpl,0,0);
	grid_credits->addWidget(t_credits,0,0);
	
	grid_cl->addWidget(new QLabel(tr("Homepage:"),gb_cl),0,0);
	grid_cl->addWidget(new QLabel(" http://gofun.berlios.de/\n",gb_cl),0,1);
	grid_cl->addWidget(new QLabel(tr("E-Mail:"),gb_cl),1,0);
	grid_cl->addWidget(new QLabel(" tobi.web@gmx.de",gb_cl),1,1);
}

QString GofunAbout::getFileText( const QString & filename)
{
	QFile file(filename);
	QString text;
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		text = stream.read();
		file.close();
	}
	return text;
}


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
 
#include "gofun_settings_dlg.h" 

GofunSettingsDlg::GofunSettingsDlg()
{
	//Every config dialog has the same Icon by default
	setIcon(QPixmap("config.png"));	

	//The top grid contains the tabwidget and the buttons ever
	//config dialog shares
	QGridLayout* top_grid = new QGridLayout(this,2,2);
			
	tabwidget = new QTabWidget(this);
	tabwidget->setGeometry(0,50,100,100);
	
	top_grid->addMultiCellWidget(tabwidget,0,0,0,1);
	
	QIconSet::setIconSize(QIconSet::Small,QSize(12,12));
	apply_button = new QPushButton(QIconSet(QPixmap("green_check_12.png"),QIconSet::Small),tr("Apply"),this);
	connect(apply_button,SIGNAL(clicked()),this,SLOT(accept()));
	QPushButton* cancel = new QPushButton(QIconSet(QPixmap("red_cross_12.png"),QIconSet::Small),tr("Cancel"),this);
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	QIconSet::setIconSize(QIconSet::Small,QSize(22,22));

	top_grid->addWidget(apply_button,1,0);
	top_grid->addWidget(cancel,1,1);
	
}

void GofunSettingsDlg::save()
{
	std::cout<<"Not implemented!\n";
}

void GofunSettingsDlg::apply()
{
	std::cout<<"Not implemented!\n";
}

bool GofunSettingsDlg::inputValid()
{
	std::cout<<"Not implemented!\n";
}

void GofunSettingsDlg::accept()
{
	//call inputValid() to validate, if the settings are sufficient
	if(inputValid())
	{
		//if so, we apply those settings and save them
		apply();
		save();
		
		hide();
		
		QDialog::accept();
	}
}

void GofunSettingsDlg::reject()
{
	hide();
	QDialog::reject();
}

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
	QGridLayout* top_grid = new QGridLayout(this,2,2);
	
		
	tabwidget = new QTabWidget(this);
	tabwidget->setGeometry(0,50,100,100);

	
	top_grid->addMultiCellWidget(tabwidget,0,0,0,1);
	
	QPushButton* apply = new QPushButton("Apply",this);
	connect(apply,SIGNAL(clicked()),this,SLOT(accept()));
	QPushButton* cancel = new QPushButton("Cancel",this);
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));

	top_grid->addWidget(apply,1,0);
	top_grid->addWidget(cancel,1,1);
	
}

void GofunSettingsDlg::iconDialog()
{
	/*QString start_dir;
	if(icon->text().isEmpty())
	{
		start_dir = icon->text();
	}
	else if(directory->text().isEmpty())
	{
		start_dir = ext_filestring(directory->text());
	}
	my $file = QFileDialog::getOpenFileName($start_dir,"Icons (*.png *.xpm *.jpg *.bmp *.ico)", this, "Pick icon dialog", "Pick an icon");
	if($file ne "")
	{
		icon->setText($file);
		icon_button->setPixmap(QPixmap($file));
	}*/
}

void GofunSettingsDlg::dirDialog()
{/*
	my $start_dir;
	if(directory->text() ne "")
	{
		$start_dir = ext_filestring(directory->text());
	}
	my $dir = QFileDialog::getExistingDirectory($start_dir, this, "get existing directory", "Choose a directory");
	if($dir ne "")
	{
		directory->setText($dir);
	}*/
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
	if(inputValid())
	{
		apply();
		save();
		hide();
	}
}

void GofunSettingsDlg::reject()
{
	hide();
}

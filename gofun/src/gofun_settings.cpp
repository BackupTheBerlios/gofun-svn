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

#include <qfile.h>
#include <qlabel.h>
#include <qsettings.h>
#include <qdir.h>

#include "gofun_settings.h" 

GofunSettingsContainer* GofunSettingsContainer::_instance = NULL;

GofunSettingsContainer::GofunSettingsContainer()
{
	m_settings = new QSettings(QSettings::Native);
	
	m_settings->insertSearchPath( QSettings::Unix, QDir::homeDirPath() + "/.gofun/");
	
	m_settings->beginGroup("config");
	m_settings->beginGroup("commands");
	terminal_cmd = m_settings->readEntry("terminal");
	m_settings->endGroup();
}

GofunSettingsContainer::~GofunSettingsContainer()
{
	m_settings->writeEntry("/commands/terminal",terminal_cmd);
	m_settings->endGroup();
	delete m_settings;
}

GofunSettings::GofunSettings()
{
	setCaption("GoFun Settings");
	
	QWidget* widget_general = new QWidget(this);
	QGridLayout* grid = new QGridLayout(widget_general,6,2);
	tabwidget->addTab(widget_general,"General");
	
	terminal = new QLineEdit(widget_general);
	directory = new QLineEdit(widget_general);
	grid->addWidget(new QLabel("Terminal command",widget_general),0,0);
	grid->addWidget(terminal,0,1);
	grid->addWidget(new QLabel("Directory",widget_general),2,0);
	grid->addWidget(directory,2,1);
}

void GofunSettings::save()
{
	/*if(!item)
	{
		file = category->directory + "/" + caption->text();
	}
	
	QFile file(my $fh = new FileHandle;
	if($fh->open("> ".file)) {
		print $fh "[Desktop Entry]\n";
		print $fh "Name=".caption->text()."\n";
		print $fh "Exec=".command->text()."\n";
		print $fh "Path=".directory->text()."\n";
		$fh->close;
	}*/
}

bool GofunSettings::inputValid()
{
	return true;
}

void GofunSettings::apply()
{
	GSC::get()->terminal_cmd = terminal->text();
	/*if(item)
	{
		item->setCommand(command->text());
		item->setDirectory(directory->text());
		item->setText(caption->text());
	}
	else
	{
		item = GofunItem(category->iconview);
		applyToItem();
		
	}*/
}

void GofunSettings::load()
{
	/*item = shift;
	caption->setText(item->text());
	command->setText(item->data->Exec);
	directory->setText(item->data->Path);
	file = item->file;*/
	terminal->setText(GSC::get()->terminal_cmd);
}

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

#include <qapplication.h>
#include <qfile.h>
#include <qlabel.h>
#include <qsettings.h>
#include <qdir.h>
#include <qcolordialog.h>

#include "gofun_settings.h" 
#include "gofun_widget.h"
#include "gofun_misc.h"

GofunSettingsContainer* GofunSettingsContainer::_instance = NULL;

GofunSettingsContainer::GofunSettingsContainer()
{
	m_settings = new QSettings(QSettings::Native);
	
	m_settings->insertSearchPath( QSettings::Unix, QDir::homeDirPath() + "/.gofun/");
	
	m_settings->beginGroup("config");
	
	m_settings->beginGroup("general");
	gofun_dir = m_settings->readEntry("datadir");
	m_settings->endGroup();
	
	if(gofun_dir.isEmpty())
		gofun_dir = QDir::homeDirPath() + "/.gofun/data";
	QDir dir;
	if(!dir.exists(gofun_dir))
	{
		GofunMisc::makeDir(gofun_dir);
	}
	
	m_settings->beginGroup("commands");
	terminal_cmd = m_settings->readEntry("terminal");
	filemanager_cmd = m_settings->readEntry("filemanager");
	browser_cmd = m_settings->readEntry("browser");
	m_settings->endGroup();
		
	if(terminal_cmd.isEmpty())
		terminal_cmd = "xterm";
		
	m_settings->beginGroup("lookandfeel");
	color_source = m_settings->readEntry("colorsource");
	costum_color = m_settings->readEntry("costumcolor");
	main_width = m_settings->readEntry("main_width");
	main_height = m_settings->readEntry("main_height");
	main_x = m_settings->readEntry("main_x");
	main_y = m_settings->readEntry("main_y");
	save_main_geom = m_settings->readEntry("save_main_geom");
	m_settings->endGroup();
	
	if(color_source.isEmpty())
		color_source = "random";
		
	if(main_width.isEmpty())
		main_width = "365";
	if(main_height.isEmpty())
		main_height = "240";
	if(main_x.isEmpty())
		main_x = "-1";
	if(main_y.isEmpty())
		main_y = "-1";
	if(save_main_geom.isEmpty())
		save_main_geom = "true";		
}

GofunSettingsContainer::~GofunSettingsContainer()
{
	m_settings->writeEntry("/commands/terminal",terminal_cmd);
	m_settings->writeEntry("/commands/filemanager",filemanager_cmd);
	m_settings->writeEntry("/commands/browser",browser_cmd);
	m_settings->writeEntry("/general/datadir",gofun_dir);
	m_settings->writeEntry("/lookandfeel/colorsource",color_source);
	m_settings->writeEntry("/lookandfeel/costumcolor",costum_color);
	
	if(save_main_geom == "true")
	{
		main_width = QString::number(qApp->mainWidget()->width());
		main_height = QString::number(qApp->mainWidget()->height());
		main_x = QString::number(qApp->mainWidget()->x());
		main_y = QString::number(qApp->mainWidget()->y());
	}
	else
	{
		main_width = "";
		main_height = "";
		main_x = "";
		main_y = "";
	}
	
	m_settings->writeEntry("/lookandfeel/main_width",main_width);
	m_settings->writeEntry("/lookandfeel/main_height",main_height);
	m_settings->writeEntry("/lookandfeel/main_x",main_x);
	m_settings->writeEntry("/lookandfeel/main_y",main_y);
	m_settings->writeEntry("/lookandfeel/save_main_geom",save_main_geom);
	m_settings->endGroup();
	delete m_settings;
}

GofunSettings::GofunSettings()
{
	setCaption(tr("GoFun Settings"));
	
	QWidget* widget_general = new QWidget(this);
	QGridLayout* grid_general = new QGridLayout(widget_general,3,2);
	tabwidget->addTab(widget_general,tr("General"));
	
	terminal = new QLineEdit(widget_general);
	directory = new QLineEdit(widget_general);
	filemanager = new QLineEdit(widget_general);
	browser = new QLineEdit(widget_general);
	grid_general->addWidget(new QLabel(tr("Terminal command"),widget_general),0,0);
	grid_general->addWidget(terminal,0,1);
	grid_general->addWidget(new QLabel(tr("Directory"),widget_general),1,0);
	grid_general->addWidget(directory,1,1);
	grid_general->addWidget(new QLabel(tr("Filemanager"),widget_general),2,0);
	grid_general->addWidget(filemanager,2,1);
	grid_general->addWidget(new QLabel(tr("Browser"),widget_general),3,0);
	grid_general->addWidget(browser,3,1);
	
	QWidget* widget_laf = new QWidget(this);
	QGridLayout* grid_laf = new QGridLayout(widget_laf,3,1);
	tabwidget->addTab(widget_laf,tr("Look and feel"));
	
	QButtonGroup* col_group = new QButtonGroup(widget_laf, tr("Color"));
	col_group->setColumnLayout(0, Qt::Vertical );
	col_group->setAlignment( int( QButtonGroup::AlignTop ) );
	QGridLayout* grid_col = new QGridLayout(col_group->layout());
	col_system = new QRadioButton(tr("System"),col_group);
	col_random = new QRadioButton(tr("Random"),col_group);
	col_costum = new QRadioButton(tr("Costum"),col_group);
	costum_col_bt = new QToolButton(col_group);
	costum_col_bt->setMaximumWidth(30);
	grid_col->addWidget(col_system,0,0);
	grid_col->addMultiCellWidget(col_costum,0,0,1,2);
	grid_col->addWidget(col_random,0,3);
	grid_col->addWidget(new QLabel(tr("Costum color"),col_group),1,0);
	grid_col->addWidget(costum_col_bt,1,1);
	QSpacerItem* spacer = new QSpacerItem(40,20,QSizePolicy::Expanding, QSizePolicy::Minimum);
	grid_col->addMultiCell(spacer,1,1,2,3);
	
	save_main_geom = new QCheckBox(tr("Save main window geometry"),widget_laf);
	
	connect(costum_col_bt,SIGNAL(clicked()),this,SLOT(costumColorDialog()));
		
	grid_laf->addWidget(col_group,0,0);
	grid_laf->addWidget(save_main_geom,1,0);
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
	GSC::get()->filemanager_cmd = filemanager->text();
	GSC::get()->browser_cmd = browser->text();
	GSC::get()->gofun_dir = directory->text();
	
	col_costum->isChecked() ? GSC::get()->color_source = "costum" : 0;
	col_random->isChecked() ? GSC::get()->color_source = "random" : 0;
	col_system->isChecked() ? GSC::get()->color_source = "system" : 0;
	GSC::get()->save_main_geom = GofunMisc::boolToString(save_main_geom->isChecked());
	
	GSC::get()->costum_color = costum_col_bt->paletteBackgroundColor().name();
	
	GofunWidget::applyColorSettings();
		
	dynamic_cast<GofunWidget*>(qApp->mainWidget())->reloadData();
	
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
	command->setText(item->data()->Exec);
	directory->setText(item->data()->Path);
	file = item->file;*/
	terminal->setText(GSC::get()->terminal_cmd);
	filemanager->setText(GSC::get()->filemanager_cmd);
	browser->setText(GSC::get()->browser_cmd);
	directory->setText(GSC::get()->gofun_dir);
	
	if(GSC::get()->color_source == "random")
		col_random->setChecked(true);
	else if(GSC::get()->color_source == "system")
		col_system->setChecked(true);
	else if(GSC::get()->color_source == "costum")
		col_costum->setChecked(true);
		
	costum_col_bt->setPaletteBackgroundColor(GSC::get()->costum_color);
	
	save_main_geom->setChecked(GofunMisc::stringToBool(GSC::get()->save_main_geom));
}

void GofunSettings::costumColorDialog()
{
	costum_col_bt->setPaletteBackgroundColor(QColorDialog::getColor(GSC::get()->costum_color));
}




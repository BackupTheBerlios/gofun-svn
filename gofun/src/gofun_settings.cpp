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
#include <qcombobox.h>
#include <qstylefactory.h>
#include <qlineedit.h>
#include <qtoolbox.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

#include "gofun_settings.h"
#include "gofun_settings_container.h"
#include "gofun_widget.h"
#include "gofun_misc.h"
#include "gofun_directory_browser.h"

GofunSettings::GofunSettings()
{
	setCaption(tr("GoFun Settings"));
	
	QWidget* widget_general = new QWidget(this);
	QGridLayout* grid_general = new QGridLayout(widget_general,3,2);
	tabwidget->addTab(widget_general,tr("General"));
	
	terminal = new QLineEdit(widget_general);
	directory = new QLineEdit(widget_general);
	QToolButton* directory_button = new QToolButton(widget_general);
	filemanager = new QLineEdit(widget_general);
	browser = new QLineEdit(widget_general);
	
	connect(directory_button,SIGNAL(clicked()),this,SLOT(directoryDialog()));
	
	grid_general->addWidget(new QLabel(tr("Directory"),widget_general),0,0);
	grid_general->addWidget(directory,0,1);
	grid_general->addWidget(directory_button,0,2);
	grid_general->addWidget(new QLabel(tr("Terminal"),widget_general),1,0);
	grid_general->addWidget(terminal,1,1);
	grid_general->addWidget(new QLabel(tr("Filemanager"),widget_general),2,0);
	grid_general->addWidget(filemanager,2,1);
	grid_general->addWidget(new QLabel(tr("Browser"),widget_general),3,0);
	grid_general->addWidget(browser,3,1);
	
	QWidget* widget_laf = new QWidget(this);
	QGridLayout* grid_laf = new QGridLayout(widget_laf,3,1);
	tabwidget->addTab(widget_laf,tr("Look and feel"));
	
	QButtonGroup* style_group =new QButtonGroup(tr("Style"),widget_laf);
	style_group->setColumnLayout(1,Qt::Horizontal);
	styles = new QComboBox(style_group);
	styles->insertItem(tr("(System Default)"));
	styles->insertStringList(QStyleFactory::keys());
	
	QButtonGroup* col_group = new QButtonGroup(tr("Color"),widget_laf);
	col_group->setColumnLayout(0, Qt::Vertical );
	col_group->setAlignment( int( QButtonGroup::AlignTop ) );
	QGridLayout* grid_col = new QGridLayout(col_group->layout());
	col_system = new QRadioButton(tr("System"),col_group);
	col_random = new QRadioButton(tr("Random"),col_group);
	col_custom = new QRadioButton(tr("Custom"),col_group);
	custom_col_bt = new QToolButton(col_group);
	custom_col_bt->setMaximumWidth(30);
	grid_col->addWidget(col_system,0,0);
	grid_col->addMultiCellWidget(col_custom,0,0,1,2);
	grid_col->addWidget(col_random,0,3);
	grid_col->addWidget(new QLabel(tr("Custom color"),col_group),1,0);
	grid_col->addWidget(custom_col_bt,1,1);
	QSpacerItem* spacer = new QSpacerItem(40,20,QSizePolicy::Expanding, QSizePolicy::Minimum);
	grid_col->addMultiCell(spacer,1,1,2,3);
	
	save_main_geom = new QCheckBox(tr("Save main window geometry"),widget_laf);
	
	connect(custom_col_bt,SIGNAL(clicked()),this,SLOT(customColorDialog()));
		
	grid_laf->addWidget(style_group,0,0);
	grid_laf->addWidget(col_group,1,0);
	grid_laf->addWidget(save_main_geom,2,0);
}

void GofunSettings::directoryDialog()
{
	GofunDirectoryBrowser dir_browser;
	if(!directory->text().isEmpty())
		dir_browser.setStartDirectory(directory->text());
	else
		dir_browser.setStartDirectory(QDir::homeDirPath());
	if(dir_browser.exec() == QDialog::Accepted)
		directory->setText(dir_browser.selected());
}

void GofunSettings::save()
{
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
	
	if(GSC::get()->gofun_dir != directory->text())
	{
		GSC::get()->gofun_dir = directory->text();
		dynamic_cast<GofunWidget*>(qApp->mainWidget())->reloadData();
	}
	
	if(GSC::get()->style != styles->currentText())
	{
		if(styles->currentItem() != 0)
			GSC::get()->style = styles->currentText();
		else
			GSC::get()->style = "";
	}
	
	col_custom->isChecked() ? GSC::get()->color_source = "custom" : 0;
	col_random->isChecked() ? GSC::get()->color_source = "random" : 0;
	col_system->isChecked() ? GSC::get()->color_source = "system" : 0;
	GSC::get()->save_main_geom = GofunMisc::boolToString(save_main_geom->isChecked());
	
	GSC::get()->custom_color = custom_col_bt->paletteBackgroundColor().name();
	
	GSC::get()->save();
	
	GofunWidget::applyStyleSettings();
	GofunWidget::applyColorSettings();
}

void GofunSettings::load()
{
	terminal->setText(GSC::get()->terminal_cmd);
	filemanager->setText(GSC::get()->filemanager_cmd);
	browser->setText(GSC::get()->browser_cmd);
	directory->setText(GSC::get()->gofun_dir);
	
	styles->setCurrentText(GSC::get()->style);
	
	if(GSC::get()->color_source == "random")
		col_random->setChecked(true);
	else if(GSC::get()->color_source == "system")
		col_system->setChecked(true);
	else if(GSC::get()->color_source == "custom")
		col_custom->setChecked(true);
		
	custom_col_bt->setPaletteBackgroundColor(GSC::get()->custom_color);
	
	save_main_geom->setChecked(GofunMisc::stringToBool(GSC::get()->save_main_geom));
}

void GofunSettings::customColorDialog()
{
	QColor col;
	if((col = QColorDialog::getColor(GSC::get()->custom_color)).isValid())
		custom_col_bt->setPaletteBackgroundColor(col);
}




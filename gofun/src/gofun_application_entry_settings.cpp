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

#include <qlabel.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qapplication.h>

#include "gofun_application_entry_settings.h"
#include "gofun_misc.h"
#include "gofun_shell_operations.h"
#include "gofun_application_item.h"
#include "gofun_directory_button.h"
#include "gofun_list_dialog.h"
#include "gofun_command_editor.h"
#include "gofun_icon_dialog.h"
#include "gofun_directory_browser.h"
#include "gofun_desktop_entry_settings_widget.h"
#include "gofun_parameter_edit.h"
#include "gofun_list_popup.h"
#include "gofun_envvar_edit.h"
#include "gofun_application_entry_settings_parameter.h"
#include "gofun_application_entry_settings_advanced.h"
#include "gofun_application_entry_settings_environment.h"

GofunApplicationEntrySettings::GofunApplicationEntrySettings()
{	
	QGridLayout* grid = new QGridLayout(widget_main,3,3);
		
	command = new QLineEdit(widget_main);
	command_button = new QToolButton(widget_main);
	directory = new QLineEdit(widget_main);
	dir_button = new QToolButton(widget_main);
	grid->addMultiCellWidget(desw,0,0,0,2);
	grid->addWidget(new QLabel(tr("Command"),widget_main),1,0);
	grid->addWidget(command,1,1);
	grid->addWidget(command_button,1,2);
	grid->addWidget(new QLabel(tr("Directory"),widget_main),2,0);
	grid->addWidget(directory,2,1);
	grid->addWidget(dir_button,2,2);
	
	//grid->setRowStretch(3,1);
	/*QSpacerItem* spacer = new QSpacerItem(1,1,QSizePolicy::Minimum, QSizePolicy::Expanding);
	grid->addMultiCell(spacer,3,3,0,3);*/ //I'm not sure, if it looks better or worse.
	
	connect(command_button, SIGNAL(clicked()),this, SLOT(commandEditor()));
	connect(command,SIGNAL(textChanged(const QString&)),desw,SLOT(guessIcon(const QString&)));
	connect(dir_button, SIGNAL(clicked()),this, SLOT(dirDialog()));
	
	widget_env = new GofunApplicationEntrySettingsEnvironment(this);
	tabwidget->addTab(widget_env,tr("Environment"));
	
	widget_par = new GofunApplicationEntrySettingsParameter(this);
	tabwidget->addTab(widget_par,tr("Parameter"));
		
	widget_adv = new GofunApplicationEntrySettingsAdvanced();
	tabwidget->addTab(widget_adv,tr("Advanced"));
	
	item = 0;
}

void GofunApplicationEntrySettings::commandEditor()
{
	GofunCommandEditor* cmd_editor = new GofunCommandEditor();
	cmd_editor->setSettingsWidget(this);
	cmd_editor->setCommand(command->text());
	if(cmd_editor->exec() == QDialog::Accepted)
		command->setText(cmd_editor->command());	
}

void GofunApplicationEntrySettings::iconDialog()
{
	GofunIconDialog* id = new GofunIconDialog();
	QString start_dir;
	if(!desw->icon.isEmpty())
	{
		start_dir = desw->icon;
		id->setStartIcon(desw->icon);
	}
	else if(!directory->text().isEmpty())
	{
		start_dir = GofunShellOperations::extendFileString(directory->text());
	}
	id->setStartDir(start_dir);
	if(id->exec() == QDialog::Accepted)
	{
		desw->setIcon(id->selected());
	}
	delete id;
}

void GofunApplicationEntrySettings::dirDialog()
{
	QString start_dir;
	if(!directory->text().isEmpty())
		start_dir = GofunShellOperations::extendFileString(directory->text());
	else
		start_dir = "/";
	
	GofunDirectoryBrowser dir_browser;
	dir_browser.setStartDirectory(start_dir);
	if(dir_browser.exec() == QDialog::Accepted)
		directory->setText(dir_browser.selected());

/*	QString dir = QFileDialog::getExistingDirectory(start_dir, this, tr("get existing directory"), tr("Choose a directory"));
	if(!dir.isEmpty())
	{
		directory->setText(dir);
	}*/
}

void GofunApplicationEntrySettings::save()
{
	GofunDesktopEntrySettings::save();
}

void GofunApplicationEntrySettings::apply()
{
	if(!item)
		item = new GofunApplicationEntryData;
		
	GofunDesktopEntrySettings::apply();
	
	apply(data());
}

void GofunApplicationEntrySettings::apply(GofunApplicationEntryData* app_entry)
{
	if(app_entry->Type.isEmpty())
		app_entry->Type = "Application";
	app_entry->Exec = command->text();
	app_entry->Path = directory->text();

	widget_env->apply(app_entry);
	widget_par->apply(app_entry);
	widget_adv->apply(app_entry);
}

bool GofunApplicationEntrySettings::inputValid()
{
	if(!GofunDesktopEntrySettings::inputValid())
		return false;
	else
		return true;
}

void GofunApplicationEntrySettings::load(GofunApplicationEntryData* _item)
{
	GofunDesktopEntrySettings::load(_item);
	
	command->setText(data()->Exec);
	directory->setText(data()->Path);

	widget_env->load(data());
	widget_par->load(data());
	widget_adv->load(data());
}


GofunApplicationEntryData* GofunApplicationEntrySettings::data()
{
	return dynamic_cast<GofunApplicationEntryData*>(item);
}

void GofunApplicationEntrySettings::setDefaults()
{
	GofunDesktopEntrySettings::setDefaults();

	desw->setIcon("default_application.png");
}



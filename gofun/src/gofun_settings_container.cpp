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

#include <qsettings.h>
#include <qdir.h>
#include <qapplication.h>
 
#include "gofun_settings_container.h"
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
	style = m_settings->readEntry("style");
	color_source = m_settings->readEntry("colorsource");
	custom_color = m_settings->readEntry("customcolor");
	main_width = m_settings->readEntry("main_width");
	main_height = m_settings->readEntry("main_height");
	main_x = m_settings->readEntry("main_x");
	main_y = m_settings->readEntry("main_y");
	save_main_geom = m_settings->readEntry("save_main_geom");
	m_settings->endGroup();
	
	if(color_source.isEmpty())
		color_source = "system";
		
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
		
	m_settings->beginGroup("command_editor");
	ce_completion_popup = m_settings->readEntry("completion_popup");
	m_settings->endGroup();

	m_settings->beginGroup("shortcuts");
	sc_fullscreen = m_settings->readEntry("fullscreen");
	m_settings->endGroup();

	if(sc_fullscreen.isEmpty())
		sc_fullscreen = "f";
}

GofunSettingsContainer::~GofunSettingsContainer()
{
	m_settings->writeEntry("/commands/terminal",terminal_cmd);
	m_settings->writeEntry("/commands/filemanager",filemanager_cmd);
	m_settings->writeEntry("/commands/browser",browser_cmd);
	m_settings->writeEntry("/general/datadir",gofun_dir);
	m_settings->writeEntry("/lookandfeel/style",style);
	m_settings->writeEntry("/lookandfeel/colorsource",color_source);
	m_settings->writeEntry("/lookandfeel/customcolor",custom_color);
	
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
	m_settings->writeEntry("/command_editor/completion_popup",ce_completion_popup);
	m_settings->writeEntry("/shortcuts/fullscreen",sc_fullscreen);
	m_settings->endGroup();
	delete m_settings;
}


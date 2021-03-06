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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>  
#include <iostream>
#include <cstdlib>

#include <qapplication.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <qstyle.h>

#include "gofun_widget.h"
#include "gofun_misc.h"
#include "gofun_shell_operations.h"
#include "gofun_settings_container.h"

using namespace std;

int main(int argc, char *argv[])
{
  //Do some magic to setup the right data directory
  QString gofun_binDir = GofunMisc::binDir();
  QString gofun_data_dir;
  if(!gofun_binDir.isEmpty())
  {
  	gofun_data_dir = gofun_binDir + "/../share/gofun/";
	
	QDir dir;
	if(!dir.exists(gofun_data_dir))
		gofun_data_dir = "";
  }
  
  if(!gofun_data_dir.isEmpty())
  	chdir(gofun_data_dir);
	
  //Random isn't random by default  
  srand (time (0));
  
  QString icon_dir_string;
  if(QFileInfo("/usr/share/icons").exists())
  	icon_dir_string += "/usr/share/icons ";
  if(QFileInfo("/usr/share/pixmaps").exists())
  	icon_dir_string += "/usr/share/pixmaps";
  if(!icon_dir_string.isEmpty())
  	GofunShellOperations::shellCall("find "+icon_dir_string+" > $HOME/.gofun/icon_files");
  else
  	GofunShellOperations::shellCall("touch $HOME/.gofun/icon_files");
  
  //This is the app's core
  QApplication app(argc, argv);
  GofunWidget::system_palette = qApp->palette();
  GofunWidget::system_style = qApp->style().name();
    
  //make GoFun look colorful and different every startup
  GofunWidget::applyStyleSettings();
  GofunWidget::applyColorSettings();
  
  //Here the main widget is being created
  GofunWidget gofun_widget(0); //Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Dialog);
  if((GSC::get()->main_x == "-1") && (GSC::get()->main_y == "-1"))
  {
  	//Move the widget to the middle of the screen
  	GofunWindowOperations::centerWindow(&gofun_widget,GSC::get()->main_width.toInt(),GSC::get()->main_height.toInt());
  }
  else
  {
  	gofun_widget.setGeometry(GSC::get()->main_x.toInt(),GSC::get()->main_y.toInt(),GSC::get()->main_width.toInt(),GSC::get()->main_height.toInt());
  }
  //Apply shortcut settings
  gofun_widget.applyShortcutSettings();
  
  //Set a caption and an icon
  gofun_widget.setCaption("GoFun");
  gofun_widget.setIcon(GofunMisc::getIcon("gofun-16x16.png",16,16));
  
  //Declare gofun_widget to the official mainWidget
  app.setMainWidget(&gofun_widget);
  
  //Paint it on the screen
  gofun_widget.show();

  //Finally we start the application!
  int ret = app.exec();
  
  //Destroy the global GofunSettingsContainer instance and save its status
  GSC::save();
  
  //Return the return-value returned by app.exec() :)
  return ret;  
}


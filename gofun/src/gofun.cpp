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

#include <iostream>
#include <cstdlib>

#include <qapplication.h>
#include <qdir.h>

#include "gofun_widget.h"
#include "gofun_misc.h"
#include "gofun_settings.h"

using namespace std;

int main(int argc, char *argv[])
{
  //Do some magic to setup the right data directory
  QString gofun_bin_dir = GofunMisc::shell_call("whereis gofun | sed -e 's/gofun://' | sed -e 's/gofun//';").simplifyWhiteSpace();
  QString gofun_data_dir;
  if(!gofun_bin_dir.isEmpty())
  {
  	gofun_data_dir = gofun_bin_dir + "/../share/gofun/";
	
	QDir dir;
	if(!dir.exists(gofun_data_dir))
		gofun_data_dir = "";
  }
  
  if(!gofun_data_dir.isEmpty())
  	chdir(gofun_data_dir);
	
  //Random isn't random by default  
  srand (time (0));

  //This is the app's core
  QApplication app(argc, argv);
  //make GoFun look colorful and different every startup
  app.setPalette(QPalette(QColor(int(rand() % 256),int(rand() % 256),int(rand() % 256))));

  //Here the main widget is being created
  GofunWidget gofun_widget;
  int height = 240;
  int width = 360;
  //Move the widget to the middle of the screen
  gofun_widget.setGeometry(QApplication::desktop()->screen()->width() / 2 - width/2, QApplication::desktop()->screen()->height() / 2 - height/2, width, height);
  //Set a caption and an icon
  gofun_widget.setCaption("GoFun");
  gofun_widget.setIcon(QPixmap("gofun.png"));
  
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


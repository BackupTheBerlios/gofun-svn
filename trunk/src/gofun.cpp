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

#include "gofun_widget.h"

using namespace std;

int main(int argc, char *argv[])
{

  srand (time (0));

  QApplication app(argc, argv);
  app.setPalette(QPalette(QColor(int(rand() % 256),int(rand() % 256),int(rand() % 256))));

  GofunWidget gofun_widget;
  int height = 240;
  int width = 360;
  gofun_widget.setGeometry(QApplication::desktop()->screen()->width() / 2 - width/2, QApplication::desktop()->screen()->height() / 2 - height/2, width, height);
  gofun_widget.setCaption("GoFun");
  gofun_widget.setIcon(QPixmap("gofun.png"));
  app.setMainWidget(&gofun_widget);
  gofun_widget.show();

  app.exec();

  return EXIT_SUCCESS;
}


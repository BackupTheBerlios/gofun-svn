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

#include <qpixmap.h>
#include <qapplication.h>
 
#include "gofun_misc.h" 

QString GofunMisc::shell_call(const QString& call)
{
        char buf[1024];
        FILE *ptr;

	QString output;
        if ((ptr = popen(call.ascii(), "r")) != NULL)
                while (fgets(buf, 1024, ptr) != NULL)
                        output += buf;
                (void) pclose(ptr);
        return output;
}

QString GofunMisc::ext_filestring(const QString& str)
{
	return shell_call("echo "+ str);
}

void GofunMisc::center_window(QWidget* w, int width, int height)
{
  w->setGeometry(QApplication::desktop()->screen()->width() / 2 - width/2, QApplication::desktop()->screen()->height() / 2 - height/2, width, height);
}

void GofunFileDialogPreview::previewUrl(const QUrl& u)
{
	QString path = u.path();
	QPixmap pix( path );
	if ( pix.isNull() )
		setText(tr("This is not a pixmap"));
	else
		setPixmap( pix );
}


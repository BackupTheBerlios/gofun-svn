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
#include <qimage.h>
#include <qapplication.h>
#include <qprocess.h>
#include <qwidget.h>
#include <qtoolbutton.h>

#include "gofun_misc.h" 
#include "gofun_settings.h"
#include "gofun_file_dialog_preview.h"

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

QString GofunMisc::shellify_path(const QString& path)
{
	//QString res = path;
	//return res.replace(' ',"\\ ");
	return "'"+ext_filestring(path)+"'";
}

QString GofunMisc::ext_filestring(const QString& str)
{
	return shell_call("echo -n \""+ str+"\"").stripWhiteSpace();
}

QString GofunMisc::bin_dir()
{
	return GofunMisc::shell_call("whereis gofun | sed -e 's/gofun://' | sed -e 's/gofun//';").stripWhiteSpace();
}

void GofunMisc::center_window(QWidget* w, int width, int height)
{
  w->setGeometry(QApplication::desktop()->screen()->width() / 2 - width/2, QApplication::desktop()->screen()->height() / 2 - height/2, width, height);
}

QPixmap GofunMisc::get_icon(const QString& name, int pref_width, int pref_height)
{
	if(name.isEmpty())
		return QPixmap();
		
	QString file;
	if(QFile::exists(name))
		file = name;
	else if( QFile::exists(GofunMisc::ext_filestring(name)))
		file = GofunMisc::ext_filestring(name);
	else
	{		
		//FIXME: that code still looks quite hackish
		QStringList::Iterator choice;
		int m_width = 0;
		QStringList icon_files = QStringList::split("\n",GofunMisc::shell_call("grep "+name+" $HOME/.gofun/icon_files"));
		for(QStringList::Iterator it = icon_files.begin(); it != icon_files.end(); ++it)
		{
			if(!(*it).contains(name))
				continue;
		
			QImage img((*it));
			if(img.width() > m_width)
			{
				file = (*it);
				m_width = img.width();
			}
			if(img.width() == pref_width)
				break;
		}
	}
	if(!QFile::exists(file))
		return QPixmap();
	
	QImage image(file);
	QPixmap pixmap;
	pixmap.convertFromImage(image.scale(pref_width,pref_height));
	return pixmap;
}

void GofunMisc::attach_window( QWidget * base, QWidget * to_attach, Side pref, Side alt, int width, int height )
{
	if(width == -1)
		width = base->width();
	if(height == -1)
		height = base->height();

	bool alt_not_tried = true;
	bool pref_not_tried = true;
	bool success = false;
	Side side = pref;
	while(alt_not_tried)
	{
		if(!pref_not_tried && alt != D_None)
		{
			side = alt;
			alt_not_tried = false;
		}

		if(side == D_Under)
		{
			if((base->geometry().y()+base->geometry().height() + height) < 	QApplication::desktop()->screen()->height())
			{
				to_attach->setGeometry(base->x(),base->geometry().y()+base->geometry().height(),base->width(),-1);
				success = true;
				break;
			}
		}
		else if(side == D_Above)
		{
			if(base->y()-height-(base->geometry().y()-base->y()) > 0)
			{
				to_attach->setGeometry(base->x(),base->y()-height-(base->geometry().y()-base->y()),base->width(),height);
				success = true;
				break;
			}
		}
		else if(side == D_Right)
		{
			if(base->geometry().x()+base->width()+width < QApplication::desktop()->screen()->width())
			{
				to_attach->setGeometry(base->geometry().x()+base->width(),base->y(),width,base->height());
				success = true;
				break;
			}
		}
		else if(side == D_Left)
		{
			if((base->x()-base->width()-(base->geometry().x()-base->x())) > 0)
			{
				to_attach->setGeometry(base->x()-base->width()-(base->geometry().x()-base->x()),base->y(),base->width(),base->height());
				success = true;
				break;
			}
		}
		pref_not_tried = false;
	}
	
	if(!success)
		GofunMisc::center_window(to_attach,width,height);
}

bool GofunMisc::makeDir(const QString& path)
{
	//HACK: this might be slightly hackish, but should work (tm)
	QString call = "mkdir -p " + path;
	shell_call(call);
}

bool GofunMisc::stringToBool(const QString& str)
{
	return (str == "true");
}

QString GofunMisc::boolToString(bool b)
{
	if(b)
		return "true";
	else
		return "false";
}





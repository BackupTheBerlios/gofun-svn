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

#include <qstring.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qwidget.h>
 
#ifndef GOFUN_MISC
#define GOFUN_MISC

///Mixed methods needed in GoFun
struct GofunMisc
{
	static QString shell_call(const QString&);
	static QString ext_filestring(const QString&);
	static void center_window(QWidget*, int, int);
	static QString fileDialogGetImage(const QString& start_dir,const QString& caption, const QString& filter_desc);
};

class GofunFileDialogPreview : public QLabel, public QFilePreview
{
	public:
        GofunFileDialogPreview( QWidget *parent=0 ) : QLabel( parent ) {}

        void previewUrl( const QUrl &u );
};

class GofunFileIconProvider : public QFileIconProvider
{
	public:
	virtual const QPixmap * pixmap ( const QFileInfo & info );
};

#endif



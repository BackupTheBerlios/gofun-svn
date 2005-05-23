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

#include <qfiledialog.h>
#include <qlabel.h>
 
#ifndef GOFUN_FILE_DIALOG_PREVIEW
#define GOFUN_FILE_DIALOG_PREVIEW

///Generates a preview for files in a file-dialog
class GofunFileDialogPreview : public QLabel, public QFilePreview
{
	public:
        GofunFileDialogPreview( QWidget *parent=0 ) : QLabel( parent ) {}

        void previewUrl( const QUrl &u );
};

///Provides icons for files in a file-dialog
class GofunFileIconProvider : public QFileIconProvider
{
	public:
	virtual const QPixmap * pixmap ( const QFileInfo & info );
};

#endif

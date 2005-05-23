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

#include <qimage.h>
 
#include "gofun_file_dialog_preview.h"

void GofunFileDialogPreview::previewUrl(const QUrl& u)
{
	QImage img(u.path());
	if(img.isNull())
	{
		setText(tr("This is not an image"));
		return;
	}

	if(img.width() > 200)
		img = img.scaleWidth(200);
	if(img.height() > 300)
		img = img.scaleHeight(300);
	QPixmap pix;
	pix.convertFromImage(img);
	
	if ( pix.isNull() )
		setText(tr("This is not an image"));
	else
		setPixmap( pix );
}

const QPixmap * GofunFileIconProvider::pixmap(const QFileInfo& info)
{
	if(!info.isFile())
		return 0;
	
	QImage image(info.filePath());
	if(image.isNull())
		return 0;

	QPixmap* pixmap = new QPixmap;
	pixmap->convertFromImage(image.scale(16,16));
	return pixmap;
}


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
 
#include "gofun_file_dialog.h"
#include "gofun_file_dialog_preview.h"

QString GofunFileDialog::getOpenImageFileName(const QString& start_dir,const QString& caption, const QString& filter_desc)
{
	GofunFileDialogPreview* pre = new GofunFileDialogPreview;
	GofunFileIconProvider* iconpro = new GofunFileIconProvider;
	
	QFileDialog fd(start_dir,filter_desc + " (*.png *.xpm *.jpg *.bmp *.ico)");
	fd.setDir(start_dir);
	fd.setCaption(caption);
	fd.setContentsPreviewEnabled( TRUE );
	fd.setContentsPreview( pre, pre );
	fd.setIconProvider(iconpro);
	fd.setPreviewMode( QFileDialog::Contents );
	if(fd.exec() == QDialog::Accepted)
	{
		QString file = fd.selectedFile();
		delete pre;
		delete iconpro;
		if(QFileInfo(file).isFile())
			return file;
		else
			return QString();
	}
	else
	{
		delete pre;
		delete iconpro;
		return QString();
	}
}


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

#include <qlabel.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qapplication.h>

#include "gofun_item_settings.h"
#include "gofun_misc.h"
#include "gofun_item.h"
#include "gofun_cat_button.h"
#include "gofun_data.h"
#include "gofun_list_dialog.h"
#include "gofun_command_editor.h"

GofunItemSettings::GofunItemSettings()
{

//	item = 0;
}

void GofunItemSettings::iconDialog()
{
	QString start_dir;
	if(!icon->text().isEmpty())
	{
		start_dir = icon->text();
	}
	
	QString file = GofunMisc::fileDialogGetImage(start_dir,tr("Pick an icon"),tr("Icons"));
	if(!file.isEmpty())
	{
		icon->setText(file);
		icon_button->setPixmap(QPixmap(file));
	}
}

void GofunItemSettings::save()
{
	if(data()->File.isEmpty())
	{
	 	data()->File = category->data()->Catdir + "/" + caption->text() + ".desktop";
	}
	item->save();
}

void GofunItemSettings::apply()
{
	if(!item)
		item = new GofunItem(category->iconview,QString(""));

	data()->Name = caption->text();
	item->setText(data()->Name);
	data()->Icon = icon->text();
	data()->Comment = comment->text();
	if(!data()->Comment.isEmpty())
		item->setToolTipText(data()->Comment);
	if(data()->File.isEmpty())
	{
		data()->File = category->data()->Catdir + "/" + caption->text() + ".desktop";
	}
	item->loadIcon();
}

bool GofunItemSettings::inputValid()
{
	if(caption->text().isEmpty())
	{
		QMessageBox::information(this,tr("Caption isn't set yet"),tr("Dear User, I can set up this stuff properly for you,\n after you type in a caption for this entry. Thanks. :)"));
		caption->setFocus();
		return false;
	}
	else
	{
		return true;
	}
}

void GofunItemSettings::setCategory(GofunCatButton* cat)
{
	category = cat;
}

void GofunItemSettings::load(GofunItem* _item)
{
	item = _item;
	caption->setText(item->text());
	icon->setText(data()->Icon);
	icon_button->setPixmap(item->pixmap()?*item->pixmap():0);
	comment->setText(data()->Comment);
}

GofunItemData* GofunItemSettings::data()
{
	return item->data();
}


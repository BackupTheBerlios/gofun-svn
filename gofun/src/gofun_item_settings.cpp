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
#include "gofun_desktop_entry_settings_widget.h"
#include "gofun_icon_dialog.h"

GofunItemSettings::GofunItemSettings()
{
//	item = 0;
}

void GofunItemSettings::iconDialog()
{
	GofunIconDialog* id = new GofunIconDialog();
	QString start_dir;
	if(!desw->icon.isEmpty())
	{
		start_dir = desw->icon;
		id->setStartIcon(desw->icon);
	}
	if(!start_dir.isEmpty())
		start_dir = "/usr/share/icons";
	id->setStartDir(start_dir);
	if(id->exec() == QDialog::Accepted)
	{
		desw->icon = id->selected();
		desw->icon_button->setPixmap(GofunMisc::get_icon(id->selected()));
	}
	delete id;
}

void GofunItemSettings::save()
{
	if(data()->File.isEmpty())
	{
	 	data()->File = category->data()->Catdir + "/" + desw->caption->text().replace('/','\\') + ".desktop"; //FIXME: redundant? implement general "filename correcter"
	}
	data()->save();
}

void GofunItemSettings::apply()
{
	if(!item)
		item = new GofunItem(category->iconview,QString(""));

	data()->Name = desw->caption->text();
	item->setText(data()->Name);
	data()->Icon = desw->icon;
	data()->Comment = desw->comment->text();
	data()->GenericName = desw->generic_name;
	if(!data()->Comment.isEmpty())
		item->setToolTipText(data()->Comment);
	if(data()->File.isEmpty()) //FIXME: redundant? implement general "filename correcter"
	{
		data()->File = category->data()->Catdir + "/" + desw->caption->text().replace('/','\\') + ".desktop";
	}
	item->loadIcon();
}

bool GofunItemSettings::inputValid()
{
	if(desw->caption->text().isEmpty())
	{
		QMessageBox::information(this,tr("Caption isn't set yet"),tr("Dear User, I can set up this stuff properly for you,\n after you type in a caption for this entry. Thanks. :)"));
		desw->caption->setFocus();
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
	desw->caption->setText(item->text());
	desw->icon = data()->Icon;
	desw->icon_button->setPixmap(item->pixmap()?*item->pixmap():0);
	desw->comment->setText(data()->Comment);
	desw->generic_name = data()->GenericName;
	
	if(item->isReadOnly())
		apply_button->setEnabled(false);
}

GofunDesktopEntryData* GofunItemSettings::data()
{
	return item->data();
}

void GofunItemSettings::setDefaults()
{
	desw->caption->setFocus();
}


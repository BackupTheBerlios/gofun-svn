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

#include <qapplication.h>
#include <qlabel.h>
#include <qmessagebox.h>

#include "gofun_cat_settings.h"
#include "gofun_cat_button.h"
#include "gofun_data.h"
#include "gofun_iconview.h"
#include "gofun_misc.h"
#include "gofun_widget.h"
#include "gofun_settings.h"
#include "gofun_desktop_entry_settings_widget.h"
#include "gofun_icon_dialog.h"
#include "gofun_file_dialog.h"

GofunCatSettings::GofunCatSettings()
{
	setCaption(tr("GoFun Category Settings"));
	
	QWidget* widget_main = new QWidget(this);
	QGridLayout* grid = new QGridLayout(widget_main,6,3);

	tabwidget->addTab(widget_main,tr("Main"));
	
	desw = new GofunDesktopEntrySettingsWidget(widget_main);
	background = new QLineEdit(widget_main);
	background_button = new QToolButton(widget_main);
	grid->addMultiCellWidget(desw,0,0,0,2);
	grid->addWidget(new QLabel(tr("Background"),widget_main),1,0);
	grid->addWidget(background,1,1);
	grid->addWidget(background_button,1,2);
	
	connect(desw->icon_button,SIGNAL(clicked()),this,SLOT(iconDialog()));
	connect(background_button,SIGNAL(clicked()),this,SLOT(backgroundDialog()));

	item = 0;	
}

void GofunCatSettings::iconDialog()
{	//@TODO: Eliminate code duplication (GofunItem::iconDialog())
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

void GofunCatSettings::backgroundDialog()
{
	QString start_dir;
	if(!background->text().isEmpty())
		start_dir = background->text();
	else
		start_dir = QDir::homeDirPath();
	QString file = GofunFileDialog::getOpenImageFileName(start_dir,tr("Select background image"),tr("Images"));
	if(!file.isEmpty())
	{
		background->setText(file);
		background_button->setPixmap(file);
	}
}

void GofunCatSettings::save()
{
	if(item && item->data()->Catdir.isEmpty())
	{
		item->data()->Catdir = GSC::get()->gofun_dir + "/" + desw->caption->text();
		QDir dir;
		if(!dir.exists(item->data()->Catdir))
			dir.mkdir(item->data()->Catdir);
		
		if(item->data()->File.isEmpty())
			item->data()->File = item->data()->Catdir + "/.directory";
	}
	item->save();
}

void GofunCatSettings::apply()
{
	if(item)
	{
		item->data()->Name = desw->caption->text();
		item->setText(item->data()->Name + "   ");
		item->data()->Comment = desw->comment->text();

		item->data()->Icon = desw->icon;
		
		item->data()->X_GoFun_Background = background->text();
		item->refreshBackground();
		item->loadIcon();
	}
	else
	{
		item = new GofunCatButton(QString(""),dynamic_cast<GofunWidget*>(qApp->mainWidget())->categoryButtons());
		item->setData(new GofunCatEntryData);
		item->show();
		item->makeCurrent();
		apply();
	}
}

bool GofunCatSettings::inputValid()
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

void GofunCatSettings::load(GofunCatButton* _item)
{
	item = _item;
	desw->caption->setText(item->data()->Name);
	desw->comment->setText(item->data()->Comment);
	background->setText(item->data()->X_GoFun_Background);
	background_button->setPixmap(QPixmap(item->data()->X_GoFun_Background));
	desw->icon = item->data()->Icon;
	desw->icon_button->setPixmap(item->confButtonPixmap()?*item->confButtonPixmap():0);
	if(item->isReadOnly())
		apply_button->setEnabled(false);
}

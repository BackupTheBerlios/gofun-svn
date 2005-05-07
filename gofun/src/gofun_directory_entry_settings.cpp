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
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qcolordialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qtoolbutton.h>

#include "gofun_directory_entry_settings.h"
#include "gofun_directory_button.h"
#include "gofun_iconview.h"
#include "gofun_misc.h"
#include "gofun_widget.h"
#include "gofun_settings_container.h"
#include "gofun_desktop_entry_settings_widget.h"
#include "gofun_icon_dialog.h"
#include "gofun_file_dialog.h"
#include "gofun_radio_button_group.h"

GofunDirectoryEntrySettings::GofunDirectoryEntrySettings()
{
	setCaption(tr("GoFun Directory Settings"));
	
	QWidget* widget_main = new QWidget(this);
	QGridLayout* grid = new QGridLayout(widget_main,6,3);

	tabwidget->addTab(widget_main,tr("Main"));
	
	desw = new GofunDesktopEntrySettingsWidget(widget_main);

	QGroupBox* gb_background = new QGroupBox(tr("Background"),widget_main);
	gb_background->setColumnLayout(0, Qt::Vertical );
	gb_background->layout()->setSpacing( 6 );
	gb_background->layout()->setMargin( 5 );

	QGridLayout* grid_background = new QGridLayout(gb_background->layout());

	background_image = new QLineEdit(gb_background);
	background_image_button = new QToolButton(gb_background);
	background_color_button = new QToolButton(gb_background);

	GofunRadioButtonGroup* rb_bg_group = new GofunRadioButtonGroup;

	rb_bg_image = new QRadioButton(tr("Image"),gb_background);
	rb_bg_color = new QRadioButton(tr("Color"),gb_background);
	rb_bg_none = new QRadioButton(tr("None"),gb_background);

	rb_bg_group->add(rb_bg_image);
	rb_bg_group->add(rb_bg_color);
	rb_bg_group->add(rb_bg_none);

	grid_background->addWidget(rb_bg_image,0,0);
	grid_background->addWidget(background_image,0,1);
	grid_background->addWidget(background_image_button,0,2);
	grid_background->addWidget(rb_bg_color,1,0);
	grid_background->addMultiCellWidget(background_color_button,1,1,1,2);
	grid_background->addWidget(rb_bg_none,2,0);

	grid->addWidget(desw,0,0);
	grid->addWidget(gb_background,1,0);
	
	connect(desw->icon_button,SIGNAL(clicked()),this,SLOT(iconDialog()));
	connect(background_image_button,SIGNAL(clicked()),this,SLOT(backgroundDialog()));
	connect(background_color_button,SIGNAL(clicked()),this,SLOT(backgroundColorDialog()));

	item = 0;	
}

void GofunDirectoryEntrySettings::backgroundDialog()
{
	QString start_dir;
	if(!background_image->text().isEmpty())
		start_dir = background_image->text();
	else
		start_dir = QDir::homeDirPath();
	QString file = GofunFileDialog::getOpenImageFileName(start_dir,tr("Select background image"),tr("Images"));
	if(!file.isEmpty())
	{
		background_image->setText(file);
		background_image_button->setPixmap(file);
	}
}

void GofunDirectoryEntrySettings::save()
{
	/*if(data()->Directorydir.isEmpty())
	{
		data()->Directorydir = GSC::get()->gofun_dir + "/" + desw->caption->text();
		QDir dir;
		if(!dir.exists(data()->Directorydir))
			dir.mkdir(data()->Directorydir);
		
		if(data()->File.isEmpty())
			data()->File = data()->Directorydir + "/.directory";
	}*/
	GofunDesktopEntrySettings::save();
}

void GofunDirectoryEntrySettings::apply()
{
	if(data()->Type.isEmpty())
		data()->Type = "Directory";
	data()->Name = desw->caption->text();
	data()->Comment = desw->comment->text();
	data()->Icon = desw->icon;
	if(rb_bg_image->isChecked())
		data()->X_GoFun_Background = background_image->text();
	else if(rb_bg_color->isChecked())
		data()->X_GoFun_Background = background_color_button->paletteBackgroundColor().name();
	else if(rb_bg_none->isChecked())
		data()->X_GoFun_Background = "";

/*	setText(data()->Name + "   ");
	refreshBackground();
	loadIcon();*/

/*	else
	{
		item = new GofunDirectoryButton(QString(""),dynamic_cast<GofunWidget*>(qApp->mainWidget())->categoryButtons());
		setData(new GofunDirectoryEntryData);
		show();
		makeCurrent();
		apply();
	}*/
}

bool GofunDirectoryEntrySettings::inputValid()
{
	if(!GofunDesktopEntrySettings::inputValid())
		return false;
	else
		return true;
}

void GofunDirectoryEntrySettings::load(GofunDirectoryEntryData* _item)
{
	GofunDesktopEntrySettings::load(_item);
	
	if(QFile::exists(data()->X_GoFun_Background))
	{
		background_image->setText(data()->X_GoFun_Background);
		background_image_button->setPixmap(QPixmap(data()->X_GoFun_Background));
		rb_bg_image->setChecked(true);
	}
	else if(data()->X_GoFun_Background.isEmpty())
	{
		rb_bg_none->setChecked(true);
	}
	else if(QColor(data()->X_GoFun_Background).isValid())
	{
		background_color_button->setPaletteBackgroundColor(QColor(data()->X_GoFun_Background));
		rb_bg_color->setChecked(true);
	}

	if(item->isReadOnly())
		apply_button->setEnabled(false);
}

void GofunDirectoryEntrySettings::setDefaults( )
{
	GofunDesktopEntrySettings::setDefaults();

	desw->icon = "default_directory.png";
	desw->icon_button->setPixmap(QPixmap(desw->icon));
}

GofunDirectoryEntryData* GofunDirectoryEntrySettings::data()
{
	return dynamic_cast<GofunDirectoryEntryData*>(item);
}

void GofunDirectoryEntrySettings::backgroundColorDialog()
{
	QColor col;
	if((col = QColorDialog::getColor(QColor())).isValid())
		background_color_button->setPaletteBackgroundColor(col);
}

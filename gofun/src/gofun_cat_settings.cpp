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

GofunCatSettings::GofunCatSettings()
{
	setCaption(tr("GoFun Category Settings"));
	
	QWidget* widget_main = new QWidget(this);
	QGridLayout* grid = new QGridLayout(widget_main,6,3);

	tabwidget->addTab(widget_main,tr("Main"));
	
	caption = new QLineEdit(widget_main);
	comment = new QLineEdit(widget_main);
	background = new QLineEdit(widget_main);
	background_button = new QToolButton(widget_main);
	grid->addWidget(new QLabel(tr("Caption"),widget_main),0,0);
	grid->addWidget(caption,0,1);
	grid->addWidget(new QLabel(tr("Comment"),widget_main),1,0);
	grid->addWidget(comment,1,1);
	grid->addWidget(new QLabel(tr("Background"),widget_main),2,0);
	grid->addWidget(background,2,1);
	grid->addWidget(background_button,2,2);
	
	connect(background_button,SIGNAL(clicked()),this,SLOT(backgroundDialog()));

	item = 0;	
}

void GofunCatSettings::backgroundDialog()
{
	QString start_dir;
	if(!background->text().isEmpty())
		start_dir = background->text();
	else
		start_dir = QDir::homeDirPath();
	QString file = GofunMisc::fileDialogGetImage(start_dir,tr("Select background image"),tr("Images"));
	if(!file.isEmpty())
		background->setText(file);
}

void GofunCatSettings::save()
{
	if(!item)
	{
		//file.setName(caption->text() + "/" + ".directory");
	}
	item->save();
}

void GofunCatSettings::apply()
{
	if(item)
	{
		item->data->Name = caption->text();
		item->data->Comment = comment->text();
		item->data->X_GoFun_Background = background->text();
		if(!item->data->X_GoFun_Background.isEmpty())
			item->iconview->setPaletteBackgroundPixmap(QPixmap(item->data->X_GoFun_Background));	
		else
			item->iconview->setPaletteBackgroundColor(QApplication::palette().color(QPalette::Active,QColorGroup::Base));
	}
}

bool GofunCatSettings::inputValid()
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

void GofunCatSettings::load(GofunCatButton* _item)
{
	item = _item;
	caption->setText(item->data->Name);
	comment->setText(item->data->Comment);
	background->setText(item->data->X_GoFun_Background);
	background_button->setPixmap(QPixmap(item->data->X_GoFun_Background));
}

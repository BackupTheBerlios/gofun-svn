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

#include "gofun_item_settings.h"
#include "gofun_misc.h"
#include "gofun_item.h"
#include "gofun_cat_button.h"
#include "gofun_data.h"
 
GofunItemSettings::GofunItemSettings()
{	
	QWidget* widget_main = new QWidget(this);	
	QGridLayout* grid = new QGridLayout(widget_main,5,3);
	
	tabwidget->addTab(widget_main,"Main");
		
	caption = new QLineEdit(widget_main);
	command = new QLineEdit(widget_main);
	directory = new QLineEdit(widget_main);
	dir_button = new QToolButton(widget_main);
	icon = new QLineEdit(widget_main);
	icon_button = new QToolButton(widget_main);
	comment = new QLineEdit(widget_main);
	grid->addWidget(new QLabel("Caption",widget_main),0,0);
	grid->addWidget(caption,0,1);
	grid->addWidget(new QLabel("Command",widget_main),1,0);
	grid->addWidget(command,1,1);
	grid->addWidget(new QLabel("Directory",widget_main),2,0);
	grid->addWidget(directory,2,1);
	grid->addWidget(dir_button,2,2);
	grid->addWidget(new QLabel("Icon",widget_main),3,0);
	grid->addWidget(icon,3,1);
	grid->addWidget(icon_button,3,2);
	grid->addWidget(new QLabel("Comment",widget_main),4,0);
	grid->addWidget(comment,4,1);
	
	connect(icon_button, SIGNAL(clicked()),this, SLOT(iconDialog()));
	connect(dir_button, SIGNAL(clicked()),this, SLOT(dirDialog()));
	
	QWidget* widget_env = new QWidget(this);
	tabwidget->addTab(widget_env,"Environment");
	
	QGridLayout* grid_env = new QGridLayout(widget_env,1,3);
	
	envvars = new QListView(widget_env);
	envvars->addColumn("Name");
	envvars->addColumn("Value");
	envvars->setResizeMode(QListView::AllColumns);
	envadd = new QPushButton("Add", widget_env);
	envrem = new QPushButton("Remove", widget_env);
	envpre = new QPushButton("Predefined", widget_env);
	grid_env->addMultiCellWidget(envvars,0,0,0,3);
	grid_env->addWidget(envadd,1,0);
	grid_env->addWidget(envrem,1,1);
	grid_env->addWidget(envpre,1,3);
		
	connect(envvars, SIGNAL(doubleClicked(QListViewItem*,const QPoint&,int)), SLOT(envItemEdit(QListViewItem*,const QPoint&,int)));
	connect(envadd, SIGNAL(clicked()), SLOT(addEnvVar()));
	connect(envrem, SIGNAL(clicked()), SLOT(remEnvVar()));
	
	QWidget* widget_adv = new QWidget(this);
	tabwidget->addTab(widget_adv,"Advanced");
	
	QGridLayout* grid_adv = new QGridLayout(widget_adv,3,2);
	terminal_chk = new QCheckBox("Start in terminal",widget_adv);
	user_chk = new QCheckBox("Start as (user)",widget_adv);
	user_combo = new QComboBox(widget_adv);
	user_combo->insertItem("root");
	QStringList users = QStringList::split('\n',GofunMisc::shell_call("cat /etc/passwd | grep /home/ | sed -e 's/:.*$//'"));
	for(QStringList::Iterator it = users.begin(); it != users.end(); ++it)
	{
		user_combo->insertItem((*it));
	}
	
	grid_adv->addMultiCellWidget(terminal_chk,0,0,0,1);
	grid_adv->addWidget(user_chk,1,0);
	grid_adv->addWidget(user_combo,1,1);
	
	connect(user_chk, SIGNAL(toggled(bool)),this, SLOT(userChkToggled(bool)));
	
	userChkToggled(user_chk->isChecked());	
	
	item = 0;
}

void GofunItemSettings::userChkToggled(bool b)
{
	user_combo->setEnabled(b);
}

void GofunItemSettings::addEnvVar()
{
	new QListViewItem(envvars,"NAME","VALUE");
}

void GofunItemSettings::remEnvVar()
{
	if(envvars->currentItem())
	{
		envvars->takeItem(envvars->currentItem());
	}
}

void GofunItemSettings::envItemEdit(QListViewItem* item,const QPoint& pos,int i)
{
	if(!item)
		return;
	
	QDialog* edit_dlg = new QDialog(this,0,1);
	edit_dlg->setCaption("Edit environment variable");
	QGridLayout* grid = new QGridLayout(edit_dlg,3,2);
	grid->addWidget(new QLabel("Name",edit_dlg),0,0);
	QLineEdit* name_le = new QLineEdit(item->text(0),edit_dlg);
	grid->addWidget(name_le,0,1);
	grid->addWidget(new QLabel("Value",edit_dlg),1,0);
	QLineEdit* value_le = new QLineEdit(item->text(1),edit_dlg);
	grid->addWidget(value_le,1,1);
	QPushButton* apply = new QPushButton("Apply",edit_dlg);
	connect(apply, SIGNAL(clicked()),edit_dlg, SLOT(accept()));
	QPushButton* cancel = new QPushButton("Cancel",edit_dlg);
	connect(cancel, SIGNAL(clicked()),edit_dlg, SLOT(reject()));
	
	grid->addWidget(apply,2,0);
	grid->addWidget(cancel,2,1);
	
	if( edit_dlg->exec() == QDialog::Accepted)
	{
		item->setText(0,name_le->text());
		item->setText(1,value_le->text());
	}
}

void GofunItemSettings::iconDialog()
{
	QString start_dir;
	if(!icon->text().isEmpty())
	{
		start_dir = icon->text().ascii();
	}
	else if(!directory->text().isEmpty())
	{
		start_dir = GofunMisc::ext_filestring(directory->text());
	}
	GofunFileDialogPreview* pre = new GofunFileDialogPreview;
	
	QFileDialog* fd = new QFileDialog(start_dir,"Icons (*.png *.xpm *.jpg *.bmp *.ico)",this,"Pick icon dialog");
	fd->setDir(start_dir);
	fd->setCaption("Pick an icon");
	fd->setContentsPreviewEnabled( TRUE );
	fd->setContentsPreview( pre, pre );
	fd->setPreviewMode( QFileDialog::Contents );
	fd->exec();
	QString file =  fd->selectedFile();
	if(!file.isEmpty())
	{
		icon->setText(file);
		icon_button->setPixmap(QPixmap(file));
	}
	delete fd;
}

void GofunItemSettings::dirDialog()
{
	QString start_dir;
	if(!directory->text().isEmpty())
	{
		start_dir = GofunMisc::ext_filestring(directory->text());
	}
	QString dir = QFileDialog::getExistingDirectory(start_dir, this, "get existing directory", "Choose a directory");
	if(!dir.isEmpty())
	{
		directory->setText(dir);
	}
}

void GofunItemSettings::save()
{
	if(item && item->data->File.isEmpty())
	{
	 	item->data->File = category->data->Catdir + "/" + caption->text().ascii() + ".desktop";
	}
	item->save();
}

void GofunItemSettings::apply()
{
	if(item)
	{
		item->data->Exec = command->text().ascii();
		item->data->Path = directory->text().ascii();
		item->data->Name = caption->text().ascii();
		item->setText(item->data->Name);
		item->data->Icon = icon->text().ascii();
		item->data->Comment = comment->text().ascii();
		if(item->data->File.isEmpty())
		{
			item->data->File = category->data->Catdir + "/" + caption->text().ascii() + ".desktop";
		}
		item->loadIcon();
		if(terminal_chk->isChecked())
		{
			item->data->Terminal = "true";
		}
		else
		{
			item->data->Terminal = "false";
		}
		if(envvars->childCount() > 0)
		{
			QListViewItem* env_item = envvars->firstChild();
			std::vector<QString> tmp_array;
			while(env_item)
			{
				tmp_array.push_back(env_item->text(0)+"="+env_item->text(1));
				env_item = env_item->nextSibling();
			}
			item->data->X_GoFun_Env = tmp_array;
		}
		else
		{
			item->data->X_GoFun_Env.clear();
		}
		if(user_chk->isChecked())
		{
			item->data->X_GoFun_User = user_combo->currentText().ascii();
		}
		else
		{
			item->data->X_GoFun_User = "";
		}
	}
	else
	{
		item = new GofunItem(category->iconview,QString(""));
		apply();
		
	}
}

bool GofunItemSettings::inputValid()
{
	if(caption->text().isEmpty())
	{
		QMessageBox::information(this,"Caption isn't set yet","Dear User, I can set up this stuff properly for you,\n after you type in a caption for this entry. Thanks. :)");
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
	command->setText(item->data->Exec);
	directory->setText(item->data->Path);
	file = item->data->File;
	icon->setText(item->data->Icon);
	icon_button->setPixmap(QPixmap(item->data->Icon));
	comment->setText(item->data->Comment);
	if(item->data->Terminal == "true")
	{
		terminal_chk->setChecked(1);
	}
	if(!item->data->X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = item->data->X_GoFun_Env.begin(); it != item->data->X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			QStringList vk_pair = QStringList::split("=",(*it));
			new QListViewItem(envvars,vk_pair[0],vk_pair[1]);
		}
	}
	if(!item->data->X_GoFun_User.isEmpty())
	{
		user_chk->setChecked(true);
		user_combo->setCurrentText(item->data->X_GoFun_User);
	}
}

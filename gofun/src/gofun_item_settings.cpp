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
	grid->addWidget(new QLabel(tr("Caption"),widget_main),0,0);
	grid->addWidget(caption,0,1);
	grid->addWidget(new QLabel(tr("Command"),widget_main),1,0);
	grid->addWidget(command,1,1);
	grid->addWidget(new QLabel(tr("Directory"),widget_main),2,0);
	grid->addWidget(directory,2,1);
	grid->addWidget(dir_button,2,2);
	grid->addWidget(new QLabel(tr("Icon"),widget_main),3,0);
	grid->addWidget(icon,3,1);
	grid->addWidget(icon_button,3,2);
	grid->addWidget(new QLabel(tr("Comment"),widget_main),4,0);
	grid->addWidget(comment,4,1);
	
	connect(icon_button, SIGNAL(clicked()),this, SLOT(iconDialog()));
	connect(dir_button, SIGNAL(clicked()),this, SLOT(dirDialog()));
	
	QWidget* widget_env = new QWidget(this);
	tabwidget->addTab(widget_env,tr("Environment"));
	
	QGridLayout* grid_env = new QGridLayout(widget_env,1,3);
	
	envvars = new QListView(widget_env);
	envvars->addColumn(tr("Name"));
	envvars->addColumn(tr("Value"));
	envvars->setResizeMode(QListView::AllColumns);
	envadd = new QPushButton(tr("Add"), widget_env);
	envrem = new QPushButton(tr("Remove"), widget_env);
	envpre = new QPushButton(tr("Predefined"), widget_env);
	grid_env->addMultiCellWidget(envvars,0,0,0,3);
	grid_env->addWidget(envadd,1,0);
	grid_env->addWidget(envrem,1,1);
	grid_env->addWidget(envpre,1,3);
		
	connect(envvars, SIGNAL(doubleClicked(QListViewItem*,const QPoint&,int)), SLOT(envItemEdit(QListViewItem*,const QPoint&,int)));
	connect(envadd, SIGNAL(clicked()), SLOT(addEnvVar()));
	connect(envrem, SIGNAL(clicked()), SLOT(remEnvVar()));
	
	QWidget* widget_par = new QWidget(this);
	tabwidget->addTab(widget_par,tr("Parameter"));
	
	QGridLayout* grid_par = new QGridLayout(widget_par,3,3);
	tb_par = new QTable(widget_par);
	QHeader* tb_h = tb_par->horizontalHeader();
	tb_h->addLabel("Flag");
	tb_h->addLabel("Values");
	tb_h->addLabel("Default");
	tb_h->addLabel("Prompt");
	paradd = new QPushButton(tr("Add"), widget_par);
	parrem = new QPushButton(tr("Remove"), widget_par);	
	
	grid_par->addMultiCellWidget(tb_par,0,0,0,2);
	grid_par->addWidget(paradd,2,0);
	grid_par->addWidget(parrem,2,1);
	
	connect(paradd, SIGNAL(clicked()), this, SLOT(addParRow()));
	connect(parrem, SIGNAL(clicked()), this, SLOT(remParRow()));
	
	QWidget* widget_adv = new QWidget(this);
	tabwidget->addTab(widget_adv,tr("Advanced"));
	
	QGridLayout* grid_adv = new QGridLayout(widget_adv,3,2);
	terminal_chk = new QCheckBox(tr("Start in terminal"),widget_adv);
	user_chk = new QCheckBox(tr("Start as (user)"),widget_adv);
	user_combo = new QComboBox(widget_adv);
	user_combo->insertItem("root");
	newx_chk = new QCheckBox(tr("Start in new X-Server"),widget_adv);
	QStringList users = QStringList::split('\n',GofunMisc::shell_call("cat /etc/passwd | grep /home/ | sed -e 's/:.*$//'"));
	for(QStringList::Iterator it = users.begin(); it != users.end(); ++it)
	{
		user_combo->insertItem((*it));
	}
	
	grid_adv->addMultiCellWidget(terminal_chk,0,0,0,1);
	grid_adv->addWidget(user_chk,1,0);
	grid_adv->addWidget(user_combo,1,1);
	grid_adv->addMultiCellWidget(newx_chk,2,2,0,1);
	
	connect(user_chk, SIGNAL(toggled(bool)),this, SLOT(userChkToggled(bool)));
	
	userChkToggled(user_chk->isChecked());	
	
	item = 0;
}

void GofunItemSettings::addParRow()
{
	tb_par->insertRows(tb_par->numRows());
	tb_par->setItem(tb_par->numRows()-1,0,new QTableItem(tb_par,QTableItem::WhenCurrent));
	QPushButton* par_valedit_bt = new QPushButton(tr("Edit"),tb_par);
	tb_par->setCellWidget(tb_par->numRows()-1,1,par_valedit_bt);
	QStringList* sl = new QStringList;
	tb_par->setItem(tb_par->numRows()-1,2,new QComboTableItem(tb_par,*sl));
	tb_par->setItem(tb_par->numRows()-1,3,new QCheckTableItem(tb_par,""));
}

void GofunItemSettings::remParRow()
{

}

void GofunItemSettings::userChkToggled(bool b)
{
	user_combo->setEnabled(b);
}

void GofunItemSettings::addEnvVar()
{
	new QListViewItem(envvars,tr("NAME"),tr("VALUE"));
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
	edit_dlg->setCaption(tr("Edit environment variable"));
	QGridLayout* grid = new QGridLayout(edit_dlg,3,2);
	grid->addWidget(new QLabel(tr("Name"),edit_dlg),0,0);
	QLineEdit* name_le = new QLineEdit(item->text(0),edit_dlg);
	grid->addWidget(name_le,0,1);
	grid->addWidget(new QLabel(tr("Value"),edit_dlg),1,0);
	QLineEdit* value_le = new QLineEdit(item->text(1),edit_dlg);
	grid->addWidget(value_le,1,1);
	QPushButton* apply = new QPushButton(tr("Apply"),edit_dlg);
	connect(apply, SIGNAL(clicked()),edit_dlg, SLOT(accept()));
	QPushButton* cancel = new QPushButton(tr("Cancel"),edit_dlg);
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
		start_dir = icon->text();
	}
	else if(!directory->text().isEmpty())
	{
		start_dir = GofunMisc::ext_filestring(directory->text());
	}

	QString file = GofunMisc::fileDialogGetImage(start_dir,tr("Pick an icon"),tr("Icons"));
	if(!file.isEmpty())
	{
		icon->setText(file);
		icon_button->setPixmap(QPixmap(file));
	}
}

void GofunItemSettings::dirDialog()
{
	QString start_dir;
	if(!directory->text().isEmpty())
	{
		start_dir = GofunMisc::ext_filestring(directory->text());
	}
	QString dir = QFileDialog::getExistingDirectory(start_dir, this, tr("get existing directory"), tr("Choose a directory"));
	if(!dir.isEmpty())
	{
		directory->setText(dir);
	}
}

void GofunItemSettings::save()
{
	if(item && item->data()->File.isEmpty())
	{
	 	item->data()->File = category->data()->Catdir + "/" + caption->text() + ".desktop";
	}
	item->save();
}

void GofunItemSettings::apply()
{
	if(item)
	{
		item->data()->Exec = command->text();
		item->data()->Path = directory->text();
		item->data()->Name = caption->text();
		item->setText(item->data()->Name);
		item->data()->Icon = icon->text();
		item->data()->Comment = comment->text();
		if(!item->data()->Comment.isEmpty())
			item->setToolTipText(item->data()->Comment);
		if(item->data()->File.isEmpty())
		{
			item->data()->File = category->data()->Catdir + "/" + caption->text() + ".desktop";
		}
		item->loadIcon();
		if(terminal_chk->isChecked())
		{
			item->data()->Terminal = "true";
		}
		else
		{
			item->data()->Terminal = "false";
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
			item->data()->X_GoFun_Env = tmp_array;
		}
		else
		{
			item->data()->X_GoFun_Env.clear();
		}
		if(user_chk->isChecked())
		{
			item->data()->X_GoFun_User = user_combo->currentText();
		}
		else
		{
			item->data()->X_GoFun_User = "";
		}
		if(newx_chk->isChecked())
		{
			item->data()->X_GoFun_NewX = "true";
		}
		else
		{
			item->data()->X_GoFun_NewX = "false";
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
	command->setText(item->data()->Exec);
	directory->setText(item->data()->Path);
	file = item->data()->File;
	icon->setText(item->data()->Icon);
	icon_button->setPixmap(item->pixmap()?*item->pixmap():0);
	comment->setText(item->data()->Comment);
	if(item->data()->Terminal == "true")
	{
		terminal_chk->setChecked(true);
	}
	if(!item->data()->X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = item->data()->X_GoFun_Env.begin(); it != item->data()->X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			QStringList vk_pair = QStringList::split("=",(*it));
			new QListViewItem(envvars,vk_pair[0],vk_pair[1]);
		}
	}
	if(!item->data()->X_GoFun_User.isEmpty())
	{
		user_chk->setChecked(true);
		user_combo->setCurrentText(item->data()->X_GoFun_User);
	}
	if(item->data()->X_GoFun_NewX == "true")
	{
		newx_chk->setChecked(true);
	}
}


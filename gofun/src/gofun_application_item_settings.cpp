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

#include "gofun_application_item_settings.h"
#include "gofun_misc.h"
#include "gofun_application_item.h"
#include "gofun_cat_button.h"
#include "gofun_data.h"
#include "gofun_list_dialog.h"
#include "gofun_command_editor.h"
#include "gofun_icon_dialog.h"
#include "gofun_directory_browser.h"
#include "gofun_desktop_entry_settings_widget.h"
 
GofunApplicationItemSettings::GofunApplicationItemSettings()
{	
	QWidget* widget_main = new QWidget(this);	
	QGridLayout* grid = new QGridLayout(widget_main,3,3);
	
	tabwidget->addTab(widget_main,tr("Main"));
		
	desw = new GofunDesktopEntrySettingsWidget(widget_main);
	//QLineEdit* genre = new QLineEdit(widget_main);
	command = new QLineEdit(widget_main);
	command_button = new QToolButton(widget_main);
	directory = new QLineEdit(widget_main);
	dir_button = new QToolButton(widget_main);
	grid->addMultiCellWidget(desw,0,0,0,2);
	/*grid->addWidget(new QLabel(tr("Genre"),widget_main),1,0);
	grid->addWidget(genre,1,1);
	grid->addWidget(icon_button,3,2);*/
	/*QLabel* sep = new QLabel(widget_main);
	sep->setFrameStyle( QFrame::HLine | QFrame::Raised );
	sep->setLineWidth(2);
	sep->setMaximumHeight(4);
	grid->addMultiCellWidget(sep,4,4,0,2);*/
	grid->addWidget(new QLabel(tr("Command"),widget_main),1,0);
	grid->addWidget(command,1,1);
	grid->addWidget(command_button,1,2);
	grid->addWidget(new QLabel(tr("Directory"),widget_main),2,0);
	grid->addWidget(directory,2,1);
	grid->addWidget(dir_button,2,2);
	
	connect(command_button, SIGNAL(clicked()),this, SLOT(commandEditor()));
	connect(dir_button, SIGNAL(clicked()),this, SLOT(dirDialog()));
	connect(desw->icon_button, SIGNAL(clicked()),this, SLOT(iconDialog()));
	
	QWidget* widget_env = new QWidget(this);
	tabwidget->addTab(widget_env,tr("Environment"));
	
	QGridLayout* grid_env = new QGridLayout(widget_env,1,3);
	
	envvars = new QListView(widget_env);
	envvars->addColumn(tr("Name"));
	envvars->addColumn(tr("Value"));
	envvars->addColumn(tr("Interpreted"));
	envvars->setResizeMode(QListView::AllColumns);
	envadd = new QPushButton(tr("Add"), widget_env);
	envrem = new QPushButton(tr("Remove"), widget_env);
	envpre = new QPushButton(tr("Predefined"), widget_env);
	grid_env->addMultiCellWidget(envvars,0,0,0,3);
	grid_env->addWidget(envadd,1,0);
	grid_env->addWidget(envrem,1,1);
	QSpacerItem* envspacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid_env->addItem(envspacer,1,2);
	grid_env->addWidget(envpre,1,3);
		
	connect(envvars, SIGNAL(doubleClicked(QListViewItem*,const QPoint&,int)), SLOT(envItemEdit(QListViewItem*,const QPoint&,int)));
	connect(envadd, SIGNAL(clicked()), SLOT(addEnvVar()));
	connect(envrem, SIGNAL(clicked()), SLOT(remEnvVar()));
	connect(envpre, SIGNAL(clicked()), this, SLOT(envPredefinedPopup()));
	
	QWidget* widget_par = new QWidget(this);
	tabwidget->addTab(widget_par,tr("Parameter"));
	
	QGridLayout* grid_par = new QGridLayout(widget_par,3,3);
	tb_par = new QTable(0,4,widget_par);
	QHeader* tb_h = tb_par->horizontalHeader();
	tb_h->setLabel(0,"Flag");
	tb_h->setLabel(1,"Values");
	tb_h->setLabel(2,"Default");
	tb_h->setLabel(3,"Prompt");
	tb_par->adjustColumn(0);
	tb_par->adjustColumn(1);
	tb_par->adjustColumn(2);
	tb_par->adjustColumn(3);
	paradd = new QPushButton(tr("Add"), widget_par);
	parrem = new QPushButton(tr("Remove"), widget_par);	
	
	grid_par->addMultiCellWidget(tb_par,0,0,0,2);
	grid_par->addWidget(paradd,2,0);
	grid_par->addWidget(parrem,2,1);
	QSpacerItem* parspacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid_par->addItem(parspacer,2,2);
	
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

void GofunApplicationItemSettings::envItemEditInterpreted(const QString& string)
{}

void GofunApplicationItemSettings::envPredefinedPopup()
{
	QPopupMenu* popup = new QPopupMenu;
	
	QStringList envs = QStringList::split("\n",GofunMisc::shell_call("set | sed -e 's/=.*$//'"));
	
	int i = 0;
	for(QStringList::Iterator it = envs.begin(); it != envs.end(); ++it, ++i)
		popup->insertItem((*it),i);
	
	connect(popup,SIGNAL(activated(int)),this,SLOT(envPredefinedPopupActivated(int)));
	popup->popup(QCursor::pos());
}

void GofunApplicationItemSettings::envPredefinedPopupActivated(int id)
{
	QStringList envs = QStringList::split("\n",GofunMisc::shell_call("set | sed -e 's/=.*$//'"));
	addEnvVar(envs[id],"$"+envs[id]);
}

void GofunApplicationItemSettings::commandEditor()
{
	GofunCommandEditor* cmd_editor = new GofunCommandEditor();
	cmd_editor->setCommand(command->text());
	if(cmd_editor->exec() == QDialog::Accepted)
		command->setText(cmd_editor->command());	
}

void GofunApplicationItemSettings::addParRow()
{
	tb_par->insertRows(tb_par->numRows());
	tb_par->setItem(tb_par->numRows()-1,0,new QTableItem(tb_par,QTableItem::WhenCurrent));
	QPushButton* par_valedit_bt = new QPushButton(tr("Edit"),tb_par);
	connect(par_valedit_bt,SIGNAL(clicked()),this,SLOT(parValEditDialog()));
	
	tb_par->setCellWidget(tb_par->numRows()-1,1,par_valedit_bt);
	QStringList* sl = new QStringList;
	tb_par->setItem(tb_par->numRows()-1,2,new QComboTableItem(tb_par,*sl));
	tb_par->setItem(tb_par->numRows()-1,3,new QCheckTableItem(tb_par,""));
}

void GofunApplicationItemSettings::parValEditDialog()
{
	GofunListDialog* dialog = new GofunListDialog();
	dialog->exec();
}

void GofunApplicationItemSettings::remParRow()
{
	tb_par->removeRow(tb_par->currentRow());
}

void GofunApplicationItemSettings::userChkToggled(bool b)
{
	user_combo->setEnabled(b);
}

void GofunApplicationItemSettings::addEnvVar()
{
	addEnvVar(tr("NAME"),tr("VALUE"));
}

void GofunApplicationItemSettings::addEnvVar(const QString& name, const QString& value)
{
	new QListViewItem(envvars,name,value,GofunMisc::shell_call("echo -n "+value));
}

void GofunApplicationItemSettings::remEnvVar()
{
	if(envvars->currentItem())
	{
		envvars->takeItem(envvars->currentItem());
	}
}

GofunInterpretedLineEdit::GofunInterpretedLineEdit(const QString& text, QWidget* parent) : QLineEdit(text,parent)
{
}

void GofunInterpretedLineEdit::setText(const QString& text)
{
	QLineEdit::setText(GofunMisc::shell_call("echo -n "+text));
}

void GofunApplicationItemSettings::envItemEdit(QListViewItem* item,const QPoint& pos,int i)
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
	grid->addWidget(new QLabel(tr("Interpreted"),edit_dlg),2,0);
	GofunInterpretedLineEdit* interpreted_le = new GofunInterpretedLineEdit(item->text(2),edit_dlg);
	interpreted_le->setReadOnly(true);
	interpreted_le->setEnabled(false);
	grid->addWidget(interpreted_le,2,1);
	QPushButton* apply = new QPushButton(tr("Apply"),edit_dlg);
	connect(value_le, SIGNAL(textChanged(const QString&)), interpreted_le, SLOT(setText(const QString&)));
	connect(apply, SIGNAL(clicked()),edit_dlg, SLOT(accept()));
	QPushButton* cancel = new QPushButton(tr("Cancel"),edit_dlg);
	connect(cancel, SIGNAL(clicked()),edit_dlg, SLOT(reject()));
	
	grid->addWidget(apply,3,0);
	grid->addWidget(cancel,3,1);
	
	if( edit_dlg->exec() == QDialog::Accepted)
	{
		item->setText(0,name_le->text());
		item->setText(1,value_le->text());
		item->setText(2,GofunMisc::shell_call("echo -n "+value_le->text()));
	}
}

void GofunApplicationItemSettings::iconDialog()
{
	GofunIconDialog* id = new GofunIconDialog();	
	if(id->exec() == QDialog::Accepted)
	{
		desw->icon = id->selected();
		desw->icon_button->setPixmap(QPixmap(id->selected()));
	}
	delete id;

	/*QString start_dir;
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
	}*/
}

void GofunApplicationItemSettings::dirDialog()
{
	QString start_dir;
	if(!directory->text().isEmpty())
		start_dir = GofunMisc::ext_filestring(directory->text());
	else
		start_dir = "/";
	
	GofunDirectoryBrowser dir_browser;
	dir_browser.setStartDirectory(start_dir);
	if(dir_browser.exec() == QDialog::Accepted)
		directory->setText(dir_browser.selected());

/*	QString dir = QFileDialog::getExistingDirectory(start_dir, this, tr("get existing directory"), tr("Choose a directory"));
	if(!dir.isEmpty())
	{
		directory->setText(dir);
	}*/
}

void GofunApplicationItemSettings::save()
{
	GofunItemSettings::save();
}

void GofunApplicationItemSettings::apply()
{
	if(!item)
		item = new GofunApplicationItem(category->iconview,QString(""));
		
	GofunItemSettings::apply();
	
	data()->Exec = command->text();
	data()->Path = directory->text();
	data()->Terminal = GofunMisc::boolToString(terminal_chk->isChecked());
	if(envvars->childCount() > 0)
	{
		QListViewItem* env_item = envvars->firstChild();
		std::vector<QString> tmp_array;
		while(env_item)
		{
			tmp_array.push_back(env_item->text(0)+"="+env_item->text(1));
			env_item = env_item->nextSibling();
		}
		data()->X_GoFun_Env = tmp_array;
	}
	else
	{
		data()->X_GoFun_Env.clear();
	}
	data()->X_GoFun_Parameter.clear();
	if(tb_par->numRows() > 0)
	{
		for(int i = 0; i < tb_par->numRows(); ++i)
		{
			data()->X_GoFun_Parameter[i].Flag = tb_par->item(i,0)->text();
			data()->X_GoFun_Parameter[i].Default_Value = dynamic_cast<QComboTableItem*>(tb_par->item(i,2))->currentText();
			data()->X_GoFun_Parameter[i].Prompt = GofunMisc::boolToString(dynamic_cast<QCheckTableItem*>(tb_par->item(i,3))->isChecked());
		}
	}
	if(user_chk->isChecked())
	{
		data()->X_GoFun_User = user_combo->currentText();
	}
	else
	{
		data()->X_GoFun_User = "";
	}
	data()->X_GoFun_NewX = GofunMisc::boolToString(newx_chk->isChecked());
}

bool GofunApplicationItemSettings::inputValid()
{
	if(!GofunItemSettings::inputValid())
		return false;
	else
		return true;
}

void GofunApplicationItemSettings::load(GofunApplicationItem* _item)
{
	GofunItemSettings::load(_item);
	
	command->setText(data()->Exec);
	directory->setText(data()->Path);
	terminal_chk->setChecked(GofunMisc::stringToBool(data()->Terminal));
	if(!data()->X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = data()->X_GoFun_Env.begin(); it != data()->X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			QStringList vk_pair = QStringList::split("=",(*it));
			addEnvVar(vk_pair[0],QString((*it)).remove(0,vk_pair[0].length()+1));
		}
	}
	if(!data()->X_GoFun_User.isEmpty())
	{
		user_chk->setChecked(true);
		user_combo->setCurrentText(data()->X_GoFun_User);
	}
	newx_chk->setChecked(GofunMisc::stringToBool(data()->X_GoFun_NewX));
	if(!data()->X_GoFun_Parameter.empty())
	{
		for(std::map<int,GofunParameterData>::iterator it = data()->X_GoFun_Parameter.begin(); it != data()->X_GoFun_Parameter.end(); ++it)
		{
			tb_par->insertRows(tb_par->numRows());
			tb_par->setItem(tb_par->numRows()-1,0,new QTableItem(tb_par,QTableItem::WhenCurrent,(*it).second.Flag));
			QPushButton* par_valedit_bt = new QPushButton(tr("Edit"),tb_par);
			connect(par_valedit_bt,SIGNAL(clicked()),this,SLOT(parValEditDialog()));
	
			tb_par->setCellWidget(tb_par->numRows()-1,1,par_valedit_bt);
			QStringList* sl = new QStringList((*it).second.Values);
			tb_par->setItem(tb_par->numRows()-1,2,new QComboTableItem(tb_par,*sl));
			dynamic_cast<QComboTableItem*>(tb_par->item(tb_par->numRows()-1,2))->setCurrentItem((*it).second.Default_Value);
			tb_par->setItem(tb_par->numRows()-1,3,new QCheckTableItem(tb_par,""));
			dynamic_cast<QCheckTableItem*>(tb_par->item(tb_par->numRows()-1,3))->setChecked(GofunMisc::stringToBool((*it).second.Prompt));
		}
	}
}


GofunApplicationItemData* GofunApplicationItemSettings::data()
{
	return dynamic_cast<GofunApplicationItem*>(item)->data();
}


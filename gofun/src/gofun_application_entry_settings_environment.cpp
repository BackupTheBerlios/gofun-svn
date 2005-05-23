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

#include <qlayout.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qheader.h>
#include <qcursor.h>

#include "gofun_application_entry_settings_environment.h"
#include "gofun_application_entry_data.h"
#include "gofun_envvar_edit.h"
#include "gofun_list_popup.h"
#include "gofun_misc.h"
#include "gofun_shell_operations.h"

GofunApplicationEntrySettingsEnvironment::GofunApplicationEntrySettingsEnvironment(QWidget* parent) : QWidget(parent)
{
	QGridLayout* grid_env = new QGridLayout(this,1,4);
	
	envvars = new QListView(this);
	envvars->addColumn(tr("Name"));
	envvars->addColumn(tr("Value"));
	envvars->addColumn(tr("Interpreted"));
	envvars->setResizeMode(QListView::AllColumns);
	QIconSet::setIconSize(QIconSet::Small,QSize(12,12));
	envadd = new QPushButton(QIconSet(QPixmap("yellow_plus_12.png"),QIconSet::Small),tr("Add"), this);
	envrem = new QPushButton(QIconSet(QPixmap("yellow_minus_12.png"),QIconSet::Small),tr("Remove"), this);
	envpre = new QPushButton(QIconSet(QPixmap("way_and_landscape_12.png"),QIconSet::Small),tr("Predefined"), this);
	envedit = new QPushButton(QIconSet(QPixmap("stone_cutting_12.png"),QIconSet::Small),tr("Edit"), this);
	QIconSet::setIconSize(QIconSet::Small,QSize(22,22));
	envedit->setEnabled(false);
	grid_env->addMultiCellWidget(envvars,0,0,0,4);
	grid_env->addWidget(envedit,1,0);
	grid_env->addWidget(envadd,1,1);
	grid_env->addWidget(envrem,1,2);
	QSpacerItem* envspacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid_env->addItem(envspacer,1,3);
	grid_env->addWidget(envpre,1,4);
		
	connect(envvars, SIGNAL(doubleClicked(QListViewItem*,const QPoint&,int)), SLOT(envItemEdit(QListViewItem*,const QPoint&,int)));
	connect(envvars, SIGNAL(selectionChanged()),this,SLOT(editableEnvVar()));
	connect(envadd, SIGNAL(clicked()), SLOT(addEnvVar()));
	connect(envrem, SIGNAL(clicked()), SLOT(remEnvVar()));
	connect(envpre, SIGNAL(clicked()), this, SLOT(envPredefinedPopup()));
	connect(envedit, SIGNAL(clicked()), this, SLOT(envItemEdit()));
}

void GofunApplicationEntrySettingsEnvironment::envItemEditInterpreted(const QString& string)
{}

void GofunApplicationEntrySettingsEnvironment::envPredefinedPopup()
{
	GofunListPopup* popup = new GofunListPopup;
	popup->addColumn(tr("Predefined environment variables"));
	popup->header()->hide();
	
	QStringList envs = QStringList::split("\n",GofunShellOperations::shellCall("set | sed -e 's/=.*$//'"));
	
	for(QStringList::Iterator it = envs.begin(); it != envs.end(); ++it)
		new QListViewItem(popup,(*it));
	
	connect(popup,SIGNAL(clicked(QListViewItem*)),this,SLOT(envPredefinedPopupActivated(QListViewItem*)));
	popup->popup(QCursor::pos());
}

void GofunApplicationEntrySettingsEnvironment::envPredefinedPopupActivated(QListViewItem* item)
{
	addEnvVar(item->text(0),"$"+item->text(0));
}

void GofunApplicationEntrySettingsEnvironment::addEnvVar()
{
	addEnvVar(tr("NAME"),tr("VALUE"));
}

void GofunApplicationEntrySettingsEnvironment::addEnvVar(const QString& name, const QString& value)
{
	new QListViewItem(envvars,name,value,GofunShellOperations::shellCall("echo -n "+value));
}

void GofunApplicationEntrySettingsEnvironment::remEnvVar()
{
	if(envvars->currentItem())
	{
		envvars->takeItem(envvars->currentItem());
	}
}

void GofunApplicationEntrySettingsEnvironment::envItemEdit(QListViewItem* item,const QPoint& pos,int i)
{
	if(!item)
		return;

	GofunEnvVarEdit* edit_dlg = new GofunEnvVarEdit(this);
	edit_dlg->setName(item->text(0));
	edit_dlg->setValue(item->text(1));

	if( edit_dlg->exec() == QDialog::Accepted)
	{
		item->setText(0,edit_dlg->getName());
		item->setText(1,edit_dlg->getValue());
		item->setText(2,GofunShellOperations::shellCall("echo -n "+edit_dlg->getValue()));
	}
}

void GofunApplicationEntrySettingsEnvironment::envItemEdit()
{
	envItemEdit(envvars->selectedItem(),QPoint(),0);
}

void GofunApplicationEntrySettingsEnvironment::editableEnvVar()
{
	if(envvars->selectedItem())
		envedit->setEnabled(true);
	else
		envedit->setEnabled(false);
}

void GofunApplicationEntrySettingsEnvironment::apply(GofunApplicationEntryData* data)
{
	if(envvars->childCount() > 0)
	{
		QListViewItem* env_item = envvars->firstChild();
		std::vector<QString> tmp_array;
		while(env_item)
		{
			tmp_array.push_back(env_item->text(0)+"="+env_item->text(1));
			env_item = env_item->nextSibling();
		}
		data->X_GoFun_Env = tmp_array;
	}
	else
	{
		data->X_GoFun_Env.clear();
	}
}

void GofunApplicationEntrySettingsEnvironment::load( GofunApplicationEntryData* data)
{
	if(!data->X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = data->X_GoFun_Env.begin(); it != data->X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			QStringList vk_pair = QStringList::split("=",(*it));
			addEnvVar(vk_pair[0],QString((*it)).remove(0,vk_pair[0].length()+1));
		}
	}
}


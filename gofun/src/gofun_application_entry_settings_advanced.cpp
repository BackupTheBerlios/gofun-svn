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
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlabel.h>

#include "gofun_application_entry_settings_advanced.h"
#include "gofun_application_entry_data.h"
#include "gofun_misc.h"

GofunApplicationEntrySettingsAdvanced::GofunApplicationEntrySettingsAdvanced(QWidget* parent) : QWidget(parent)
{
	QGridLayout* grid_adv = new QGridLayout(this,3,2);
	terminal_chk = new QCheckBox(tr("Start in terminal"),this);
	user_chk = new QCheckBox(tr("Start as (user)"),this);
	user_combo = new QComboBox(this);
	user_combo->insertItem("root");
	QStringList users = QStringList::split('\n',GofunMisc::shellCall("cat /etc/passwd | grep /home/ | sed -e 's/:.*$//'"));
	for(QStringList::Iterator it = users.begin(); it != users.end(); ++it)
	{
		user_combo->insertItem((*it));
	}

	QGroupBox* gb_newx = new QGroupBox(tr("Start in new XServer"),this);
	gb_newx->setColumnLayout(0, Qt::Vertical );
	gb_newx->layout()->setSpacing( 6 );
	gb_newx->layout()->setMargin( 5 );

	QGridLayout* grid_newx = new QGridLayout(gb_newx->layout());

	newx_chk = new QCheckBox(tr("Start in new X-Server"),gb_newx);
	newx_options = new QLineEdit(gb_newx);

	grid_newx->addMultiCellWidget(newx_chk,0,0,0,1);
	grid_newx->addWidget(new QLabel(tr("Options:"),gb_newx),1,0);
	grid_newx->addWidget(newx_options,1,1);
	
	grid_adv->addMultiCellWidget(terminal_chk,0,0,0,1);
	grid_adv->addWidget(user_chk,1,0);
	grid_adv->addWidget(user_combo,1,1);
	grid_adv->addMultiCellWidget(gb_newx,2,2,0,1);
	grid_adv->setRowStretch(1,1);
	
	connect(user_chk, SIGNAL(toggled(bool)),this, SLOT(userChkToggled(bool)));
	connect(newx_chk, SIGNAL(toggled(bool)),this, SLOT(newxChkToggled(bool)));
	
	userChkToggled(user_chk->isChecked());
	newxChkToggled(newx_chk->isChecked());
}

void GofunApplicationEntrySettingsAdvanced::userChkToggled(bool b)
{
	user_combo->setEnabled(b);
}

void GofunApplicationEntrySettingsAdvanced::newxChkToggled(bool b)
{
	newx_options->setEnabled(b);
}

void GofunApplicationEntrySettingsAdvanced::apply(GofunApplicationEntryData * app_entry)
{
	app_entry->Terminal = GofunMisc::boolToString(terminal_chk->isChecked());
	if(user_chk->isChecked())
	{
		app_entry->X_GoFun_User = user_combo->currentText();
	}
	else
	{
		app_entry->X_GoFun_User = "";
	}
	app_entry->X_GoFun_NewX = GofunMisc::boolToString(newx_chk->isChecked());
	app_entry->X_GoFun_XOptions = newx_options->text();
}

void GofunApplicationEntrySettingsAdvanced::load(GofunApplicationEntryData* data)
{
	terminal_chk->setChecked(GofunMisc::stringToBool(data->Terminal));
	if(!data->X_GoFun_User.isEmpty())
	{
		user_chk->setChecked(true);
		user_combo->setCurrentText(data->X_GoFun_User);
	}
	newx_chk->setChecked(GofunMisc::stringToBool(data->X_GoFun_NewX));
	newx_options->setText(data->X_GoFun_XOptions);
}


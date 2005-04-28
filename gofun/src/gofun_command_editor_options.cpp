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
#include <qcheckbox.h>
#include <qpushbutton.h>
 
#include "gofun_command_editor_options.h"
#include "gofun_settings_container.h"
#include "gofun_misc.h"

GofunCommandEditorOptions::GofunCommandEditorOptions()
{
	QGridLayout* grid = new QGridLayout(this);
	
	completion_popup = new QCheckBox(tr("Enable completion popup"),this);
	completion_popup->setChecked(GofunMisc::stringToBool(GSC::get()->ce_completion_popup));
	
	QPushButton* ok = new QPushButton(tr("Ok"),this);
	QPushButton* cancel = new QPushButton(tr("Cancel"),this);
	
	connect(ok,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	
	grid->addMultiCellWidget(completion_popup,0,0,0,1);
	grid->addWidget(ok,1,0);
	grid->addWidget(cancel,1,1);
}

void GofunCommandEditorOptions::apply()
{
	GSC::get()->ce_completion_popup = GofunMisc::boolToString(completion_popup->isChecked());
}


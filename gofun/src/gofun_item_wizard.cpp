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

#include <qlayout.h>
#include <qlabel.h>
 
#include "gofun_item_wizard.h"

GofunItemWizard::GofunItemWizard()
{
	setCaption(tr("")+tr("Add Entry Wizard"));
	
	QWidget* widget_start = new QWidget(this);
	addPage(widget_start,tr("Start"));
	QGridLayout* grid_start = new QGridLayout(widget_start,3,1);
	
	grid_start->addWidget(new QLabel(tr("This wizard will guide you through the creation of a Gofun Desktop Entry"),widget_start),0,0);
}


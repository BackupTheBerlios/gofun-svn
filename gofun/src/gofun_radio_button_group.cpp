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

#include <qradiobutton.h>

#include "gofun_radio_button_group.h"

GofunRadioButtonGroup::GofunRadioButtonGroup()
{
	last_checked_button = 0;
}

void GofunRadioButtonGroup::add(QRadioButton* button)
{
	connect(button,SIGNAL(toggled(bool)),this,SLOT(buttonToggled(bool)));
	buttons.push_back(button);
}

QRadioButton* GofunRadioButtonGroup::getCheckedButton()
{
	for(std::vector<QRadioButton*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
		if((*it)->isChecked())
			return (*it);
	return 0;
}

void GofunRadioButtonGroup::buttonToggled(bool b)
{
	const QRadioButton* toggled_button = dynamic_cast<const QRadioButton*>(sender());
	if(!b)
	{
		if(buttons.size() == 1)
			return;

		if(toggled_button != last_checked_button)
		const_cast<QRadioButton*>(toggled_button)->setChecked(true);
		return;
	}

	for(std::vector<QRadioButton*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
	{
		if(toggled_button == (*it))
			continue;
		else if((*it)->isChecked())
		{
			last_checked_button = (*it);
			(*it)->setChecked(false);
		}
	}
}


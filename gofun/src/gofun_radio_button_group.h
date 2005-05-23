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

#include <vector>

#include <qobject.h>

#ifndef GOFUN_RADIO_BUTTON_GROUP
#define GOFUN_RADIO_BUTTON_GROUP

class QRadioButton;

///Radiobutton group
/** You can put QRadioButtons
    in this object and they'll 
    behave, like in a QButtonGroup. */
class GofunRadioButtonGroup : public QObject
{
	Q_OBJECT

	public:
	GofunRadioButtonGroup();
	
	void add(QRadioButton*);
	QRadioButton* getCheckedButton();

	private slots:
	void buttonToggled(bool);

	private:
	std::vector<QRadioButton*> buttons;
	QRadioButton* last_checked_button;
};

#endif

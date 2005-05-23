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

#include <qwidget.h>
#include <qvaluelist.h>

#include "gofun_application_entry_data.h"

#ifndef GOFUN_APPLICATION_ITEM_SETTINGS_PARAMETER
#define GOFUN_APPLICATION_ITEM_SETTINGS_PARAMETER

class QTable;
class QPushButton;

class GofunApplicationEntrySettingsParameter : public QWidget
{
	Q_OBJECT

	public:
	GofunApplicationEntrySettingsParameter(QWidget*);
	void apply(GofunApplicationEntryData*);
	void load(GofunApplicationEntryData*);

	private slots:
	void addParRow();
	void remParRow();
	void parRowUp();
	void parRowDown();
	void parEditDialog();

	private:
	QTable* tb_par;
	QPushButton* paradd;
	QPushButton* parrem;
	QPushButton* parup;
	QPushButton* pardown;

	QValueList<GofunParameterData> par_data;
};

#endif


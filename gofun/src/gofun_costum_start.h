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

#include <qdialog.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>

class GofunApplicationItem;
class GofunApplicationItemSettingsAdvanced;

#ifndef GOFUN_CSTART
#define GOFUN_CSTART

///Dialog making a highly costumized start possible
class GofunCostumStart : public QDialog
{
	Q_OBJECT
public:
	GofunCostumStart();
public slots:
	void start();
	virtual void load(GofunApplicationItem*);
	
private:
	QLabel* icon;
	QLabel* caption;
	QLineEdit* command;
	QLineEdit* directory;
	QComboBox* nxs_depth;
	QComboBox* nxs_resolution;
	GofunApplicationItemSettingsAdvanced* widget_adv;
	
	GofunApplicationItem* item;
};

#endif


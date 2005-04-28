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
#include <qgroupbox.h>
#include <qspinbox.h>

#include "gofun_application_entry_data.h"

class QLineEdit;
class QComboBox;
class QCheckBox;
class QGridLayout;
class GofunListWidget;
class GofunDecimalSpinBox;
class GofunParameterEditWidget;

#ifndef GOFUN_PARAMETER_EDIT
#define GOFUN_PARAMETER_EDIT

class GofunParameterEdit : public QDialog
{
	Q_OBJECT

	public:
	GofunParameterEdit();
	void setParameterData(const GofunParameterData&);
	GofunParameterData getParameterData();
	
	private slots:
	void typeChange(int);
		
	private:
	QLineEdit* flag;
	QLineEdit* comment;
	QComboBox* type;
	QGridLayout* grid_type;
	GofunParameterEditWidget* type_edit;
	
	GofunParameterData par_data;
};

#endif

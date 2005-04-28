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

#include "gofun_parameter_edit_widget.h"
 
#ifndef GOFUN_PARAMETER_DECIMAL_EDIT_WIDGET
#define GOFUN_PARAMETER_DECIMAL_EDIT_WIDGET

class GofunDecimalSpinBox;
class QCheckBox;

class GofunParameterDecimalEditWidget : public GofunParameterEditWidget
{
	Q_OBJECT
	
	public:
	GofunParameterDecimalEditWidget(QWidget*);
	void setDefaultValue(const QString&);
	void setMinimum(const QString&);
	void setMaximum(const QString&);
	QString defaultValue();
	QString minimum();
	QString maximum();
	
	public slots:
	void minMaxChange(int);
	void minMaxChange(bool);
	
	private:
	GofunDecimalSpinBox* spin_default;
	QCheckBox* set_minimum;
	GofunDecimalSpinBox* spin_minimum;
	QCheckBox* set_maximum;
	GofunDecimalSpinBox* spin_maximum;
};

#endif


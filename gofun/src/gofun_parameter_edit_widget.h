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

#include <qgroupbox.h>
 
#ifndef GOFUN_PARAMETER_EDIT_WIDGET
#define GOFUN_PARAMETER_EDIT_WIDGET

class GofunParameterEditWidget : public QGroupBox
{
	public:
	GofunParameterEditWidget(QWidget*);
	virtual void setDefaultValue(const QString&) = 0;
	virtual void setValues(const QStringList&) {};
	virtual void setMaximum(const QString&) {};
	virtual void setMinimum(const QString&) {};
	
	virtual QString defaultValue() = 0;
	virtual QStringList values() { return QStringList(); };
	virtual QString maximum() { return QString(); }
	virtual QString minimum() { return QString(); }
};

#endif

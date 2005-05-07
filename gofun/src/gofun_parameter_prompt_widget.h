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

#include "gofun_application_entry_data.h"
 
#ifndef GOFUN_PARAMETER_PROMPT_WIDGET
#define GOFUN_PARAMETER_PROMPT_WIDGET

class QGridLayout;

struct GofunParameterPromptWidget : public QWidget
{
	GofunParameterPromptWidget(QWidget*parent) : QWidget(parent) {};
	virtual void setParameterData(const GofunParameterData& _par_data) { par_data = _par_data; }
	virtual QString returnParameter() { return par_data.Flag + " " + par_data.Default_Value; }
	GofunParameterData par_data;

	QGridLayout* grid;
};

#endif


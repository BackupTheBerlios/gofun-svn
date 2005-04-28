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

#include <math.h>
 
#include <climits>

#include <qvalidator.h>

#include "gofun_decimal_spinbox.h"

GofunDecimalSpinBox::GofunDecimalSpinBox(QWidget* parent) : QSpinBox(parent)
{
	precision = 2;
	setMinValue(0);
	setMaxValue(INT_MAX);
	validator = new QDoubleValidator(0,INT_MAX,precision,this);
	setValidator(validator);
}

QString GofunDecimalSpinBox::mapValueToText(int value)
{
	/*if ( value == -1 ) // special case
		return QString( "Auto" );

	return QString( "%1.%2" ).arg( value / 10 ).arg( value % 10 );  // 0.0 to 10.0*/
	double dvalue = ((double)value)/pow(10,precision);
	return QString().setNum(dvalue,'g',precision+10);
}

int GofunDecimalSpinBox::mapTextToValue(bool * ok)
{
	/*if ( text() == "Auto" ) // special case
		return -1;*/
	return mapTextToValue(text());
}

void GofunDecimalSpinBox::setPrecision(unsigned int i)
{
	precision = i;
	validator->setDecimals(precision);
}

int GofunDecimalSpinBox::mapTextToValue(const QString& text)
{
	return (int)(pow(10,precision) * text.toDouble());
}


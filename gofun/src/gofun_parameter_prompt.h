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

#include <vector>
 
#include <qdialog.h>

#include "gofun_data.h"
 
#ifndef GOFUN_PARAMETER_PROMPT
#define GOFUN_PARAMETER_PROMPT

class QGridLayout;
class QComboBox;
class QSpinBox;
class QSlider;
class GofunDecimalSpinBox;

struct GofunParameterPromptWidget : public QWidget
{
	GofunParameterPromptWidget(QWidget*parent) : QWidget(parent) {};
	virtual void setParameterData(const GofunParameterData& _par_data) { par_data = _par_data; }
	virtual QString returnParameter() { return par_data.Flag + " " + par_data.Default_Value; }
	GofunParameterData par_data;
};

struct GofunParameterStringPromptWidget : public GofunParameterPromptWidget
{
	GofunParameterStringPromptWidget(QWidget*);
	void setParameterData(const GofunParameterData&);
	QString returnParameter();
	
	QComboBox* value;
};

struct GofunParameterIntegerPromptWidget : public GofunParameterPromptWidget
{
	GofunParameterIntegerPromptWidget(QWidget*);
	void setParameterData(const GofunParameterData&);
	QString returnParameter();
	
	QSpinBox* value_spin;
	QSlider* value_slider;
};

struct GofunParameterDecimalPromptWidget : public GofunParameterPromptWidget
{
	GofunParameterDecimalPromptWidget(QWidget*);
	void setParameterData(const GofunParameterData&);
	QString returnParameter();
	
	GofunDecimalSpinBox* value_spin;
	QSlider* value_slider;
};

class GofunParameterPrompt : public QDialog
{
	Q_OBJECT
	
	public:
	GofunParameterPrompt();	
	void addParameter(GofunParameterData*);
	QString parameterString();
	
	private:
	QGridLayout* grid;
	std::vector<GofunParameterPromptWidget*> parameter;
};

#endif


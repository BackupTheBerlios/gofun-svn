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
 
#ifndef GOFUN_PARAMETER_EDIT
#define GOFUN_PARAMETER_EDIT

class GofunDecimalSpinBox : public QSpinBox
{
	Q_OBJECT
	public:
	GofunDecimalSpinBox(QWidget*parent) : QSpinBox(parent) {}

	QString mapValueToText(int value);
	int mapTextToValue(bool *ok);
};

class GofunParameterTypeEdit : public QGroupBox
{
	public:
	GofunParameterTypeEdit(QWidget*);
	virtual void setDefaultValue(const QString&) = 0;
	virtual void setValues(const QStringList&) {};
	virtual void setMaximum(const QString&) {};
	virtual void setMinimum(const QString&) {};
	
	virtual QString defaultValue() = 0;
	virtual QStringList values() { return QStringList(); };
	virtual QString maximum() { return QString(); }
	virtual QString minimum() { return QString(); }
};

class GofunParameterTypeStringEdit : public GofunParameterTypeEdit
{
	Q_OBJECT
	
	public:
	GofunParameterTypeStringEdit(QWidget*);
	void setDefaultValue(const QString&);
	void setValues(const QStringList&);
	QStringList values();
	QString defaultValue();
	
	public slots:
	void updateCombo();
	
	private:
	GofunListWidget* value_list;
	QComboBox* default_value;
};

class GofunParameterTypeIntegerEdit : public GofunParameterTypeEdit
{
	Q_OBJECT
	
	public:
	GofunParameterTypeIntegerEdit(QWidget*);
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
	GofunListWidget* value_list;
	QSpinBox* spin_default;
	QCheckBox* set_minimum;
	QSpinBox* spin_minimum;
	QCheckBox* set_maximum;
	QSpinBox* spin_maximum;
};

class GofunParameterTypeDecimalEdit : public GofunParameterTypeEdit
{
	Q_OBJECT
	
	public:
	GofunParameterTypeDecimalEdit(QWidget*);
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
	GofunListWidget* value_list;
	GofunDecimalSpinBox* spin_default;
	QCheckBox* set_minimum;
	GofunDecimalSpinBox* spin_minimum;
	QCheckBox* set_maximum;
	GofunDecimalSpinBox* spin_maximum;
};

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
	GofunParameterTypeEdit* type_edit;
	
	GofunParameterData par_data;
};

#endif

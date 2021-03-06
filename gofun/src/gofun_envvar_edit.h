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

#include <qdialog.h>
#include <qlineedit.h>

#ifndef GOFUN_ENVVAR_EDIT
#define GOFUN_ENVVAR_EDIT

class QLineEdit;

class GofunEnvVarEdit : public QDialog
{
	Q_OBJECT

	public:
	GofunEnvVarEdit(QWidget*);
	
	void setName(const QString&);
	void setValue(const QString&);
	QString getName();
	QString getValue();

	private:
	QLineEdit* name_le;
	QLineEdit* value_le;
};

class GofunInterpretedLineEdit : public QLineEdit
{
	Q_OBJECT
	
	public:
	GofunInterpretedLineEdit(QWidget*);
	
	public slots:
	void setText(const QString&);
};

#endif

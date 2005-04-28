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

#include <vector>
 
#include "gofun_locale_string.h"
#include "gofun_desktop_entry_data.h"

class QProcess;
 
#ifndef GOFUN_APPLICATION_ENTRY_DATA
#define GOFUN_APPLICATION_ENTRY_DATA

///Data-type describing parameters in a Desktop Entry
struct GofunParameterData
{
	QString Flag;
	QStringList Values;
	QString Default_Value;
	QString Prompt;
	QString Type;
	QString Minimum;
	QString Maximum;
	GofunLocaleString Comment;
};

///Data-type for application Desktop Entries
struct GofunApplicationEntryData : public GofunDesktopEntryData
{
	QString Exec;
	QString TryExec;
	QString Path;
	QString Terminal;
	std::vector<QString> X_GoFun_Env;
	QString X_GoFun_User;
	QString X_GoFun_NewX;
	std::map<int,GofunParameterData> X_GoFun_Parameter;
	
	///Parse a line of the Desktop Entry.
	bool parseLine(const QString&);
	///Execute the application
	void execute();
	///Save the Desktop Entry
	void save();
	
	virtual GofunApplicationEntryData* makeCopy();
	
	private:
	///Adds arguments to a process by splitting a string
	void addSplittedProcArgument(QProcess*,const QString&);
	///Saves process arguments
	/** The filename is returned */
	QString saveProcArguments(QProcess*);
	///Saves a stringlist to a file
	QString saveStringListToFile(const QString&,QStringList*);
};

#endif


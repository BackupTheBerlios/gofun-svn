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

#include <qfile.h>
#include <qprocess.h>
#include <qiconview.h>

#include "gofun.h"
#include "gofun_data.h"

#ifndef GOFUN_ITEM
#define GOFUN_ITEM

class GofunItem : public QIconViewItem
{
public:
	GofunItem(QIconView*, const QString& = 0);

	void loadIcon();
	void setData(GofunItemData*);
	void save();
	void deleteEntry();
	void executeCommand(ExecuteOption* = NULL);
	
	GofunItemData* data;
private:
	QString saveProcArguments(QProcess*);
	void interpretExecString(QString&);
};

#endif
      

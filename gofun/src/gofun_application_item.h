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

#include "gofun_item.h"
#include "gofun_application_entry_data.h"

#ifndef GOFUN_APPLICATION_ITEM
#define GOFUN_APPLICATION_ITEM

class QIconView;

enum
{
  PID_Execute = PID_Delete + 1,
  PID_Execute_in_terminal,
  PID_Execute_with_xinit,
  PID_Open_directory,
  PID_Customized_start
};

class GofunApplicationItem : public GofunItem
{
	Q_OBJECT
	
	public:
	GofunApplicationItem(QIconView*, const QString& = 0);
	GofunApplicationItem(QIconView*, const QString&, GofunDesktopEntryData*);
	//virtual ~GofunApplicationItem();

	void setData(GofunDesktopEntryData*);
	void save();
	void customizedStart();
	void openDirectory();
	void executeCommand();
	QPopupMenu* rightClickPopup(const QPoint&);
	void editEntry();
	void performDefaultAction() { execute(); };
	static void createNewItem(QIconView*);
	
	GofunApplicationEntryData* data() { return m_data; }
	
	public slots:
	void popupActivated(int);
	private:
	void execute(const QString& = QString::null);
	
	GofunApplicationEntryData* m_data;
};

#endif


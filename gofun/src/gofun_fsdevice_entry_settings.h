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

#include "gofun_desktop_entry_settings.h"
#include "gofun_fsdevice_entry_data.h"
 
#ifndef GOFUN_FSDEVICE_ITEM_SETTINGS
#define GOFUN_FSDEVICE_ITEM_SETTINGS

///Settings-dialog for fsdevice entries
class GofunFSDeviceEntrySettings : public GofunDesktopEntrySettings
{
	Q_OBJECT
public:
	GofunFSDeviceEntrySettings();
	void load(GofunFSDeviceEntryData*);
	void setDefaults();
	
private slots:
	void iconDialog();
	void dirDialog();
	void deviceDialog();
	void mountPointDirectoryDialog();
	void unmountIconDialog();
	void selectedDevice(const QString&);
private:
	void save();
	void apply();
	bool inputValid();

	GofunFSDeviceEntryData* data();
	
	QLineEdit* unmount_icon;
	QToolButton* unmount_icon_button;
	QCheckBox* readonly_chk;
	QComboBox* device;
	QComboBox* mount_point;
	QComboBox* type;
};

#endif

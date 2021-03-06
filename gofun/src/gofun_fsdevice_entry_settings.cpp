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
 
#include <mntent.h>
 
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
 
#include "gofun_fsdevice_entry_settings.h"
#include "gofun_fsdevice_item.h"
#include "gofun_misc.h"
#include "gofun_shell_operations.h"
#include "gofun_directory_button.h"
#include "gofun_desktop_entry_settings_widget.h"
#include "gofun_directory_browser.h"
#include "gofun_icon_dialog.h"

GofunFSDeviceEntrySettings::GofunFSDeviceEntrySettings()
{
	QGridLayout* grid = new QGridLayout(widget_main,3,3);

	device = new QComboBox(true,widget_main);
	QToolButton* device_button = new QToolButton(widget_main);
	mount_point = new QComboBox(true,widget_main);
	mount_point->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	QToolButton* mount_point_button = new QToolButton(widget_main);
		
	FILE* fp = setmntent("/etc/fstab","r");
	struct mntent* me;
	while((me = getmntent(fp)) != NULL)
	{
		if(QString(me->mnt_fsname) != "none")
			device->insertItem(me->mnt_fsname);
		if(QString(me->mnt_dir).find("/dev") != 0 && QString(me->mnt_dir) != "/proc")
			mount_point->insertItem(me->mnt_dir);
	}
	endmntent(fp);
	
	grid->addMultiCellWidget(desw,0,0,0,2);
	grid->addWidget(new QLabel(tr("Device"),widget_main),1,0);
	grid->addWidget(device,1,1);
	grid->addWidget(device_button,1,2);
	grid->addWidget(new QLabel(tr("MountPoint"),widget_main),2,0);
	grid->addWidget(mount_point,2,1);
	grid->addWidget(mount_point_button,2,2);
	
	connect(mount_point_button,SIGNAL(clicked()),this,SLOT(mountPointDirectoryDialog()));
	connect(device_button,SIGNAL(clicked()),this,SLOT(deviceDialog()));
	connect(device,SIGNAL(activated(const QString&)),this,SLOT(selectedDevice(const QString&)));
	connect(device,SIGNAL(textChanged(const QString&)),this,SLOT(selectedDevice(const QString& )));
	
	QWidget* widget_advanced = new QWidget(this);
	QGridLayout* grid_adv = new QGridLayout(widget_advanced,3,3);
	
	tabwidget->addTab(widget_advanced,tr("Advanced"));
	
	type = new QComboBox(widget_advanced);
	QStringList filesystems = QStringList::split('\n',GofunShellOperations::shellCall("cat /proc/filesystems | sed -e 's/nodev//' | sed -e 's/\\t//'"));
	filesystems.sort();
	type->insertItem("auto");
	type->insertStringList(filesystems);
	unmount_icon = new QLineEdit(widget_advanced);
	unmount_icon_button = new QToolButton(widget_advanced);
	readonly_chk = new QCheckBox(tr("Use filesystem in ReadOnly mode"),widget_advanced);
	grid_adv->addWidget(new QLabel(tr("UnmountIcon"),widget_advanced),0,0);
	grid_adv->addWidget(unmount_icon,0,1);
	grid_adv->addWidget(unmount_icon_button,0,2);
	grid_adv->addWidget(new QLabel(tr("Filesystem"),widget_advanced),1,0);
	grid_adv->addWidget(type,1,1);
	grid_adv->addMultiCellWidget(readonly_chk,2,2,0,2);

	connect(unmount_icon_button, SIGNAL(clicked()),this, SLOT(unmountIconDialog()));
	
	item = 0;
}

void GofunFSDeviceEntrySettings::deviceDialog()
{
	QString dev_str = QFileDialog::getOpenFileName("/dev/","*",this,"",tr("Select device"));
	if(!dev_str.isNull())
		device->setCurrentText(dev_str);
}

void GofunFSDeviceEntrySettings::mountPointDirectoryDialog()
{
	QString start_dir;
	if(!mount_point->currentText().isEmpty())
		start_dir = GofunShellOperations::extendFileString(mount_point->currentText());
	else
		start_dir = "/mnt";
	
	GofunDirectoryBrowser dir_browser;
	dir_browser.setStartDirectory(start_dir);
	if(dir_browser.exec() == QDialog::Accepted)
		mount_point->setCurrentText(dir_browser.selected());
}

void GofunFSDeviceEntrySettings::load(GofunFSDeviceEntryData* _item)
{
	GofunDesktopEntrySettings::load(_item);
	
	unmount_icon->setText(data()->UnmountIcon);
	unmount_icon_button->setPixmap(GofunMisc::getIcon(data()->UnmountIcon,32,32));
	
	device->setCurrentText(data()->Device);
	mount_point->setCurrentText(data()->MountPoint);
	
	type->setCurrentText(data()->FSType);
	
	readonly_chk->setChecked(GofunMisc::stringToBool(data()->ReadOnly));
}

void GofunFSDeviceEntrySettings::dirDialog()
{
}

void GofunFSDeviceEntrySettings::save()
{
	GofunDesktopEntrySettings::save();
}

void GofunFSDeviceEntrySettings::apply()
{
	if(!item)
		item = new GofunFSDeviceEntryData;
		
	GofunDesktopEntrySettings::apply();


	if(data()->Type.isEmpty())
		data()->Type = "FSDevice";
	
	data()->Device = device->currentText();
	data()->MountPoint = mount_point->currentText();
	
	data()->UnmountIcon = unmount_icon->text();
	data()->ReadOnly = GofunMisc::boolToString(readonly_chk->isChecked());
	data()->FSType = type->currentText();
}

bool GofunFSDeviceEntrySettings::inputValid()
{
	if(!GofunDesktopEntrySettings::inputValid())
		return false;
	else
		return true;
}

GofunFSDeviceEntryData* GofunFSDeviceEntrySettings::data()
{
	return dynamic_cast<GofunFSDeviceEntryData*>(item);
}

void GofunFSDeviceEntrySettings::unmountIconDialog()
{
	GofunIconDialog* id = new GofunIconDialog();
	QString start_dir;
	if(!unmount_icon->text().isEmpty())
	{
		start_dir = unmount_icon->text();
		id->setStartIcon(unmount_icon->text());
	}
	if(!start_dir.isEmpty())
		start_dir = "/usr/share/icons";
	id->setStartDir(start_dir);
	if(id->exec() == QDialog::Accepted)
	{
		unmount_icon->setText(id->selected());
		unmount_icon_button->setPixmap(GofunMisc::getIcon(id->selected()));
	}
	delete id;
}

void GofunFSDeviceEntrySettings::iconDialog()
{
	GofunDesktopEntrySettings::iconDialog();
	
	QString unfile =  desw->icon;
	unfile.replace("mount","unmount");
	if(GofunMisc::getIcon(unfile).isNull())
		unfile = QDir(desw->icon).path() + "/un" + QFileInfo(desw->icon).fileName();
	if(GofunMisc::getIcon(unfile).isNull())
		unfile.replace("mount","unmount");
	if(!GofunMisc::getIcon(unfile).isNull())	
	{
		unmount_icon->setText(unfile);
		unmount_icon_button->setPixmap(GofunMisc::getIcon(unfile));
	}	
}

void GofunFSDeviceEntrySettings::selectedDevice(const QString& device)
{
	FILE* fp = setmntent("/etc/fstab","r");
	struct mntent* me;
	while((me = getmntent(fp)) != NULL)
	{
		if(me->mnt_fsname == device)
		{
			mount_point->setCurrentText(me->mnt_dir);
			break;
		}
	}
	endmntent(fp);
}

void GofunFSDeviceEntrySettings::setDefaults()
{
	GofunDesktopEntrySettings::setDefaults();

	desw->setIcon("default_device.png");
}




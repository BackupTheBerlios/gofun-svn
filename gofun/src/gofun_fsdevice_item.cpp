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

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
 
#include "gofun_fsdevice_item.h"
#include "gofun_fsdevice_item_settings.h"
#include "gofun_misc.h"
#include "gofun_settings.h"

GofunFSDeviceItem::GofunFSDeviceItem(GofunIconView* iconview, const QString& string) : GofunItem(iconview, string)
{
	m_data = new GofunFSDeviceEntryData();
}

//Open dialog for editing a Desktop Entry.
void GofunFSDeviceItem::editEntry()
{
	GofunFSDeviceItemSettings* settings_dlg = new GofunFSDeviceItemSettings();
	GofunMisc::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,375,200);
	settings_dlg->setCaption(tr("Edit entry"));
	settings_dlg->load(this);
	settings_dlg->exec();
}

void GofunFSDeviceItem::setData(GofunDesktopEntryData* d)
{
	delete m_data;
	m_data = dynamic_cast<GofunFSDeviceEntryData*>(d);
	loadIcon();
	if(!data()->Comment.isEmpty())
		setToolTipText(data()->Comment);
}

void GofunFSDeviceItem::loadIcon()
{
	if(isMounted())
		GofunItem::loadIcon();
	else
	{
		QPixmap px = GofunMisc::get_icon(data()->UnmountIcon,32,32);
		if(!px.isNull())
			setPixmap(px);
	}
}

QPopupMenu* GofunFSDeviceItem::rightClickPopup(const QPoint& pos)
{
	QPopupMenu* popup = GofunItem::rightClickPopup(pos);
		
	
	popup->insertItem(tr("Open"),PID_Open,0);
	popup->insertSeparator(1);
	if(isMounted())
	popup->insertItem(tr("Unmount"),PID_Unmount,2);
	else
	popup->insertItem(tr("Mount"),PID_Mount,2);
	
	popup->popup(pos);
	
	return popup;
}

void GofunFSDeviceItem::popupActivated(int id)
{
	GofunItem::popupActivated(id);

	//if(GofunItem* item = (GofunItemdynamic_cast<GofunIconView*>(view_ws->visibleWidget()))->currentItem() )
	{
		switch(id)
		{
			case PID_Open:
				open(); break;
			case PID_Mount:
				mount(); break;
			case PID_Unmount:
				unMount(); break;
		}
	}
}

void GofunFSDeviceItem::save()
{
	GofunDesktopObject::save();
	GofunItem::save();

	QFile file( data()->File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "Type=FSDevice\n";
		stream << "Dev=" << data()->Device << "\n";
		stream << "FSType=" << data()->FSType << "\n";
		stream << "MountPoint=" << data()->MountPoint << "\n";
		stream << "ReadOnly=" << data()->ReadOnly << "\n";
		stream << data()->Unknownkeys.join("\n") << "\n";
		file.close();
	}
}

void GofunFSDeviceItem::performDefaultAction()
{
	open();
}

bool GofunFSDeviceItem::isMounted()
{
	FILE* fp = setmntent("/etc/mtab","r");
	struct mntent* me;
	while((me = getmntent(fp)) != NULL)
	{
		if(me->mnt_dir == data()->MountPoint)
		{
			endmntent(fp);
			return true;
		}
	}
	endmntent(fp);
	return false;
}

void GofunFSDeviceItem::open() //@TODO resolve code duplication between this and GofunApplicationItem::openDirectory()
{
	if(!isMounted())
		mount();
	if(!isMounted())
		return;

	QProcess proc(GSC::get()->filemanager_cmd);
	if(!data()->MountPoint.isEmpty())
		proc.addArgument((GofunMisc::ext_filestring(data()->MountPoint)));
	else
		proc.addArgument(QDir::homeDirPath());
	if(!proc.start())
	{
		std::cout<<QObject::tr("Execution of directory viewer failed. :(\n");
	}	
}

void GofunFSDeviceItem::mount()
{
	QString tmp = GofunMisc::shell_call("mount " + QString(GofunMisc::stringToBool(data()->ReadOnly) ? "-r " : "") + data()->MountPoint+" 2>&1");
	if(!isMounted())
		QMessageBox::warning(0,tr("Mount error"),tr("Mounting failed:\n")+tmp);
	else
		loadIcon();
}

void GofunFSDeviceItem::unMount()
{
	QString tmp = GofunMisc::shell_call("umount "+data()->MountPoint+" 2>&1");
	if(isMounted())
		QMessageBox::warning(0,tr("Unmount error"),tr("Unmounting failed:\n")+tmp);
	else
		loadIcon();
}


void GofunFSDeviceItem::createNewItem(GofunCatButton* cat)
{
	GofunFSDeviceItemSettings* settings_dlg = new GofunFSDeviceItemSettings();
	int height = 200;
	GofunMisc::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->setCategory(cat);
	settings_dlg->exec();
	delete settings_dlg;
}




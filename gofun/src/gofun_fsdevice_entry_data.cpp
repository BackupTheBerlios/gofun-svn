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

#include <mntent.h>
 
#include <qfile.h>
#include <qprocess.h>
#include <qapplication.h>
#include <qmessagebox.h>
 
#include "gofun_fsdevice_entry_data.h"
#include "gofun_settings_container.h"
#include "gofun_data.h"
#include "gofun_misc.h"

/*GofunDesktopObject* GofunFSDeviceEntryData::GofunDesktopObjectFactory(QWidget* parent)
{
	GofunFSDeviceItem* item = new GofunFSDeviceItem(dynamic_cast<GofunIconView*>(parent),Name);
	item->setData(this);
	return item;
}*/

bool GofunFSDeviceEntryData::parseLine(const QString& line)
{
	if(GofunDesktopEntryData::parseLine(line))
		return true;
		
	if(GofunDataLoader::parseLine("Dev",line,Device)
	|| GofunDataLoader::parseLine("FSType",line,FSType)
	|| GofunDataLoader::parseLine("MountPoint",line,MountPoint)
	|| GofunDataLoader::parseLine("ReadOnly",line,ReadOnly)
	|| GofunDataLoader::parseLine("UnmountIcon",line,UnmountIcon))
		return true;
	return false;
}

GofunFSDeviceEntryData * GofunFSDeviceEntryData::makeCopy( )
{
	GofunFSDeviceEntryData* copy = new GofunFSDeviceEntryData;
	*copy = *this;
	return copy;
}

void GofunFSDeviceEntryData::save()
{
	GofunDesktopEntryData::save();

	QFile file( File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "Type=FSDevice\n";
		if(!Device.isEmpty())
			stream << "Dev=" << Device << "\n";
		if(!FSType.isEmpty())
			stream << "FSType=" << FSType << "\n";
		if(!MountPoint.isEmpty())
			stream << "MountPoint=" << MountPoint << "\n";
		if(!UnmountIcon.isEmpty())
			stream << "UnmountIcon=" << UnmountIcon << "\n";
		stream << "ReadOnly=" << ReadOnly << "\n";
		stream << Unknownkeys.join("\n") << "\n";
		file.close();
	}
}

bool GofunFSDeviceEntryData::isMounted()
{
	if(getMntEnt())
		return true;
	else
		return false;
}

bool GofunFSDeviceEntryData::open() //@TODO resolve code duplication between this and GofunApplicationItem::openDirectory()
{
	if(!isMounted())
		mount();
	if(!isMounted())
		return false;

	QProcess proc(GSC::get()->filemanager_cmd);
	proc.addArgument(getMountPoint());
	if(!proc.start())
	{
		qDebug(QObject::tr("Execution of directory viewer failed. :(\n"));
	}	
}

bool GofunFSDeviceEntryData::mount()
{
	QString shell_call;
	shell_call += "mount ";
	shell_call += QString(GofunMisc::stringToBool(ReadOnly) ? "-r " : " ");
	
	if((!Device.isEmpty() && !MountPoint.isEmpty()) || (!FSType.isEmpty() && FSType != "auto")) //When we give 'mount' both it'll end up in an error (only root bla) //fixme: this can just be seen as workaround
	{
		bool solved = false;
		bool cmd_cmplt = false;
		FILE* fp = setmntent("/etc/fstab","r");
		struct mntent* me;
		while((me = getmntent(fp)) != NULL)
		{
			if((me->mnt_dir == MountPoint && me->mnt_fsname == Device)
			|| (me->mnt_dir == MountPoint && !FSType.isEmpty())
			|| (me->mnt_fsname == Device && !FSType.isEmpty()))
			{
				solved = true;
				if(me->mnt_type != FSType && !FSType.isEmpty())
				{
					shell_call += QString(FSType.isEmpty() ? " " : ("-t "+FSType+" "));
					if(Device.isEmpty())
						shell_call += QString(me->mnt_fsname) + " ";
					else
						shell_call += Device + " ";
					if(MountPoint.isEmpty())
						shell_call += QString(me->mnt_dir) + " ";
					else
						shell_call += MountPoint + " ";
					solved = false;
					cmd_cmplt = true;
					break;
				}
				if(!Device.isEmpty())
					shell_call += Device + " ";
				else
					shell_call += MountPoint;
				cmd_cmplt  = true;
				break;
			}
		}
		endmntent(fp);
		
		if(!solved)
		{
			if(!cmd_cmplt)
			{
				shell_call += QString(FSType.isEmpty() ? " " : ("-t "+FSType+" "));
				shell_call += Device + " ";
				shell_call += MountPoint + " ";
			}
			
			qDebug(shell_call);
		
			QProcess proc_gosu;
		
			proc_gosu.addArgument("gosu");
			proc_gosu.addArgument("root");
			proc_gosu.addArgument("--color");
			proc_gosu.addArgument(qApp->palette().color(QPalette::Active,QColorGroup::Background).name());
			proc_gosu.addArgument("-l");
			proc_gosu.addArgument("-c");
			proc_gosu.addArgument(shell_call);
			proc_gosu.start();
			return true;
		}
	}
	else
	{
		shell_call += Device + " ";
		shell_call += MountPoint + " ";
	}
	
	shell_call += " 2>&1";

	QString tmp = GofunMisc::shell_call(shell_call);
	if(!isMounted())
	{
		QMessageBox::warning(0,QObject::tr("Mount error"),QObject::tr("Mount failed:\n")+tmp);
		return false;
	}
	else
		return true;
}

bool GofunFSDeviceEntryData::unMount()
{
	QString tmp = GofunMisc::shell_call("umount "+getMountPoint()+" 2>&1");
	if(isMounted())
	{
		QMessageBox::warning(0,QObject::tr("Unmount error"),QObject::tr("Unmounting failed:\n")+tmp);
		return false;
	}
	else
		return true;
}

QString GofunFSDeviceEntryData::noLinkGuarantuee(const QString& possible_symlink)
{
	QFileInfo info(possible_symlink);
	if(info.isSymLink())
	{
		QFileInfo tmp(info.readLink());
		if(!tmp.isRelative())
			return tmp.filePath();
		else
			return info.dirPath() + "/" + tmp.filePath();
	}
	else
		return info.filePath();
}

QString GofunFSDeviceEntryData::getMountPoint()
{
	if(mntent* me = getMntEnt())
		return me->mnt_dir;
	else
		return QString::null;
}

QString GofunFSDeviceEntryData::getDevice()
{
	if(mntent* me = getMntEnt())
		return me->mnt_fsname;
	else
		return QString::null;
}

mntent* GofunFSDeviceEntryData::getMntEnt()
{
	FILE* fp = setmntent("/etc/mtab","r");
	struct mntent* me;
	while((me = getmntent(fp)) != NULL)
	{
		if(noLinkGuarantuee(me->mnt_dir) == noLinkGuarantuee(MountPoint) || noLinkGuarantuee(me->mnt_fsname) == noLinkGuarantuee(Device))
		{
			endmntent(fp);
			return me;
		}
	}
	endmntent(fp);
	return 0;
}



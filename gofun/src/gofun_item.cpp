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

#include <iostream> 

#include <qapplication.h>
#include <qinputdialog.h>
#include <qtooltip.h>

#include "gofun_misc.h"
#include "gofun_item.h"
#include "gofun.h"
#include "gofun_data.h"
#include "gofun_iconview.h"
#include "gofun_settings.h"
#include "gofun_parameter_prompt.h"

GofunItem* GofunIconViewToolTip::last_active = 0;
 
GofunItem::GofunItem(GofunIconView* iconview, const QString& string) : QIconViewItem(iconview,string)
{
	m_data = new GofunItemData();
	toolTip = NULL;
}

/*GofunItem::~GofunItem()
{
	delete data;
}*/

void GofunItem::save()
{
	GofunDesktopObject::save();

	QFile file( data()->File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		stream << "Type=Application\n";
		stream << "Exec=" << data()->Exec << "\n";
		stream << "Path=" << data()->Path << "\n";
		stream << "Terminal=" << data()->Terminal << "\n";
		stream << "X-GoFun-NewX=" << data()->X_GoFun_NewX << "\n";
		if(!data()->X_GoFun_Env.empty())
		{
			stream << "X-GoFun-Env=";
			for(std::vector<QString>::iterator it = data()->X_GoFun_Env.begin(); it != data()->X_GoFun_Env.end(); ++it)
			{
				stream << "\t" << (*it);
			}
			stream << "\n";
		}
		stream << "X-GoFun-User=" << data()->X_GoFun_User << "\n";
		stream << data()->Unknownkeys << "\n";
		file.close();
	}
}

void GofunItem::deleteEntry()
{
	QProcess proc;
	proc.addArgument("rm");
	proc.addArgument("-f");
	proc.addArgument(data()->File);
	proc.start();
	while(proc.isRunning())
	{
	}
	delete this;
}

void GofunItem::setData(GofunItemData* d)
{
	delete m_data;
	m_data = d;
	loadIcon();
	if(!data()->Comment.isEmpty())
		setToolTipText(data()->Comment);
}

void GofunItem::loadIcon()
{
	QPixmap px = GofunMisc::get_icon(data()->Icon,32,32);
	if(!px.isNull())
		setPixmap(px);
}

void GofunItem::interpretExecString(QString& exec)
{
	int off = -1;
	while((off = exec.find("$GP",off+1)) != -1)
	{
		if(exec.find("$GPN") == off)
		{
			int i = QInputDialog::getInteger("Enter a number","Enter a number");
			exec.remove(off,off+QString("$GPN").length());
			exec.insert(off,QString::number(i));
		}
		else if(exec.find("$GPT") == off)
		{
		}
	}
}

void GofunItem::executeCommand(ExecuteOption* option)
{
	QString poststring;
	QProcess proc;
	
	QString exec;
	if(!option->Exec.isEmpty())
	exec = option->Exec;
	else
	exec = data()->Exec;
	
	interpretExecString(exec);
	
	if(!data()->X_GoFun_Parameter.empty())
	{
		GofunParameterPrompt* prompt = new GofunParameterPrompt();
		for(std::map<int,GofunParameterData>::iterator it = data()->X_GoFun_Parameter.begin();it != data()->X_GoFun_Parameter.end(); ++it)
		{
			prompt->addParameter(&((*it).second));
		}
		if(prompt->exec() == QDialog::Accepted)
			exec += prompt->parameterString();
	}
	if(!data()->X_GoFun_Env.empty())
	{
		for(std::vector<QString>::iterator it = data()->X_GoFun_Env.begin(); it != data()->X_GoFun_Env.end(); ++it)
		{
			if((*it).isEmpty())
			{
				continue;
			}
			QStringList vk_pair = QStringList::split('=',(*it));
			exec = "export " + vk_pair[0] + "='" + QString((*it)).remove(0,vk_pair[0].length()+1) + "';" + exec;
		}
	}
	if((!option->terminal.isEmpty()) || (data()->Terminal == "true"))
	{ 
		addSplittedProcArgument(&proc,GSC::get()->terminal_cmd);
		exec += ";echo -e \"\\E[${2:-44};${3:-7}m\n" + QObject::tr("End of execution has been reached. Press any key to remove this terminal\";");
		if(!(!option->xinit.isEmpty()) || (data()->X_GoFun_NewX == "true")) //FIXME
			exec += "read evar";
		
	}
	if((!option->xinit.isEmpty()) || (data()->X_GoFun_NewX == "true"))
	{
		proc.addArgument("xinit");
	}
	if(data()->Path.isEmpty())
	{
		proc.setWorkingDirectory(QDir::homeDirPath());
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		exec = "cd " + GofunMisc::shellify_path(QDir::homeDirPath()) + ";" + exec;
		proc.addArgument(exec);
	}
	else
	{
		proc.setWorkingDirectory(QDir(data()->Path));
		proc.addArgument("/bin/sh");
		proc.addArgument("-c");
		exec = "cd " + GofunMisc::shellify_path(data()->Path) + ";" + exec;
		proc.addArgument(exec);
	}
	if((!option->xinit.isEmpty()) || (data()->X_GoFun_NewX == "true"))
	{
		proc.addArgument("--");
		QString xservnum = GofunMisc::shell_call("ps -Ac | grep X | wc -l");
		xservnum = xservnum.simplifyWhiteSpace();
		proc.addArgument(":"+xservnum);

	}
	if(!data()->X_GoFun_User.isEmpty())
	{		
		QString spa_file = saveProcArguments(&proc);
		
		QProcess proc_gosu;
		proc_gosu.addArgument("gosu");
		proc_gosu.addArgument(data()->X_GoFun_User);
		proc_gosu.addArgument("--color");
		proc_gosu.addArgument(qApp->palette().color(QPalette::Active,QColorGroup::Background).name());
		proc_gosu.addArgument("-l");
		proc_gosu.addArgument("-g");
		proc_gosu.start();
		
	}
	else
	{
		proc.start();
	}
}

void GofunItem::addSplittedProcArgument(QProcess* proc,const QString& argument)
{
	QStringList arguments = QStringList::split(' ',argument);
	for(QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		proc->addArgument((*it));
	}
}

QString GofunItem::saveProcArguments(QProcess* proc)
{
	QStringList arguments = proc->arguments();
	QString tmp = QString(getenv("HOME")) + QString("/.gofun/tmp_proc_exec"); //You'll probably wonder "wtf is this tmp needed"? The answer is "cause gcc is mucking around without this ugly code on some systems"
	QFile file(tmp);
	if(file.open( IO_WriteOnly ))
	{
		QTextStream stream(&file);
		for(QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it)
		{
			stream << (*it) << '\0';
		}
		file.close();
	}
	return file.name();
}

void GofunItem::setToolTipText(const QString text)
{
	toolTipText = text;
	toolTip = new GofunIconViewToolTip(iconView());
}

GofunIconViewToolTip::GofunIconViewToolTip(QIconView *lParent, QToolTipGroup* lGroup)
	:QToolTip(lParent->viewport(), lGroup), parent(lParent)
{
}

GofunIconViewToolTip::~GofunIconViewToolTip(void)
{
}

void GofunIconViewToolTip::maybeTip(const QPoint &p)
{
	GofunItem* item;
	QPoint absCoords;

	// get absolute item coords
	absCoords.setX(p.x()+parent->contentsX());
	absCoords.setY(p.y()+parent->contentsY());
	item=dynamic_cast<GofunItem*>(parent->findItem(absCoords));
	if(!item)
		last_active = 0;
	if (item && item != last_active)
	{
		last_active = item;
		QRect relCoords;

		// use portview-relative coords
		relCoords.setX(p.x());
		relCoords.setY(p.y());
		relCoords.setWidth(item->rect().width());
		relCoords.setHeight(item->rect().height());
		tip(relCoords, item->getToolTipText());
	}
}


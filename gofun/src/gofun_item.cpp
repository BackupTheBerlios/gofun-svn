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
#include <qpopupmenu.h>
#include <qmessagebox.h>

#include "gofun_misc.h"
#include "gofun_item.h"
#include "gofun.h"
#include "gofun_data.h"
#include "gofun_iconview.h"
#include "gofun_settings.h"
#include "gofun_parameter_prompt.h"
#include "gofun_item_settings.h"

GofunItem* GofunIconViewToolTip::last_active = 0;
 
GofunItem::GofunItem(GofunIconView* iconview, const QString& string) : QIconViewItem(iconview,string)
{
	m_data = new GofunDesktopEntryData();
	toolTip = NULL;
}

GofunItem::~GofunItem()
{
	delete data();
	delete toolTip;
}

void GofunItem::save()
{
	GofunDesktopObject::save();

	QFile file( data()->File );
	if ( file.open( IO_WriteOnly | IO_Append ) )
	{
		QTextStream stream( &file );
		file.close();
	}
}

void GofunItem::deleteEntry()
{
	//Kindly warn the user
	if(!(QMessageBox::warning(qApp->mainWidget(),tr("Delete entry"),tr("Do you really want to delete this entry, sir?"), tr("Ok"), tr("Cancel")) == 0))
		return;
	
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

void GofunItem::setData(GofunDesktopEntryData* d)
{
	delete m_data;
	m_data = d;
	
	implementData();
}

void GofunItem::loadIcon()
{
	QPixmap px = GofunMisc::get_icon(data()->Icon,32,32); //@todo: let the user choose the icon-size
	if(!px.isNull())
		setPixmap(px);
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

QPopupMenu* GofunItem::rightClickPopup(const QPoint& pos)
{
	QPopupMenu* popup = new QPopupMenu();
	QObject::connect(popup,SIGNAL(activated(int)),this,SLOT(popupActivated(int)));
	popup->insertSeparator();
	
	if(!isReadOnly())
	{
		popup->insertItem(tr("Edit entry"),PID_Edit);
		popup->insertItem(tr("Delete entry"),PID_Delete);
	}
	else
		popup->insertItem(tr("View settings"),PID_Edit);
	popup->popup(pos);

	return popup;
}

void GofunItem::popupActivated(int id)
{	
	switch(id)
	{		
		case PID_Edit:
			editEntry(); break;
		case PID_Delete: 
			deleteEntry();
			//dynamic_cast<QIconView*>(view_ws->visibleWidget())->arrangeItemsInGrid();break; //FIXME
	}
}

void GofunItem::createNewItem(GofunCatButton* cat)
{
	GofunItemSettings* settings_dlg = new GofunItemSettings();
	int height = 200;
	GofunMisc::attach_window(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->setCategory(cat);
	settings_dlg->exec();
	delete settings_dlg;
}

void GofunItem::implementData()
{
	loadIcon();
	if(!data()->Comment.isEmpty())
		setToolTipText(data()->Comment);
		
	if(GofunMisc::shell_call("[ -w '"+data()->File+"' ] && echo true").simplifyWhiteSpace() != "true")
		readonly = true;
	else
		readonly = false;
}


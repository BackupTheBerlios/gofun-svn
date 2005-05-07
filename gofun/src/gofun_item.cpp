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
#include "gofun_iconview.h"
#include "gofun_settings.h"
#include "gofun_entry_settings.h"

GofunItem* GofunIconViewToolTip::last_active = 0;
 
GofunItem::GofunItem(QIconView* iconview, const QString& string) : QIconViewItem(iconview,string)
{
	m_data = new GofunDesktopEntryData();
	toolTip = NULL;
}

GofunItem::~GofunItem()
{
	delete data();
	delete toolTip;
}

void GofunItem::deleteEntry()
{
	//Kindly warn the user
	if(!(QMessageBox::warning(qApp->mainWidget(),tr("Delete entry"),tr("Do you really want to delete this entry, sir?"), tr("Ok"), tr("Cancel")) == 0))
		return;
	
	deleteEntryFile();
	delete this;
}

void GofunItem::deleteEntryFile()
{
	QFile::remove(data()->File);
}

void GofunItem::setData(GofunDesktopEntryData* d)
{
	m_data = d;
	
	implementData();
}

void GofunItem::loadIcon()
{
	QPixmap px = GofunMisc::getIcon(data()->Icon,32,32); //@todo: let the user choose the icon-size
	if(!px.isNull())
	{
		setPixmap(px);
		
	}
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
	
	if(!data()->isReadOnly())
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
			QIconView* my_icon_view = iconView();
			deleteEntry();
			my_icon_view->arrangeItemsInGrid(); break;
	}
}

void GofunItem::createNewItem(QIconView* iconview)
{
	GofunItem* new_item = new GofunItem(iconview,(""));
	GofunDesktopEntrySettings* settings_dlg = new GofunDesktopEntrySettings();
	int height = 200;
	GofunWindowOperations::attachWindow(qApp->mainWidget(),settings_dlg,D_Above,D_Under,365,200);
	settings_dlg->setCaption(tr("Add entry"));
	settings_dlg->exec();
	delete settings_dlg;
	new_item->implementData();
}

void GofunItem::implementData()
{
	setText(data()->Name);
	loadIcon();
	if(!data()->Comment.isEmpty())
		setToolTipText(data()->Comment);
}


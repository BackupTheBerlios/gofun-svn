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

#include <qvbox.h>
#include <qobjectlist.h>

#include "gofun_shortcut_view.h"
#include "gofun_shortcut_widget.h"

GofunShortcutView::GofunShortcutView(QWidget* parent) : QScrollView(parent)
{
	setResizePolicy(QScrollView::AutoOneFit);

	vbox = new QVBox(viewport());
	addChild(vbox);
}

void GofunShortcutView::insert(GofunShortcutWidget* widget)
{
	widget->reparent(vbox,QPoint(0,0));
}

QKeySequence GofunShortcutView::getShortcut(int SID)
{
	const QObjectList* wl = vbox->queryList("GofunShortcutWidget");

	for(QObjectListIt it(*wl); it.current(); ++it)
	{
		if(static_cast<GofunShortcutWidget*>(it.current()))
		{
			if(static_cast<GofunShortcutWidget*>(it.current())->getSID() == SID)
			{
				QKeySequence shortcut = static_cast<GofunShortcutWidget*>(it.current())->getShortcut();
				delete wl;
				return shortcut;
			}
		}
	}
	delete wl;
	return QKeySequence();
}


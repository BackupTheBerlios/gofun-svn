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

#include <qapplication.h>
 
#include "gofun_list_popup.h"

GofunListPopup::GofunListPopup() : QListView( 0, 0, WType_Popup )
{
	setFrameStyle( WinPanel|Raised );
	resize(150,100);
	setMouseTracking( TRUE );
	
	connect(this,SIGNAL(clicked(QListViewItem*)),this,SLOT(close()));
	connect(this,SIGNAL(returnPressed(QListViewItem*)),this,SLOT(close()));
	connect(this,SIGNAL(spacePressed(QListViewItem*)),this,SLOT(close()));
}

void GofunListPopup::popup(const QPoint& pos)
{
 /*   popupParent = parent;
    setText("Move the mouse!");
    if (popupParent)
        move( popupParent->mapToGlobal( popupParent->rect().bottomLeft() ) );*/
	move(pos);
	fitSizeToContent();
	placeProperly();
	show();
}

void GofunListPopup::fitSizeToContent()
{
	//FIXME: redundant?
	adjustSize();
}

void GofunListPopup::placeProperly()
{
	if(mapToGlobal(QPoint(width(),0)).x() > QApplication::desktop()->width())
		move(x() - (mapToGlobal(QPoint(width(),0)).x() - QApplication::desktop()->width()),y());
	if(mapToGlobal(QPoint(0,height())).y() > QApplication::desktop()->height())
		move(x(),y() - (mapToGlobal(QPoint(0,height())).y() - QApplication::desktop()->height()));
}

void GofunListPopup::closeEvent( QCloseEvent *e ){
    e->accept();
//    if (!popupParent)
        //return;

    // remember that we (as a popup) might recieve the mouse release
    // event instead of the popupParent. This is due to the fact that
    // the popupParent popped us up in its mousePressEvent handler. To
    // avoid the button remaining in pressed state we simply send a
    // faked mouse button release event to it.
    /*QMouseEvent me( QEvent::MouseButtonRelease, QPoint(0,0), QPoint(0,0), QMouseEvent::LeftButton, QMouseEvent::NoButton);*/
    //QApplication::sendEvent( popupParent, &me );
}

void GofunListPopup::mouseReleaseEvent(QMouseEvent * e)
{
	if(!(rect().contains(e->pos())))
		close();
}

void GofunListPopup::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
		case Qt::Key_Escape:
			close();
			break;
	}
	
	QListView::keyPressEvent(e);
}


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
#include <qimage.h>
 
#include "gofun_iconview.h"
#include "gofun_item.h"

GofunIconView::GofunIconView()
{
	setSpacing(3);
	setItemTextBackground(QApplication::palette().brush(QPalette::Active,QColorGroup::Base));
	setAutoArrange(true);
	setResizeMode(QIconView::Adjust);
	
	connect(this,SIGNAL(contentsMoving(int,int)),this,SLOT(contentsMove(int,int)));
}

GofunIconView::~GofunIconView()
{
}

void GofunIconView::contentsMove(int x, int y)
{
	if(paletteBackgroundPixmap())
		updateContents();
}

void GofunIconView::resizeEvent(QResizeEvent* event)
{
	/*if(paletteBackgroundPixmap() && isVisible())
	{
		setPaletteBackgroundPixmap((paletteBackgroundPixmap()->convertToImage()).scale(width(),height()));
	}*/
	QIconView::resizeEvent(event);
}

//We reimplement this method to add automatic change of the grid size
void GofunIconView::arrangeItemsInGrid(bool update)
{	
	int b_height, b_width = 0;
	
	QIconViewItem* item = firstItem();
	while(item)
	{
		//if(item->height() > b_height) //Outcommented because we currently don't need
		//	b_height = item->height(); //it anyways, but maybe in a later implementation
		if(item->width() > b_width)
			b_width = item->width();
		item = item->nextItem();
	}
	
	if(gridX() != b_width && b_width)
		setGridX(b_width);
	//if(gridY() != b_width && b_height) //FIXME: If we uncomment this we end up with
	//	setGridY(b_height); //an endless loop and bugs all the way. Not of high priority thought.
	
	QIconView::arrangeItemsInGrid(update);
}

void GofunIconView::setTopMode()
{
	setItemTextPos(QIconView::Bottom);
	setWordWrapIconText(true);
}

void GofunIconView::setLeftMode()
{
	setItemTextPos(QIconView::Right);
	setWordWrapIconText(false);
}

void GofunIconView::setPalette(const QPalette& pal)
{
	QPixmap bg;
	if(paletteBackgroundPixmap())
		bg = *paletteBackgroundPixmap();	
	
	QIconView::setPalette(pal);
	setItemTextBackground(QApplication::palette().brush(QPalette::Active,QColorGroup::Base));
	
	if(!bg.isNull())
		setPaletteBackgroundPixmap(bg);
}




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

#include <qiconview.h>

#ifndef GOFUN_ICONVIEW
#define GOFUN_ICONVIEW

///Specialised IconView for GoFun 
class GofunIconView : public QIconView
{
	Q_OBJECT;

	public:
	GofunIconView();
	~GofunIconView();
	void setTopMode();
	void setLeftMode();
	void updateBackground();
	
	public slots:
	virtual void arrangeItemsInGrid(bool = TRUE);
	void setBackground(const QString&);
	void setPalette(const QPalette&);
	void contentsMove(int,int);
	
	private:
	virtual void resizeEvent(QResizeEvent*);
	QString background;
};

#endif

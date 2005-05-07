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

#include <qstring.h>
#include <qpixmap.h>
#include <qwidget.h>
 
#ifndef GOFUN_MISC
#define GOFUN_MISC

class QWidget;
class QToolButton;
class QLineEdit;

enum Side {
	D_Left,
	D_Right,
	D_Above,
	D_Under,
	D_None
};

class QProcess;

///Mixed methods needed in GoFun
struct GofunMisc
{
	static QString shellCall(const QString&);
	static QString extendFileString(const QString&);
	static QString binDir();
	static QPixmap getIcon(const QString&,int = 32,int = 32);
	static bool makeDir(const QString&);
	static QString shellifyPath(const QString&);
	static bool stringToBool(const QString&);
	static QString boolToString(bool);
	static void playSound(const QString&);
};

struct GofunWindowOperations
{
	static void centerWindow(QWidget*, int, int);
	static void attachWindow(QWidget* base,QWidget* to_attach, Side pref, Side alt, int width = -1, int height = -1);
};

class GofunLineEdit : public QWidget
{
	public:
	GofunLineEdit();
	
	private:
	QLineEdit* lineedit;
	QToolButton* button;
};

#endif



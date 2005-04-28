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

#include <qstringlist.h>
 
#ifndef GOFUN_PROCESS
#define GOFUN_PROCESs

///Process execution class
/** Designed after the QProcess API.
    The main difference is, that this class
    doesn't redirect the output, this means
    you don't need to catch it, if you just
    want to print it on a terminal. */
class GofunProcess
{
	public:
	GofunProcess();
	void setArguments(const QStringList&);
	void start(QStringList* = 0);
	
	private:
	QStringList arguments;
};

#endif


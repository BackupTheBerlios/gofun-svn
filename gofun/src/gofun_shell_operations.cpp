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

#include <stdio.h>

#include "gofun_shell_operations.h"

QString GofunShellOperations::shellCall(const QString& call)
{
        char buf[1024];
        FILE *ptr;

	QString output;
        if ((ptr = popen(call.ascii(), "r")) != NULL)
                while (fgets(buf, 1024, ptr) != NULL)
                        output += buf;
                (void) pclose(ptr);
        return output;
}

QString GofunShellOperations::shellifyPath(const QString& path)
{
	//QString res = path;
	//return res.replace(' ',"\\ ");
	return "'"+extendFileString(path)+"'";
}

QString GofunShellOperations::extendFileString(const QString& str)
{
	return shellCall("echo -n \""+ str+"\"").stripWhiteSpace();
}


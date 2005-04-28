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

#include "gofun_process.h"

GofunProcess::GofunProcess()
{
}

void GofunProcess::setArguments(const QStringList& args)
{
	arguments = args;
}

void GofunProcess::start(QStringList* envs)
{
	// construct the arguments for exec
	QCString *arglistQ = new QCString[arguments.count() + 1];
	const char** arglist = new const char*[arguments.count() + 1];
	int i = 0;
	for (QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		arglistQ[i] = (*it).local8Bit();
		arglist[i] = arglistQ[i];
		i++;
	}
	arglist[i] = 0;
	
	const char** envslist = const_cast<const char**>(environ);
	if(envs)
	{
		QCString *envslistQ = new QCString[envs->count() + 1];
		envslist = new const char*[envs->count() + 1];
		i = 0;
		for(QStringList::Iterator it = envs->begin(); it != envs->end(); ++it)
		{
			envslistQ[i] = (*it).local8Bit();
			envslist[i] = envslistQ[i];
			i++;
		}
		envslist[i] = 0;
	}
	
	execve(arglist[0],const_cast<char*const*>(arglist),const_cast<char*const*>(envslist));
}


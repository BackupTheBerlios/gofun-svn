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

#include "gofun_process_logger.h"

GofunProcessLogger* GofunProcessLogger::_instance = 0;

void GofunProcessLogger::connectProcToStdout(const QProcess* proc)
{
	connect(proc,SIGNAL(readyReadStdout()),this,SLOT(readProcStdoutToStdout()));
	connect(proc,SIGNAL(readyReadStderr()),this,SLOT(readProcStderrToStdout()));
}

void GofunProcessLogger::connectProcToBuffer(const QProcess* proc,QString* buf)
{
	buffer = buf;
	
	connect(proc,SIGNAL(readyReadStdout()),this,SLOT(readProcStdoutToBuffer()));
	connect(proc,SIGNAL(readyReadStderr()),this,SLOT(readProcStderrToBuffer()));
}

void GofunProcessLogger::readProcStderrToStdout()
{
	QProcess* proc = const_cast<QProcess*>(dynamic_cast<const QProcess*>(sender()));
	while(proc->canReadLineStderr())
		qDebug(proc->readLineStderr());
}

void GofunProcessLogger::readProcStdoutToStdout()
{
	QProcess* proc = const_cast<QProcess*>(dynamic_cast<const QProcess*>(sender()));
	while(proc->canReadLineStdout())
		qDebug(proc->readLineStdout());
}

void GofunProcessLogger::readProcStderrToBuffer()
{
	QProcess* proc = const_cast<QProcess*>(dynamic_cast<const QProcess*>(sender()));
	while(proc->canReadLineStderr())
		*buffer += proc->readLineStderr();
}

void GofunProcessLogger::readProcStdoutToBuffer()
{
	QProcess* proc = const_cast<QProcess*>(dynamic_cast<const QProcess*>(sender()));
	while(proc->canReadLineStdout())
		*buffer += proc->readLineStdout();
}


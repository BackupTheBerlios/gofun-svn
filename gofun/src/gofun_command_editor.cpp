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
 
#include <qlayout.h>
#include <qpushbutton.h>
 
#include "gofun_command_editor.h"

GofunCommandEditor::GofunCommandEditor()
{
	setCaption(tr("GoFun") + tr("Command Editor"));

	text = new QTextEdit(this);
	text->setTextFormat(Qt::PlainText);
	text->setWordWrap(QTextEdit::NoWrap);
	QPushButton* apply = new QPushButton(tr("Apply"),this);
	QPushButton* cancel = new QPushButton(tr("Cancel"),this);
	
	connect(apply,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	
	QGridLayout* grid = new QGridLayout(this,3,3);
	grid->addMultiCellWidget(text,1,1,0,2);
	grid->addWidget(apply,2,0);
	grid->addWidget(cancel,2,1);
}

void GofunCommandEditor::setCommand(const QString& _cmd)
{
	cmd = _cmd;	
	text->setText(cmd.replace(';',"\n"));
}

QString GofunCommandEditor::command()
{
	cmd = text->text();
	cmd = cmd.replace('\n',";");
	return cmd;
}

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

#include <qlayout.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
 
#include "gofun_process_problem_feedback.h"

GofunProcessProblemFeedback::GofunProcessProblemFeedback(QWidget* parent) : QDialog(parent)
{
	setCaption(tr("Launch failure"));

	QGridLayout* grid = new QGridLayout(this);
	grid->layout()->setMargin(5);
	grid->layout()->setSpacing(3);
	
	QGridLayout* grid_report = new QGridLayout();
	grid_report->layout()->setSpacing(3);

	output = new QTextEdit(this);
	output->setReadOnly(true);
	exit_status = new QLabel(this);
	normal_exit = new QLabel(this);
	
	grid_report->addWidget(new QLabel(tr("Normal exit:"),this),0,0);
	grid_report->addWidget(normal_exit,0,1);
	grid_report->addWidget(new QLabel(tr("Exit status:"),this),1,0);
	grid_report->addWidget(exit_status,1,1);
	grid_report->addMultiCellWidget(new QLabel(tr("Output"),this),2,2,0,1);
	grid_report->addMultiCellWidget(output,3,3,0,1);
	
	QPushButton* try_again = new QPushButton(tr("Try again"),this);
	QPushButton* give_up = new QPushButton(tr("Give up"),this);
	
	connect(try_again,SIGNAL(clicked()),this,SLOT(accept()));
	connect(give_up,SIGNAL(clicked()),this,SLOT(reject()));
	
	grid->addMultiCellLayout(grid_report,0,0,0,1);
	grid->addWidget(try_again,1,0);
	grid->addWidget(give_up,1,1);
}

void GofunProcessProblemFeedback::setNormalExit(bool b)
{
	normal_exit->setText(normal_exit ? tr("true") : tr("false"));
}

void GofunProcessProblemFeedback::setExitStatus(int status)
{
	exit_status->setText(QString::number(status));
}

void GofunProcessProblemFeedback::setOutput(const QString& _output)
{
	output->setText(_output);
}




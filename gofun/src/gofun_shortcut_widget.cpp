/************** = *************************************************************
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

#include <qlabel.h>
#include <qtoolbutton.h>

#include "gofun_shortcut_widget.h"

GofunShortcutWidget::GofunShortcutWidget(QWidget* parent,const QString& _description,const QKeySequence& shortcut, int _SID) : QHBox(parent)
{
	setFrameStyle( QFrame::Box | QFrame::Plain );
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
	setMargin(3);
	setPaletteBackgroundColor(QColor(128,128,128));

	description = new QLabel("<b>"+_description+"</b>",this);
	//description->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
	description->setMaximumHeight(32);
	description->setFrameStyle(QFrame::Panel | QFrame::Plain);
	description->setLineWidth(1);
	description->setPaletteBackgroundColor(QColor(150,150,150));

	key = shortcut[0];

	representation = new QLabel(this);
	representation->setFrameStyle(QFrame::Panel | QFrame::Plain);
	representation->setLineWidth(3);
	representation->setPaletteBackgroundColor(QColor(180,180,180));
	representation->setMargin(5);
	representation->setMaximumHeight(32);
	representation->setAlignment(Qt::AlignHCenter);
	QFont font;
	font.setPointSize(QFont().pointSize()+4);
	representation->setFont(font);
	record_shortcut = new QToolButton(this);
	record_shortcut->setPixmap(QPixmap("red_key_record_16.png"));
	record_shortcut->setMaximumHeight(32);
	record_shortcut->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
	SID = _SID;
	
	record = false;

	connect(record_shortcut,SIGNAL(clicked()),this,SLOT(recordShortcut()));

	updateRepresentation();
}

QKeySequence GofunShortcutWidget::getShortcut()
{
	return QKeySequence(key);
}

void GofunShortcutWidget::recordShortcut()
{
	if(!record)
	{
		record_shortcut->setPixmap(QPixmap("blue_key_stop_16.png"));
		key = 0;
		setFocusPolicy(QWidget::ClickFocus);
		setFocus();
		record = true;
	}
	else
	{
		record_shortcut->setPixmap(QPixmap("red_key_record_16.png"));
		record = false;
	}
	updateRepresentation();
}

void GofunShortcutWidget::keyPressEvent(QKeyEvent* e)
{
	if(!record)
		return;

	int modifier = 0;
	switch(e->state())
	{
		case Qt::ShiftButton:
			modifier = Qt::SHIFT;
			break;
		case Qt::MetaButton:
			modifier = Qt::META;
			break;
		case Qt::ControlButton:
			modifier = Qt::CTRL;
			break;
		case Qt::AltButton:
			modifier = Qt::ALT;
			break;
		case Qt::KeyButtonMask:
			modifier = Qt::MODIFIER_MASK;
			break;
	}
	if(!e->text().isEmpty())
		key = (modifier | e->key());

	QWidget::keyPressEvent(e);

	updateRepresentation();
}

void GofunShortcutWidget::updateRepresentation()
{
	representation->setText(QKeySequence(key));
}






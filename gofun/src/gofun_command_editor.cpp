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
#include <qcursor.h>
#include <qapplication.h> 
#include <qheader.h>
#include <qvbuttongroup.h>

#include "gofun_command_editor.h"
#include "gofun_misc.h"
#include "gofun_list_popup.h"
#include "gofun_directory_browser.h"
#include "gofun_application_item.h"
#include "gofun_application_item_settings.h"
#include "gofun_executable_browser.h"

GofunCommandEditor::GofunCommandEditor()
{
	setCaption(tr("Command Editor"));

	text = new QTextEdit(this);
	text->setTextFormat(Qt::PlainText);
	text->setWordWrap(QTextEdit::NoWrap);
	QPushButton* apply = new QPushButton(tr("Ok"),this);
	QPushButton* cancel = new QPushButton(tr("Cancel"),this);
	QPushButton* test = new QPushButton(tr("Test"),this);
	
	connect(text,SIGNAL(textChanged()),this,SLOT(commandExpand()));
	connect(apply,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(test,SIGNAL(clicked()),this,SLOT(test()));
	
	QVButtonGroup* browse_for = new QVButtonGroup(tr("Browse for"),this);
	QPushButton* browse_for_executable = new QPushButton(tr("Executable"),browse_for);
	QPushButton* browse_for_directory = new QPushButton(tr("Directory"),browse_for);
	QPushButton* browse_for_file = new QPushButton(tr("File"),browse_for);
	
	connect(browse_for_executable,SIGNAL(clicked()),this,SLOT(browseForExecutable()));
	connect(browse_for_directory,SIGNAL(clicked()),this,SLOT(browseForDirectory()));
	connect(browse_for_file,SIGNAL(clicked()),this,SLOT(browseForFile()));
	
	QGridLayout* grid = new QGridLayout(this,3,4);
	grid->addMultiCellWidget(text,0,1,0,2);
	grid->addWidget(apply,2,0);
	grid->addWidget(cancel,2,1);
	QSpacerItem* spacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid->addItem(spacer,2,2);
	grid->addWidget(test,2,3);
	grid->addWidget(browse_for,0,3);
	
	settings_widget = 0;
}

void GofunCommandEditor::commandExpand()
{
	if( text->text()[text->text().length()-1] == '/' && text->text()[text->text().length()-2] == '.' )
	{
		GofunListPopup* expand_list = new GofunListPopup;
		expand_list->addColumn("files");
		expand_list->header()->hide();
		qDebug(text->text(text->paragraphs()-1));
		QStringList files = QStringList::split('\n',GofunMisc::shell_call(text->text(text->paragraphs()-1)+"\t"));
		for(QStringList::Iterator it = files.begin(); it != files.end(); ++it)
		new QListViewItem(expand_list,(*it));
		
		QFontMetrics metrics(text->font());
		int x,y;
		text->getCursorPosition(&y,&x);
		
		int xp = metrics.size(Qt::SingleLine,text->text(y)).width();
		int yp = metrics.lineSpacing() * (y+1);
		expand_list->popup(text->mapToGlobal(QPoint(xp,yp)));
		
		connect(expand_list,SIGNAL(clicked(QListViewItem*)),this,SLOT(commandCompletion(QListViewItem*)));
		connect(expand_list,SIGNAL(returnPressed(QListViewItem*)),this,SLOT(commandCompletion(QListViewItem*)));
		connect(expand_list,SIGNAL(spacePressed(QListViewItem*)),this,SLOT(commandCompletion(QListViewItem*)));
	}
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

void GofunCommandEditor::commandCompletion(QListViewItem* item)
{
	if(!item)
		return;
		
	text->insert(item->text(0));
}

void GofunCommandEditor::test()
{
	if(settings_widget)
	{
		GofunApplicationEntryData app_entry = *dynamic_cast<GofunApplicationItem*>(settings_widget->item)->data();
		settings_widget->apply(&app_entry);
		app_entry.Exec = text->text();
		dynamic_cast<GofunApplicationItem*>(settings_widget->item)->executeCommand(&app_entry);
	}
}

void GofunCommandEditor::browseForDirectory()
{
	GofunDirectoryBrowser dir_browser;
	if(dir_browser.exec() == QDialog::Accepted)
		text->insert(dir_browser.selected());
}

void GofunCommandEditor::browseForExecutable()
{
	GofunExecutableBrowser exec_browser;
	if(exec_browser.exec() == QDialog::Accepted)
		text->insert(exec_browser.getExecutable());
}

void GofunCommandEditor::browseForFile()
{
	QString s = QFileDialog::getOpenFileName(
                    "/home",
                    "*.*",
                    this,
                    "Browse for file",
                    "Choose a file" );
	if(s.isNull())
		return;
		
	text->insert(s);
}

void GofunCommandEditor::setSettingsWidget(GofunApplicationItemSettings *_settings_widget)
{
	settings_widget = _settings_widget;
}



